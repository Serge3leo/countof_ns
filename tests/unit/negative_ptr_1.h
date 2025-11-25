static size_t TU_UNIT(void) {
    int a[1] = { 0 };
    int *p = a;
    return TU_COUNTOF(p);
}
