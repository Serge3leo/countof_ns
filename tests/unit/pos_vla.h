// vim:set sw=4 ts=8 et fileencoding=utf8:
size_t TU_UNIT(void) {
    size_t d1 = 1;
    int a[d1];
    TU_ASSERT_AND_RETURN(d1, TU_COUNTOF(a));
}
