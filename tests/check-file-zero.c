#include "ipc-bus/raw-raii.h"
#include "logger.h"
#include <bits/stdint-uintn.h>
#include <ipc-bus/utils.h>
#include <getopt.h>
#include <stdio.h>

double check_zero(const char *fname) {
  FILE *fp = fopen(fname, "rb");
  int fd;
  uint64_t file_len = 0;
  uint64_t zero_count = 0;
  double _double_1 = 1.0;
  double ratio, a, b;
  uint32_t buf_size = 64 * 1024 * 1024; // 64M
  declare_charptr(buf);


  buf = malloc(buf_size);
  assert(buf != NULL); 
  if (!buf) {
    die(1, "Fail to malloc %d memry\n", buf_size);
  }
  if (!fp) {
    die(1, "fail to open %s\n", fname);
  }
  fd = fileno(fp);

  while (true) {
    size_t len = read(fd, buf, sizeof(buf));
    if (len <= 0)
      break;
    for (size_t i = 0; i < len; i++) {
      if (buf[i] == '\0')
        zero_count++;
    }
    file_len += len;
  }
  ut_logger_debug("%d/%d\n", zero_count, file_len);
  a = _double_1 * zero_count;
  b = _double_1 * file_len;
  ratio = a / b;

  return ratio;
}

enum {
 GETOPT_ID_TARGET = 't',
 GETOPT_ID_HELP = 'h',
};

static struct option long_options[] = {
    { "target",    required_argument,   NULL, GETOPT_ID_TARGET },
    { "help",   no_argument,       NULL, GETOPT_ID_HELP },
    { NULL, 0, NULL, 0 }
};

struct {
  char *m_fname;
} _opts = {.m_fname = NULL};

void _help_exit(int lineno)
{
#define TABX "    "
  char help[] = ""
    "Usage: check-file-zero.exe -t target-filepath -h\n"
    "Options:\n"
    TABX "-t* --target\t" "target-filepath"
    TABX "-h --help\t" "print this help message\n";
  printf("%s\n", help);
  printf("lineno = %d\n", lineno);
  _exit(-1);
}

int main(int argc, char **argv) {
  double ratio;
  int c;

  logger_init(NULL, LOGGER_LEVEL_DEBUG);
  while ((c = getopt_long(argc, argv, "t:h", long_options, NULL)) != -1) {
    switch (c) {
    case GETOPT_ID_TARGET:
      _opts.m_fname = strdup(optarg);
      break;
    case GETOPT_ID_HELP:
      _help_exit(__LINE__);
      break;
    default:
      ut_logger_debug("m_fname === $s\n", _opts.m_fname);
      printf("%c: %s\n", c, optarg);
      _help_exit(__LINE__);
    }
  }
  if (!_opts.m_fname) {
    ut_logger_debug("m_fname = $s\n", _opts.m_fname);
    _help_exit(__LINE__);
  }

  ratio = check_zero(_opts.m_fname);
  ut_logger_debug("ratio=%0.4lf\n", ratio);
  return 0;
}
