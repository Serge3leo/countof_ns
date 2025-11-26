size_t TU_UNIT(void) {
    int c[0][2][0];
    TU_STATIC_ASSERT_AND_RETURN(2, TU_COUNTOF(c[0]));
}
