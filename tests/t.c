#include "ipc-bus/fork.h"
#include <stdio.h>
#include <ipc-bus/types.h>
#include <ipc-bus/utils.h>

// typedef unsigned int uint;

int main()
{
  logger_init(NULL, LOGGER_LEVEL_DEBUG);
  fork_ctx_t_run("~/conf/bash/get-os-env.lua HOME", NULL);

  return 0;
}
