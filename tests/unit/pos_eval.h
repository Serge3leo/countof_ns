// vim:set sw=4 ts=8 et fileencoding=utf8:
#ifndef ADD_EVAL_CNT
    #define ADD_EVAL_CNT  (0)
#endif
static size_t TU_UNIT(void) {
    #define L  (17)
    #define M  (10)
    #define N  (25)
    int a[L][M][N];
    size_t i = 0;
    tu_assert(M*N*sizeof(int) == sizeof(a[i++]));
    tu_assert(N*sizeof(int) == sizeof(a[i++][0]));
    tu_assert(0 == i);
    tu_assert(M == TU_COUNTOF(a[i++]));
    TU_ASSERT_AND_RETURN(0 + ADD_EVAL_CNT, i);
}
