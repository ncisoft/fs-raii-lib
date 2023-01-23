#include <ipc-bus/types.h>
#include <ipc-bus/utils.h>
#include <stdint.h>
#include <stdio.h>

// typedef unsigned int uint;

 void printf_int(const char*name, int v) {
    printf("%-8s = %d\n", name, v);
 }

 void printf_ulong64(const char*name, uint64_t v) {
    printf("%-9s= %lux\n", name, v);
 }

int main()
{
  uint8_t uc;
  uint ui;
  ushort us;
  uint32_t i;
  ulong ul;


  logger_init(NULL, LOGGER_LEVEL_DEBUG);

  printf_int("uchar", sizeof_to_int(uchar));
  printf_int("ushort", sizeof_to_int(us));
  printf_int("uint32_t", sizeof_to_int(i));
  printf_int("uint", sizeof_to_int(ui));
  printf_int("ulong", sizeof_to_int(ul));
  printf_int("ulonglong", sizeof_to_int(ulonglong));
  printf_int("size_t", (int)sizeof(size_t));
  printf_int("BUFSIZ", BUFSIZ);

  printf_ulong64("SIZE_MAX", SIZE_MAX);
  puts("");

  return 0;
}
