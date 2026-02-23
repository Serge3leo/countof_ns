// vim:set sw=4 ts=8 et fileencoding=utf8:
#ifndef ADD_EVAL_CNT
    #define ADD_EVAL_CNT  (0)
#endif
static size_t TU_UNIT(void) {
    size_t l = 17;
    size_t m = 10;
    size_t n = 25;
    int v[l][m][n];
    size_t i = 0;
    tu_assert(m*n*sizeof(int) == sizeof(v[i++]));
    #if !__cplusplus && !HAVE_BROKEN_SIZEOF
        tu_assert(1 == i);
    #endif
    tu_assert(n*sizeof(int) == sizeof(v[i++][0]));
    #if !__cplusplus && !HAVE_BROKEN_SIZEOF
        tu_assert(2 == i);
    #endif
    i = 0;
    tu_assert(m == TU_COUNTOF(v[i++]));
    TU_ASSERT_AND_RETURN(1 + ADD_EVAL_CNT, i);
}
