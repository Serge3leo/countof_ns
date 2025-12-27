// vim:set sw=4 ts=8 et fileencoding=utf8:
#define N_  (7)
#if !__cplusplus
    static size_t pvf_foo(size_t n, int (*a)[n]) {
        TU_ASSERT_AND_RETURN(N_, TU_COUNTOF(*a));
    }
    static size_t TU_UNIT(void) {
        size_t n = N_;
        int a[n];
        return pvf_foo(n, &a);
    }
#else
    #ifdef __has_include
        #if __cplusplus >= 202002L && __has_include(<span>)
            #include <span>
            #define HAVE_SPAN  (1)
        #endif
    #endif
    #if HAVE_SPAN
        static size_t pvf_foo(std::span<int> a) {
            TU_ASSERT_AND_RETURN(N_, TU_COUNTOF(a));
        }
    #else
        static size_t pvf_foo(size_t n, int *a_) {
            typedef int a_t[n];
            a_t &a = *(a_t *)a_;

            TU_ASSERT_AND_RETURN(N_, TU_COUNTOF(a));
        }
    #endif
    static size_t TU_UNIT(void) {
        size_t n = N_;
        int a[n];
        #if HAVE_SPAN
            return pvf_foo(std::span(a, n));
        #else
            return pvf_foo(n, a);
        #endif
    }
#endif
