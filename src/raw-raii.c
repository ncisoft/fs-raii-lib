#include <ipc-bus/utils.h>

void ut_unused() {}
void voidptr_raii_close(voidptr *data) {
  if (*data != NULL) {
    free(*data);
    ut_logger_warn("raii_close (void *) %p\n", *data);
  }
}

void fd_raii_close(int *pfd) {
  if (*pfd > 0) {
    close(*pfd);
    ut_logger_warn("raii_close (int) %d\n", *pfd);
  }
}