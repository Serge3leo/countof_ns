static size_t TU_UNIT(void) {
    // Former: short_example.h:73: fail += countof_ns(p2);
    int a1[42] = { 0 };
    int *p2 = a1;
    return TU_COUNTOF(p2);
}
