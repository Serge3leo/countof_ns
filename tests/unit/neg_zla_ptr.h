// vim:set sw=4 ts=8 et fileencoding=utf8:
static size_t TU_UNIT(void) {
    int (*p)[0];
    return TU_COUNTOF(p);
}
