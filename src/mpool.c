#include <ipc-bus/utils.h>


extern char *mpool_strdup(const char *source) {
 return strdup(source);
}
extern char *mpool_strndup(const char *source, size_t n) {
    return strndup(source, n);
}