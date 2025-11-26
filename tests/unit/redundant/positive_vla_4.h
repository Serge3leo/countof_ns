size_t TU_UNIT(void) {
    size_t d4 = 4, d5 = 5, d6 = 6;
    int c[d4][d5][d6];
    TU_ASSERT_AND_RETURN(d5, TU_COUNTOF(c[0]));
}
