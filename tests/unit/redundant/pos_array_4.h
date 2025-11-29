size_t TU_UNIT(void) {
    int c[4][5][6];
    TU_STATIC_ASSERT_AND_RETURN(5, TU_COUNTOF(c[0]));
}
