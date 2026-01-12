// vim:set sw=4 ts=8 et fileencoding=utf8:
size_t TU_UNIT(void) {
    TU_ASSERT_AND_RETURN(7, TU_COUNTOF(int [7]));
}
