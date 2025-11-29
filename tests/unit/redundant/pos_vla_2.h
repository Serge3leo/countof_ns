// vim:set sw=4 ts=8 et fileencoding=utf8:
size_t TU_UNIT(void) {
    size_t d2 = 2, d3 = 3;
    int b[d2][d3];
    TU_ASSERT_AND_RETURN(d2, TU_COUNTOF(b));
}
