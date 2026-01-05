// vim:set sw=4 ts=8 et fileencoding=utf8:
size_t TU_UNIT(void) {
    size_t d7 = 7;
    struct { } a[d7];
    #if !__cplusplus
        tu_static_assert(0 == sizeof(a[0]));
    #endif
    tu_assert(d7*sizeof(a[0]) == sizeof(a));
    TU_ASSERT_AND_RETURN(d7, TU_COUNTOF(a));
}
