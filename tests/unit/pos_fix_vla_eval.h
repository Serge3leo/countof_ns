// vim:set sw=4 ts=8 et fileencoding=utf8:
#ifndef ADD_EVAL_CNT
    #define ADD_EVAL_CNT  (0)
#endif
static size_t TU_UNIT(void) {
    #define L  (17)
    #define M  (10)
    volatile size_t n = 25;
    int v[L][M][n];
    size_t i = 0;
    tu_assert(M*n*sizeof(int) == sizeof(v[i++]));
    #if !__cplusplus && !HAVE_BROKEN_SIZEOF
        #if __SUNPRO_C  // TODO: Bug or change test?
            tu_assert(0 == i);
        #else
            tu_assert(1 == i);
        #endif
    #endif
    tu_assert(n*sizeof(int) == sizeof(v[i++][0]));
    #if !__cplusplus && !HAVE_BROKEN_SIZEOF
        #if __SUNPRO_C
            tu_assert(1 == i);
        #else
            tu_assert(2 == i);
        #endif
    #endif
    i = 0;
    tu_assert(M == TU_COUNTOF(v[i++]));
    TU_ASSERT_AND_RETURN(0 + ADD_EVAL_CNT, i);
}
