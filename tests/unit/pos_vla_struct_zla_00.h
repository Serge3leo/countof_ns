size_t TU_UNIT(void) {
    size_t d0 = 0;
    struct { } a[d0];
    TU_ASSERT_AND_RETURN(d0, TU_COUNTOF(a));
}
