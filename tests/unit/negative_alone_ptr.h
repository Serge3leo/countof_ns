size_t TU_UNIT(void) {
    struct {
	int flex[];
    } *p;
    TU_STATIC_ASSERT_AND_RETURN(1, TU_COUNTOF(p));
}
