#pragma once
#include <ipc-bus/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fork_ctx {
  char *cmdline;
  char *args;
  pid_t wait_child_pid;
  int channel_fd;

} fork_ctx_t;
;
extern fork_ctx_t *fork_ctx_t_run(const char *cmd_path, const char *args);
extern fork_ctx_t *fork_ctx_t_new(const char *cmd_path, char *args);
extern void fork_ctx_t_free(fork_ctx_t *ctx);
extern int fork_execl(fork_ctx_t *ctx, const char *cmd_path, const char *arg, ...);

#ifdef __cplusplus
}
#endif
