#include <stdio.h>
#include <fs_raii.h>
#include <logger.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <utils.h>

typedef struct timeval ut_clock_t;

static struct
{
  bool clock_inited ;
  u_int clock_counter;
  ut_clock_t start_t;
} L =
{
  .clock_inited = false,
  .clock_counter= 0
};


/**
 * 1 second = 1,000 milliseconds
 * 1 second = 1,000 * 1,000 microsecond
 * 1 second = 1,000 * 1,000 * 1,000 nanoseconds
 */

ut_clock_t get_clock_t()
{
  ut_clock_t now;
  gettimeofday(&now, NULL);
  return now;
}

void init_clock()
{
  if (L.clock_inited == false)
    {
      L.clock_inited = true;
      L.start_t = get_clock_t();
    }
}

ut_clock_t time_offset()
{
  ut_clock_t now, ts_offset;
  if (L.clock_inited == false)
    {
      init_clock();
      now = L.start_t;
    }
  else
    now = get_clock_t();
  timersub(&now, &L.start_t, &ts_offset);
  logger_debug("time_offset: %ld.%06ld\n", (long int)ts_offset.tv_sec, (long int)ts_offset.tv_usec);
  return ts_offset;
}

void ut_print_time_elapsed_impl(ut_logger_context_t *ctx, char *keyword)
{
  ut_clock_t elapsed = time_offset();
  ut_logger_info("[%s] elapsed time: %ld.%06ld\n", keyword, (long int)elapsed.tv_sec, (long int)elapsed.tv_usec);
}

/* msleep(): Sleep for the requested number of milliseconds. */
void msleep_select(long msec)
{
  struct timeval timeout = {.tv_sec= msec/1000, .tv_usec=msec%1000*1000};
  select(0, NULL, NULL, NULL, &timeout);
}

void msleep_usleep(long msec)
{
  usleep(msec*1000);
}

int msleep(long msec)
{
  struct timespec ts;
  int res;

  if (msec < 0)
    {
      errno = EINVAL;
      return -1;
    }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
      res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}
