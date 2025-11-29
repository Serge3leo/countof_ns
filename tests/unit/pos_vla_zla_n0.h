size_t TU_UNIT(void) {
    size_t d1 = 1;
    int b[d1][0];
    tu_static_assert(0 == sizeof(b[0]));
    assert(0 == sizeof(b));
    TU_ASSERT_AND_RETURN(d1, TU_COUNTOF(b));
}
