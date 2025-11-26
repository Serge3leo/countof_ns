size_t TU_UNIT(void) {
    struct {
	int flex[];
    } a[1];
    TU_STATIC_ASSERT_AND_RETURN(1, TU_COUNTOF(a));
}
