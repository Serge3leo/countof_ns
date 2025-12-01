// vim:set sw=4 ts=8 et fileencoding=utf8:
static size_t TU_UNIT(void) {
    const volatile int *p = 0; (void)++p;
    return TU_COUNTOF(p);
}
