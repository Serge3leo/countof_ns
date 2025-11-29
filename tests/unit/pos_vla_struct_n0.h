size_t TU_UNIT(void) {
    size_t d1 = 1;
    struct { } a[d1];
    tu_static_assert(0 == sizeof(a[0]));
    assert(0 == sizeof(a));
    TU_ASSERT_AND_RETURN(d1, TU_COUNTOF(a));
}
