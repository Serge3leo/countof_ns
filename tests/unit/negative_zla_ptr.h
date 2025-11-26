static size_t TU_UNIT(void) {
    int (*p)[0];
    return TU_COUNTOF(p);
}
