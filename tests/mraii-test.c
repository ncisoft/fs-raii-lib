
#include "ipc-bus/mraii.h"
#include <errno.h>
#include <ipc-bus/fs_raii.h>
#include <ipc-bus/utils.h>
#include <logger.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>

void test01() {
  using_raii_func_scope(65536) {
    void *p = malloc(1024);
    mraii_free(p);
    mraii_cleanup() { mraii_free(p); }
  }
}

int main()
{
  logger_init(NULL, LOGGER_LEVEL_DEBUG);
  init_clock();
  msleep(1400);
  //msleep_usleep(2400);
  ut_print_time_elapsed("step-00");
  msleep_usleep(400);
  ut_print_time_elapsed("step-01");
  test01();
  return 0;
}


