size_t TU_UNIT(void) {
    int a[1];
    TU_ASSERT_AND_RETURN(1, TU_COUNTOF(a));
}
