#pragma once

#include "ipc-bus/utils.h"
#ifndef __GNUC__
#error   RAII For c  support gcc only
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include <ipc-bus/types.h>


#define mraii_free(p)                                                          \
  do {                                                                         \
    if (p != NULL) {                                                           \
      free(p);                                                                 \
      ut_logger_debug("%s: free %p\n", __func__, p);                           \
      p = NULL;                                                                \
    } else {                                                                   \
      ut_logger_debug("%s: duplicated free\n", __func__);                      \
    }                                                                          \
  } while (0)

#define mraii_close_fd(fd)                                                     \
  do {                                                                         \
    if (fd > 0) {                                                              \
      close(fd);                                                               \
      fd = 0;                                                                  \
    }                                                                          \
  } while (0)

#define mraii_close_fp(fp)                                                     \
  do {                                                                         \
    if (fp != NULL) {                                                          \
      fclose(fp);                                                              \
      fp = NULL;                                                               \
    }                                                                          \
  } while (0)

#define mraii_cleanup()                                                        \
  for (int pmraii = 0,                                                         \
       mraii_next = pmraii;                                                    \
       mraii_next == pmraii; pmraii++)

#ifdef __cplusplus
}
#endif
