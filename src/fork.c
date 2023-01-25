#include <ipc-bus/utils.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAG_TOP "[ top process] "
#define TAG_WAIT "[wait process] "
#define TAG_EXCEL "[excel process] "
#define TAG_XPIPE "[xpipe] "

typedef void (*sighandler_t)(int);
typedef struct {
  bool is_init;
  int signo;
  sighandler_t old_handle;
  int tmp_file_fd;
} local_var_t;

typedef union {
  uint32_t pid;
  uchar buf[sizeof(uint32_t)];

} union_Pid_t;
static local_var_t L = {.is_init = false};

static void my_sigchld_handle(int signo) {
  // padding
  L.signo = signo;
  ut_logger_info("signal by:: %d, pid=%d\n", signo, getpid());
}

static void _init_local() {
  if (L.is_init == false) {
    sighandler_t old_handle = signal(SIGCHLD, my_sigchld_handle);

    L.old_handle = old_handle;
    if (old_handle == SIG_ERR) {
      die(EXIT_FAILURE,
          "setup SIGCHLD to SIG_IGN to avoid filter process crash fail\n");
    }
  }
}

static inline int pipe_get_read_fd(int *fd_pair) {
  // padding
  return fd_pair[0];
}

static inline int pipe_get_write_fd(int *fd_pair) {
  // padding
  return fd_pair[1];
}

static int open_tmp_file() {
  if (L.tmp_file_fd != 0)
    return L.tmp_file_fd;
  int fd = open("/tmp/0.bin", O_CREAT | O_TRUNC | O_RDWR, 00644);
  if (fd != -1) {
    L.tmp_file_fd = fd;
    return fd;
  }
  die(1, "open tmp-file fail\n");
  return 0;
}
static ssize_t xpipe_read_str(int fd, void *buf, size_t len) {
  ssize_t read_len;
  char *cp = cast(char *, buf);
  read_len = read(fd, buf, len);
  if (read_len > 0) {
    cp[read_len] = '\0';
  }
  return read_len;
}

static inline ssize_t xpipe_read(int fd, void *buf, size_t len) {
  ssize_t read_len;
  read_len = read(fd, buf, len);
  if (read_len == -1)
    ut_logger_warn(TAG_XPIPE " read:::%s\n", strerror(errno));
  ut_logger_warn(TAG_XPIPE "read.fd=%d, len= %lld\n", fd, read_len);
  return read_len;
}

static inline ssize_t xpipe_write(int pipe_fd, void *buf, size_t len) {
  int tmp_file_fd = open_tmp_file();
  ssize_t write_len;
  write_len = write(pipe_fd, buf, len);
  write_len = write(tmp_file_fd, buf, len);
  if (write_len == -1) {
    ut_logger_warn(TAG_XPIPE "write:::%s\n", strerror(errno));
  }
   ut_logger_debug(TAG_XPIPE "write.fd=%d, len= %lld\n",pipe_fd, write_len);
  return write_len;
}

static inline ssize_t xpipe_write_str(int fd, void *buf, size_t len) {
  ssize_t write_len;
  char *cp = cast(char *, buf);
  if (len >= 0) {
    cp[len] = '\0';
    len++;
  }
  write_len = xpipe_write(fd, buf, len);
  return write_len;
}

