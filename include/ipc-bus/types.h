#pragma once

#include <ipc-bus/config.h>

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifndef cast
#define cast(t, exp) ((t)(exp))
#endif

#define sizeof_to_int (int)sizeof
