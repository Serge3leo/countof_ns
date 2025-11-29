size_t TU_UNIT(void) {
    size_t d2 = 2, d3 = 3;
    int b[d2][d3];
    TU_ASSERT_AND_RETURN(d3, TU_COUNTOF(b[0]));
}
