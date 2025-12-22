// vim:set sw=4 ts=8 et fileencoding=utf8:
#define SIZE_A(A)  (sizeof(A) / sizeof((A)[0]))

size_t TU_UNIT(void) {
    size_t m = 1;
#if HAVE_EMPTY_INITIALIZER
    double param[] = {};
#elif HAVE_ZLA
    double param[0];
#else
    size_t n = 0;
    double param[n];
#endif
    double jacob[m][SIZE_A(param)];
    size_t cnt = 0;
    for (size_t i = 0; i < SIZE_A(jacob); i++ ) {
        for (size_t j = 0; j < SIZE_A(jacob[0]); j++ ) {
            jacob[i][j] = 0.;
            cnt++;
        }
    }
    return cnt;
}
