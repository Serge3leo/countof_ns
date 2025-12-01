// vim:set sw=4 ts=8 et fileencoding=utf8:
static size_t TU_UNIT(void) {
    int i = 0; (void)++i;
    return TU_COUNTOF(i);
}
