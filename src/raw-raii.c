#include <ipc-bus/utils.h>

void voidptr_raii_close(voidptr *data) {
  if (*data != NULL) {
    free(*data);
    ut_logger_info("raii_close (void *) %p\n", *data);
  }
}

void fd_raii_close(int *pfd) {
  if (*pfd > 0) {
    close(*pfd);
    ut_logger_info("raii_close (int) %d\n", *pfd);
  }
}
void charptr_raii_close(char **data) {
  if (*data != NULL) {
    free(*data);
    ut_logger_info("raii_close (void *) %p\n", *data);
  }
}
