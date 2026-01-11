// vim:set sw=4 ts=8 et fileencoding=utf8:
int a[7];
int b[TU_COUNTOF(a)];
size_t TU_UNIT(void) {
    TU_ASSERT_AND_RETURN(7, TU_COUNTOF(b));
}
