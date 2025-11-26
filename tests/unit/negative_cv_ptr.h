static size_t TU_UNIT(void) {
    const volatile int *p = 0; (void)p;
    return TU_COUNTOF(p);
}
