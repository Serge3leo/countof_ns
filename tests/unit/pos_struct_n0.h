// vim:set sw=4 ts=8 et fileencoding=utf8:
size_t TU_UNIT(void) {
    struct { } a[1];
    TU_STATIC_ASSERT_AND_RETURN(1, TU_COUNTOF(a));
}
