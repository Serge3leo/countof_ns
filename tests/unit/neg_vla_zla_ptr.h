// vim:set sw=4 ts=8 et fileencoding=utf8:
static size_t TU_UNIT(void) {
    size_t d0 = 0;
    int b[d0][0];
    int (*p)[d0][0] = &b;
    (void)p;
    return TU_COUNTOF(p);
}
