static size_t TU_UNIT(void) {
    // Former: short_example.h:72: fail += countof_ns(p1)
    int a2[56][23] = { { 0 } };
    int (*p1)[23] = &a2[0];
    return TU_COUNTOF(p1);
}
