#include <ipc-bus/utils.h>

void _ut_shorten_filename(ut_logger_context_t *ut_ctx) {
  char delimiter = '/';
  const char *cp_head = ut_ctx->m_file;
  char *cp = strrchr(cp_head, delimiter);
  if (cp != NULL) {
    cp --;
    while (cp > cp_head && *cp != delimiter) {
      cp--;
    }
    cp++;
    ut_ctx->m_file = cp;
  }
}

void _ut_shortest_filename(ut_logger_context_t* ut_ctx) {
  char delimiter = '/';
  const char *cp_head = ut_ctx->m_file;
  char *cp = strrchr(cp_head, delimiter);
  if (cp != NULL) {
    cp++;
    ut_ctx->m_file = cp;
    return;
  }
}