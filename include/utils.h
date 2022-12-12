#pragma once

#include <sys/time.h>
#ifdef __cplusplus
extern "C"
{
#endif
#include <logger.h>


typedef struct
  {
    const char *m_file;
    const char *m_func;
    int m_lineno;
  } ut_logger_context_t;

extern void init_clock();
extern void msleep_select(long msec);
extern void msleep_usleep(long msec);
extern int  msleep(long msec);
extern void ut_print_time_elapsed_impl(ut_logger_context_t *ctx, char *keyword);

#define ut_logger_context_t_init(ctx) ut_logger_context_t ctx={.m_file = __FILE__, .m_func = __func__, .m_lineno = __LINE__}


#define ut_logger_trace(ut_ctx, format, ...) \
    logger_printf(LOGGER_LEVEL_TRACE, LOGGER_COLOR_CYAN, LOGGER_PREFIX format, "TRACE", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_debug(format, ...)   \
    logger_printf(LOGGER_LEVEL_DEBUG, LOGGER_COLOR_WHITE, LOGGER_PREFIX format, "DEBUG", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_info(ut_ctx, format, ...)    \
    logger_printf(LOGGER_LEVEL_INFO, LOGGER_COLOR_GREEN, LOGGER_PREFIX format, "INFO", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_warn(ut_ctx, format, ...)    \
    logger_printf(LOGGER_LEVEL_WARNING, LOGGER_COLOR_YELLOW, LOGGER_PREFIX format, "WARN", _ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_error(ut_ctx, format, ...)   \
    logger_printf(LOGGER_LEVEL_ERROR, LOGGER_COLOR_RED, LOGGER_PREFIX format, "ERROR", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_logger_fatal(ut_ctx, format, ...)   \
    logger_printf(LOGGER_LEVEL_FATAL, LOGGER_COLOR_RED_BOLD, LOGGER_PREFIX format, "FATAL", ut_ctx.m_file, ut_ctx.m_func, ut_ctx.m_lineno, ##__VA_ARGS__)

#define ut_print_time_elapsed(keyword) do {    \
    ut_logger_context_t_init(ctx);             \
    ut_print_time_elapsed_impl(&ctx, keyword); \
                                               \
} while(0)

#ifdef __cplusplus
}
#endif
