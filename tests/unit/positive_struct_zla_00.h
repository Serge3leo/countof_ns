size_t TU_UNIT(void) {
    struct { } a[0];
    TU_STATIC_ASSERT_AND_RETURN(0, TU_COUNTOF(a));
}
