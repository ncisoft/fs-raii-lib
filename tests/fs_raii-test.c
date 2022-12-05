#include <stdio.h>
#include <func_gc.h>
#include <logger.h>

int main()
{
  logger_init(NULL, LOGGER_LEVEL_DEBUG | LOGGER_COLOR_ON);

  logger_error("color error\n");
  logger_warn("color warning\n");
  logger_info("color infomation\n");
  logger_debug("color debug\n");
  return 0;
}

