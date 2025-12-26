// vim:set sw=4 ts=8 et fileencoding=utf8:
#include <string.h>

char *fc_segv_n = NULL;
size_t fc_segv_s = 0x7ffffffff;
size_t TU_UNIT(void) {
    memset(fc_segv_n, 0, fc_segv_s);
    return 0x7ffffffff;
}
