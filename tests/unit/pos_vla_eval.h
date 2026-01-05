// vim:set sw=4 ts=8 et fileencoding=utf8:
#ifndef ADD_EVAL_CNT
    #define ADD_EVAL_CNT  (0)
#endif
static size_t TU_UNIT(void) {
    size_t m = 1917;
    size_t n = 10;
    int v[m][n];
    size_t i = 0;
    tu_assert(10*sizeof(int) == sizeof(v[i++]));
    #if !__cplusplus
        tu_assert(1 == i);
    #endif
    tu_assert(sizeof(int) == sizeof(v[i++][0]));
    #if !__cplusplus
        tu_assert(1 == i);
    #endif
    i = 0;
    tu_assert(10 == TU_COUNTOF(v[i++]));
    TU_ASSERT_AND_RETURN(1 + ADD_EVAL_CNT, i);
}
