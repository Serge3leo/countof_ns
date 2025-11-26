static size_t TU_UNIT(void) {
    const volatile int a[1] = { 0 };
    const volatile int *p = a;
    return TU_COUNTOF(p);
}