fork_ctx_t *fork_ctx_t_run(const char *cmd_path, const char *args) {
  fork_ctx_t *ctx = (fork_ctx_t *)calloc(sizeof(fork_ctx_t), 1);
  int pipefd_wait_fork[2];

  _init_local();

  if (ctx == NULL) {
    die(1, "malloc fork_ctx_t failure");
    return NULL;
  }

  // init wait)fork_pipfd
  if (pipe(pipefd_wait_fork) == -1) {
    die(EXIT_FAILURE, "create pipe failure");
  }
  ut_logger_debug(TAG_TOP " stdin.fileno=%d\n", fileno(stdin));
  ut_logger_debug(TAG_TOP "stdout.fileno=%d\n", FD_STDOUT);
  ut_logger_debug(TAG_TOP "stderr.fileno=%d\n", FD_STDERR);
  printf("\n");
  ut_logger_info(TAG_TOP "wait_channel_fd=%d\n", pipefd_wait_fork[0]);

  int wait_child_pid = fork();
  if (wait_child_pid == -1) {
    die(1, "fork wait process\n");
  }
  if (wait_child_pid == 0) {
    label(TAG_WAIT, "wait process part[01/03] here");
    int pipefd_execl_fork[2];
    // init pipefd_execl)fork
    if (pipe(pipefd_execl_fork) == -1) {
      // padding
      die(EXIT_FAILURE, "create excel failure");
    }
    if (true) {
      label(TAG_WAIT, "wait_process part[02/03] do file handle");
      int channel_fd = pipe_get_write_fd(pipefd_wait_fork);
      close(FD_STDIN);  /* close stdin fd */
      //close(FD_STDOUT); /* close stdout fd */
      //close(FD_STDERR); /* close stderr fd */
      if (false) {
        // FIXME: don't use the bellow statement
        // otherwise excel process couldn't pass back
        // pipefd_execl_fork && pipefd_wait_fork 
        close(pipe_get_write_fd(pipefd_execl_fork));
        dup2(channel_fd, FD_STDOUT); // dump to stdout
        dup2(channel_fd, FD_STDERR); // dump to stderr
      }
      
    }

    int execl_child_pid = vfork();
    if (execl_child_pid == 0) {
      label(TAG_EXCEL, "excel process here");
      int excel_channel_fd = pipe_get_write_fd(pipefd_execl_fork);
      // child */
      close(FD_STDIN);  /* close stdin fd */
      close(FD_STDOUT); /* close stderr fd */
      close(FD_STDERR); /* close stderr fd */
      close(pipe_get_read_fd(pipefd_execl_fork));
      close(pipe_get_read_fd(pipefd_wait_fork)); /* Close unused read end
      // dup channel_fd to stdout & stderr fd */
      // If the file descriptor newfd was previously open, it is  silently
      // closed  before being reused.
      dup2(excel_channel_fd, FD_STDOUT);
      dup2(excel_channel_fd, FD_STDERR);
      int rc = execl("/bin/bash", "sh", "-c", cmd_path, NULL);
      if (rc == -1) {
        _exit(EXIT_FAILURE);
      }
      ut_logger_error("execl error\n");
      // would not reach here
      _exit(1);
    } else {
      label(TAG_WAIT,"wait process part[03/03] here, after fork excel");
      int excel_child_rc = 111;
      int wait_channel_fd = pipe_get_write_fd(pipefd_wait_fork);
      int excel_channel_fd = pipe_get_read_fd(pipefd_execl_fork);
      uint32_t uint32_excel_pid;

      ut_logger_warn(TAG_WAIT "exec.pid=0x%X, %d\n", execl_child_pid,
                     execl_child_pid);
      //  step-1: write excel pid
      uint32_excel_pid = cast(uint32_t, execl_child_pid);
      ut_logger_info(TAG_WAIT "wait_channel_fd=%d\n", wait_channel_fd);
      //  write part[01/03] excel exit pid
      xpipe_write(wait_channel_fd, &uint32_excel_pid, sizeof(uint32_excel_pid));
      int rc = waitpid(execl_child_pid, &excel_child_rc, 0);
      //  step-2: write excel pid
      if (rc == -1) {
        die(1, "...");
      }
      uint32_t uint32_exit_code = cast(uint32_t, excel_child_rc);
      //  write part[03/03] excel exit code
      xpipe_write(wait_channel_fd, &uint32_exit_code, sizeof(uint32_exit_code));

      ut_logger_debug(TAG_WAIT "wait_channel_fd=%d\n", wait_channel_fd);
      ut_logger_debug(TAG_WAIT " wait.pid=0x%X, \n", getpid());
      ut_logger_debug(TAG_WAIT "execl.pid=0x%X, %d, excel.exit_code=%d\n",
                      execl_child_pid, execl_child_pid, excel_child_rc);
      ut_logger_warn(TAG_WAIT "execl.pid=%d, excel.exit_code=%d\n",
                      execl_child_pid, excel_child_rc);
      ut_logger_warn(TAG_WAIT " cmd = %s, args = %s, excel.exit_code=%d\n",
                      cmd_path, args, excel_child_rc);
      printf("\n");
      char buf[BUFSIZ];
      bzero(buf, sizeof(buf));
      int read_len = xpipe_read(excel_channel_fd, buf, sizeof(buf));
      ut_logger_debug(TAG_WAIT "execl.return.len=%d\n", read_len);
      if (read_len != -1) {
        buf[read_len] = '\0';
        ut_logger_debug(TAG_WAIT " execl.return.msg = %s\n", buf);
      }
      //  step-3: write msg
      xpipe_write_str(wait_channel_fd, buf, strlen(buf));
      ut_logger_debug(TAG_WAIT " \n---\n\n");
      sleep(0);
      exit(EXIT_SUCCESS);
    }

    // stop end
  } else if (wait_child_pid > 0) {
    label(TAG_TOP,  "top process here");
    int wait_channel_fd = pipe_get_read_fd(pipefd_wait_fork);
    int child_rc = 111;
    close(pipe_get_write_fd(pipefd_wait_fork));
    ctx->wait_child_pid = wait_child_pid;
    ctx->channel_fd = wait_channel_fd;

    ut_logger_debug(TAG_TOP "rc=%d\n", child_rc);
    ut_logger_debug(TAG_TOP "signo = %d\n", L.signo);
    // step-0: waitpid
    waitpid(wait_child_pid, &child_rc, 0);
    ut_logger_debug(TAG_TOP "signo = %d, top.pid=%d\n", L.signo, getpid());
    ut_logger_info(TAG_TOP "wait_channel_fd=%d\n", wait_channel_fd);
    ut_logger_debug(TAG_TOP "wait process: child_pid=%d, rc=%d\n",
                    wait_child_pid, child_rc);
    ut_logger_debug(TAG_TOP "cmd = %s, args = %s, watch-proc.exit_code=%d\n",
                    cmd_path, args, child_rc);
    // TODO: top process
    {
      uint32_t uint32_excel_pid, uint32_pid;
      uint32_t excel_rc;
      int wait_status;

      int read_len;
      uint32_excel_pid = 0;
      // step-1: read excel pid
      read_len = xpipe_read(wait_channel_fd, &uint32_excel_pid, sizeof(uint32_pid));
      ut_logger_warn(TAG_TOP "  top.pid=%d\n", getpid());
      ut_logger_warn(TAG_TOP "execl.pid=0x%X, %d\n", uint32_excel_pid,uint32_excel_pid);
      ut_logger_warn(TAG_TOP " wait.pid=%d\n", wait_child_pid);
      ut_logger_debug(TAG_TOP "len = %d, excel_pid = %d, err=%s\n", read_len,
                      uint32_excel_pid, strerror(errno));
      // step-2: read excel exit code
      read_len = xpipe_read(wait_channel_fd, &excel_rc, sizeof(excel_rc));
      ut_logger_debug(TAG_TOP "len = %d, excel_exit_code = %d, err=%s\n",
                      read_len, uint32_excel_pid, strerror(errno));
    }
    while (true) {
      char buf[BUFSIZ];
      ssize_t read_len;
      bzero(buf, sizeof(buf));
      // read msg
      read_len = xpipe_read_str(wait_channel_fd, buf, sizeof(buf));
      if (read_len > 0)
        ut_logger_debug(TAG_TOP "len = %d, msg = %s, err=%s\n", read_len, buf,
                        strerror(errno));
      if (read_len <= 0)
        break;
    }
    // top process
    return ctx;
  }
  return ctx;
}
// extern void fork_ctx_t_free(fork_ctx_t *ctx);
