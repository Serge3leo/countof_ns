// vim:set sw=4 ts=8 et fileencoding=utf8:
size_t TU_UNIT(void) {
    size_t d0 = 0;
    int b[d0][0];
    TU_ASSERT_AND_RETURN(d0, TU_COUNTOF(b));
}
