#include <stdio.h>
#include <fs_raii.h>
#include <logger.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <utils.h>

int main()
{
  logger_init(NULL, LOGGER_LEVEL_DEBUG);
  if (false)
    {
      struct timeval tval_before, tval_after, tval_result;

      gettimeofday(&tval_before, NULL);

      // Some code you want to time, for example:
      msleep_select(1400);

      gettimeofday(&tval_after, NULL);

      timersub(&tval_after, &tval_before, &tval_result);

      printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

    }


  if (false)
    {
      logger_error("color error\n");
      logger_warn("color warning\n");
      logger_info("color infomation\n");
      logger_debug("color debug\n");
      logger_debug("LOGGER_LEVEL_TRACE=%X\n", LOGGER_LEVEL_TRACE);
      logger_debug("LOGGER_LEVEL_DEBUG=%X\n", LOGGER_LEVEL_DEBUG);
      logger_debug("LOGGER_LEVEL_INFO=%X\n", LOGGER_LEVEL_INFO);
      logger_debug("LOGGER_LEVEL_WARNING=%X\n", LOGGER_LEVEL_WARNING);
      logger_debug("LOGGER_LEVEL_ERROR=%X\n", LOGGER_LEVEL_ERROR);
      logger_debug("LOGGER_LEVEL_FATAL=%X\n", LOGGER_LEVEL_FATAL);
      logger_debug("%o\n", LOGGER_LEVEL_FATAL);
      logger_debug("%X\n", LOGGER_LEVEL_MASK);
      logger_debug("LOGGER_LEVEL_MASK=%X\n", LOGGER_LEVEL_MASK);
      logger_debug("LOGGER_LEVEL_MASK=%o\n", LOGGER_LEVEL_MASK);
      logger_debug("LOGGER_LEVEL_MASK=%o\n", LOGGER_LEVEL_DEBUG | LOGGER_COLOR_ON);

    }

  init_clock();
  msleep(1400);
  //msleep_usleep(2400);
  print_time_elapsed();
  msleep_usleep(400);
  print_time_elapsed();
  return 0;
}


