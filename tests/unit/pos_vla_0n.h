size_t TU_UNIT(void) {
    size_t d0 = 0, d1 = 1;
    int a[d0][d1];
    TU_ASSERT_AND_RETURN(d0, TU_COUNTOF(a));
}
