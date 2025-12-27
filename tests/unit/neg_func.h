// vim:set sw=4 ts=8 et fileencoding=utf8:
#define N  (7)
#if !__cplusplus
    static size_t nf_foo(int a[static N]) {
        return TU_COUNTOF(a);
    }
#else
    static size_t nf_foo(int a[N]) {
        return TU_COUNTOF(a);
    }
#endif
static size_t TU_UNIT(void) {
    int a[N];
    return nf_foo(a);
}
