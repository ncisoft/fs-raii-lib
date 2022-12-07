#pragma once

extern void init_clock();
extern void msleep_select(long msec);
extern void msleep_usleep(long msec);
extern int  msleep(long msec);
extern void print_time_elapsed();
