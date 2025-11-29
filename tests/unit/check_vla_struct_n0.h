size_t TU_UNIT(void) {
    size_t d1 = 1;
    struct { } a[d1];
    tu_static_assert(0 == sizeof(a[0]));
    TU_ASSERT_AND_RETURN(0, sizeof(a));
}
