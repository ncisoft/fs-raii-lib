#include "logger.h"
#include <ipc-bus/utils.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

typedef void (*sighandler_t)(int);
typedef struct {
    bool is_init;
    int signo;
    sighandler_t old_handle;
} local_var_t;
static local_var_t L = { .is_init = false};

static void my_sigchld_handle(int signo) {
    // padding
    L.signo = signo;
    logger_info("signal by:: %d\n", signo);
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

static inline int pipe_read_fd(int *fd_pair) {
  // padding
  return fd_pair[0];
}

static inline int pipe_write_fd(int *fd_pair) {
  // padding
  return fd_pair[1];
}

extern fork_ctx_t *fork_ctx_t_run(const char *cmd_path, const char *args) {
  fork_ctx_t *ctx = (fork_ctx_t *) calloc(sizeof(fork_ctx_t), 1);
  int pipefd[2];

  _init_local();

  if (ctx == NULL) {
    die(1, "malloc fork_ctx_t failure");
    return NULL;
  }

  if (pipe(pipefd) == -1) {
    die(EXIT_FAILURE, "create pipe failure");
  }
  int child_pid = vfork();
  if (child_pid == 0) {
    int channel_fd = pipe_write_fd(pipefd);
    // it's child process
    close(pipe_read_fd(pipefd)); /* Close unused write end, child use pipefd[0] */
    close(0);         /* close stdout fd */
    close(1);         /* close stderr fd */
    // dup channel_fd to stdout & stderr fd */
    dup2(channel_fd, 0);
    dup2(channel_fd, 1);

    int rc = execl("/bin/bash", "sh", "-c", cmd_path, NULL);
    if (rc == -1) {

      logger_error("execl error\n");
    }
  }
  // parent process
  int channel_fd = pipe_read_fd(pipefd);
  close(pipe_write_fd(pipefd));

  int child_rc = 111;
  printf("rc=%d\n", child_rc);
  printf("signo = %d\n", L.signo);
  waitpid(child_pid, &child_rc, 0);
  printf("signo = %d\n", L.signo);

  printf("child_pid=%d, rc=%d\n", child_pid, child_rc);
  logger_debug("cmd = %s, args = %s, rc=%d\n", cmd_path, args, child_rc);
  while (true) {
    char buf[BUFSIZ];

    int read_len = read(channel_fd, buf, BUFSIZ);
    logger_debug("len = %d, msg = %s, err=%s\n", read_len, buf,
                 strerror(errno));
    break;
  }
  return ctx;
}
extern void fork_ctx_t_free(fork_ctx_t *ctx);
