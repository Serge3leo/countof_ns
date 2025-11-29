size_t TU_UNIT(void) {
    const int a[1] = { 0 };
    TU_STATIC_ASSERT_AND_RETURN(1, TU_COUNTOF(a));
}
