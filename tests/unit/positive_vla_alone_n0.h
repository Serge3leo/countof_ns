size_t TU_UNIT(void) {
    size_t d1 = 1;
    struct {
	int flex[];
    } a[d1];
    TU_ASSERT_AND_RETURN(d1, TU_COUNTOF(a));
}
