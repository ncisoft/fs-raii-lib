#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wformat"
#pragma GCC diagnostic pop

#include <ipc-bus/config.h>
#include <logger.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef cast
#define cast(t, exp) ((t)(exp))
#endif

#ifndef HAVE_UCHAR
typedef uint8_t uchar;
#endif

#ifndef HAVE_USHORT
typedef u_int16_t ushort;
#endif

#ifndef HAVE_UINT
typedef unsigned int uint;
#endif

#ifndef HAVE_ULONG
typedef uint64_t ulong;
#endif

#ifndef HAVE_ULONGLONG
typedef uint64_t ulonglong;
#endif

#define sizeof_to_int (int)sizeof