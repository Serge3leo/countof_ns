size_t TU_UNIT(void) {
    int c[0][0][0];
    TU_STATIC_ASSERT_AND_RETURN(0, TU_COUNTOF(c));
}
