size_t TU_UNIT(void) {
    size_t d0 = 0, d1 = 1;
    int b[d1][d0];
    TU_ASSERT_AND_RETURN(d1, TU_COUNTOF(b));
}
