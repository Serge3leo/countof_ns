// vim:set sw=4 ts=8 et fileencoding=utf8:
#define N  (7)
#if !__cplusplus
    static size_t pf_foo(int (*a)[N]) {
        TU_STATIC_ASSERT_AND_RETURN(N, TU_COUNTOF(*a));
    }
    static size_t TU_UNIT(void) {
        int a[N];
        return pf_foo(&a);
    }
#else
    static size_t pf_foo(int (&a)[N]) {
        TU_ASSERT_AND_RETURN(N, TU_COUNTOF(a));
    }
    static size_t TU_UNIT(void) {
        int a[N];
        return pf_foo(a);
    }
#endif
