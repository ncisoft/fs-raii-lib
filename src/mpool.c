#include <execinfo.h>
#include <ipc-bus/types.h>
#include <ipc-bus/utils.h>
#include <string.h>

extern char *mpool_strdup(const char *source)
{
	return strdup(source);
}

extern char *mpool_strndup(const char *source, size_t n)
{
	return strndup(source, n);
}

extern void ut_dump_hex(const char *tag, const void *data, size_t size)
{
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	uint32_t len = cast(uint32_t, size);
	ut_logger_debug("%s len=%u\n", tag, len);
	if (size == 8192)
		ut_logger_warn("error%s\n", strerror(errno));
	ut_assert(size < 100);
	for (i = 0; i < size; ++i) {
		if (i % 16 == 0)
			printf("%s %lu", tag, size);
		printf("%02X ", ((unsigned char *)data)[i]);
		if (((unsigned char *)data)[i] >= ' ' &&
		    ((unsigned char *)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char *)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i + 1) % 8 == 0 || i + 1 == size) {
			printf(" ");
			if ((i + 1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i + 1 == size) {
				ascii[(i + 1) % 16] = '\0';
				if ((i + 1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i + 1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

void print_stacktrace()
{
#define MAX_STACK_LEVELS 50

	void *buffer[MAX_STACK_LEVELS];
	int levels = backtrace(buffer, MAX_STACK_LEVELS);

	// print to stderr (fd = 2), and remove this function from the trace
	backtrace_symbols_fd(buffer + 1, levels - 1, 2);
}

extern void ut_println()
{
	printf("\n");
}

void ut_print_stacktrace(const char *fname, int lineno)
{
	int size = 16;
	void *array[16];
	int stack_num = backtrace(array, size);

	ut_logger_debug("backstrace: %s:%d ...\n", fname, lineno);

	char **stacktrace = backtrace_symbols(array, stack_num);
	for (int i = 1; i < stack_num; ++i) {
		printf(__C_MAGENTA__);
		printf("    %s%s\n", stacktrace[i], __C_RESET__);
		if (strstr(stacktrace[i], "main+"))
			break;
	}
	free(stacktrace);

	ut_println();
	//print_stacktrace();
}
