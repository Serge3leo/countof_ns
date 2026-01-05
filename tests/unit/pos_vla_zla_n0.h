// vim:set sw=4 ts=8 et fileencoding=utf8:
size_t TU_UNIT(void) {
    size_t d7 = 7;
    int b[d7][0];
    tu_static_assert(0 == sizeof(b[0]));
    tu_assert(0 == sizeof(b));
    TU_ASSERT_AND_RETURN(d7, TU_COUNTOF(b));
}
