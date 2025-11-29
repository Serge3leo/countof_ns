size_t TU_UNIT(void) {
    size_t d1 = 1;
    int b[d1][0];
    tu_static_assert(0 == sizeof(b[0]));
    TU_ASSERT_AND_RETURN(0, sizeof(b));
}
