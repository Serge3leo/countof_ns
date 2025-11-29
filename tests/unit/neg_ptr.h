static size_t TU_UNIT(void) {
    printf("Debug %s:%d:%s\n", __FILE__, __LINE__, __func__);  // TODO
    int *p;
    printf("Debug %s:%d:%s TU_COUNTOF(p)=%zu \n", __FILE__, __LINE__, __func__,
	   TU_COUNTOF(p));  // TODO
    return TU_COUNTOF(p);
}
