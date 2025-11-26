static size_t TU_UNIT(void) {
    size_t d1 = 1;
    int a[d1];
    int *p = a; (void)p;
    return TU_COUNTOF(p);
}
