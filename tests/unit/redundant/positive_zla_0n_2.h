size_t TU_UNIT(void) {
    int b[0][1];
    TU_STATIC_ASSERT_AND_RETURN(0, TU_COUNTOF(b));
}
