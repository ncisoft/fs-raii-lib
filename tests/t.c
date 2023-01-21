#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <ipc-bus/types.h>

// typedef unsigned int uint;
 typedef unsigned char uchar;

int main()
{
  uint8_t uc;
  uint ui;
  ushort us;
  uint32_t i;
  ulong ul;

  printf("uchar=%d\n", sizeof_to_int(uc));
  printf("ushort=%d\n", sizeof_to_int(us));
  printf("uint32_t=%d\n", sizeof_to_int(i));
  printf("uint=%d\n", sizeof_to_int(ui));
  printf("ulong=%d\n", sizeof_to_int(ul));
  printf("size_t=%d\n", (int)sizeof(size_t));
  printf("%lx\n", SIZE_MAX);

  return 0;
}
