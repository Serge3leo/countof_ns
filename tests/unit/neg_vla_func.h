// vim:set sw=4 ts=8 et fileencoding=utf8:
#if !__cplusplus
    static size_t nvf_foo(size_t n, int a[static n]) {
        return TU_COUNTOF(a);
    }
#else
    static size_t nvf_foo(size_t n, int a[]) {
        // TODO: duplicate, SKIP or?
        return TU_COUNTOF(a);
    }
#endif
static size_t TU_UNIT(void) {
    size_t n = 7;
    int a[n];
    return nvf_foo(n, a);
}
