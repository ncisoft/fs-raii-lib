#pragma once

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <logger.h>
#include <ipc-bus/config.h>
#include <ipc-bus/types.h>
#include <ipc-bus/buffer.h>
#include <ipc-bus/network.h>
#include <ipc-bus/fork.h>
#include <ipc-bus/raw-raii.h>
#include <ipc-bus/mraii.h>
//#include <ipc-bus/memory-pool.h>
#include <ipc-bus/colors.h>
#include <ipc-bus/logger.h>

#ifndef cast
#define cast(t, exp)  ((t)(exp))
#endif

#ifdef __cplusplus
}
#endif