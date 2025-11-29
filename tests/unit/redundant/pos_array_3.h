size_t TU_UNIT(void) {
    int b[2][3];
    TU_STATIC_ASSERT_AND_RETURN(3, TU_COUNTOF(b[0]));
}
