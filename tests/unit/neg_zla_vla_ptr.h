static size_t TU_UNIT(void) {
    size_t d0 = 0;
    int a[d0];
    int (*p)[d0] = &a;
    return TU_COUNTOF(p);
}
