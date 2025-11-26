size_t TU_UNIT(void) {
    const volatile int a[1] = { 0 };
    TU_ASSERT_AND_RETURN(1, TU_COUNTOF(a));
}
