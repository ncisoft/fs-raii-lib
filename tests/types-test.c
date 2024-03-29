#include <stdint.h>
#include <stdio.h>

#include <ipc-bus/types.h>
#include <ipc-bus/utils.h>
#include <pt.h>

typedef struct {
  ev_io m_io;
  ev_timer m_watcher;
  struct pt m_pt;
} connection_t;


// typedef unsigned int uint;

 void printf_int(const char*name, int v) {
    printf("%-8s = %d\n", name, v);
 }

 void printf_ulong64(const char*name, uint64_t v) {
  char fmt[80];
  sprintf(fmt,"%s%s\n", "%-9s= 0x", "%"PRIu64);
  printf("%s\n", fmt);
  printf(fmt, name, v);
  ut_assert(0>1);
 }

void clean_up(int *final_value)
{
  printf("Cleaning up\n");
  printf("Final value: %d\n",*final_value);

}

int main()
{
  uchar uc;
  ushort us;
  uint ui;
  uint32_t i;
  ulong ul;
  declare_voidptr(buf) = malloc(888);
  declare_fd(fd);
  nop(buf);
  fd = open("/tmp/0.bin", O_RDONLY,0777);

  logger_init(NULL, LOGGER_LEVEL_DEBUG);

  printf_int("uchar", sizeof_to_int(uc));
  printf_int("ushort", sizeof_to_int(us));
  printf_int("uint32_t", sizeof_to_int(i));
  printf_int("uint", sizeof_to_int(ui));
  printf_int("ulong", sizeof_to_int(ul));
  printf_int("ulonglong", sizeof_to_int(ulonglong));
  printf_int("size_t", (int)sizeof(size_t));
  printf_int("pid_t", (int)sizeof(pid_t));
  printf_int("BUFSIZ", BUFSIZ);

  printf_ulong64("SIZE_MAX", SIZE_MAX);
  puts("");
  ut_logger_debug("buf=%p\n", buf);


#ifdef IPC_BUS_DEBUG
  ut_logger_warn("DEBUG was defined\n");
#else
  printf("DEBUG was not defined\n");
#endif
  return 0;
}
