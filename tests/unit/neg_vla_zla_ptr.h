static size_t TU_UNIT(void) {
    size_t d0 = 0;
    int b[d0][0];
    int (*p)[d0][0] = &b;
    return TU_COUNTOF(p);
}
