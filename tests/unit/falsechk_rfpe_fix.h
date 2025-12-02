// vim:set sw=4 ts=8 et fileencoding=utf8:
#define SIZE_A(A)  (sizeof(A) / sizeof((A)[0]))

size_t TU_UNIT(void) {
#if HAVE_EMPTY_INITIALIZER
    double param[] = {};
#else
    double param[0];
#endif
    double jacob[1][SIZE_A(param)];
    size_t cnt = 0;
    for (size_t i = 0; i < SIZE_A(jacob); i++ ) {
        for (size_t j = 0; j < SIZE_A(jacob[0]); j++ ) {
            jacob[i][j] = 0.;
            cnt++;
        }
    }
    (void)param; (void)jacob;
    return cnt;
}
