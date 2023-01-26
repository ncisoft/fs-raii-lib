#include "ipc-bus/fork.h"
#include <stdio.h>
#include <ipc-bus/types.h>
#include <ipc-bus/utils.h>

// typedef unsigned int uint;

int main()
{
  fork_ctx_t *folk;
  logger_init(NULL, LOGGER_LEVEL_DEBUG);
  folk = fork_ctx_t_run("pwd", NULL);

  return 0;
}
