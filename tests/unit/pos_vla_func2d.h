// vim:set sw=4 ts=8 et fileencoding=utf8:
#define M_  (7)
#define N_  (1917)
#if !__cplusplus
    static size_t pvf2_foo(size_t m, size_t n, int (*a)[m][n]) {
        assert(M_ == m);
        assert(N_ == n);
        assert(N_ == TU_COUNTOF((*a)[0]));
        TU_ASSERT_AND_RETURN(M_, TU_COUNTOF(*a));
    }
    static size_t TU_UNIT(void) {
        size_t m = M_;
        size_t n = N_;
        int a[m][n];
        return pvf2_foo(m, n, &a);
    }
#else
    #ifdef __has_include
        #if __cplusplus >= 202302L && __has_include(<mdspan>)
            #include <mdspan>
            #define HAVE_MDSPAN  (1)
        #endif
    #endif
    #if HAVE_MDSPAN
        static size_t pvf2_foo(std::mdspan<int, std::dims<2>> a_) {
            assert(M_ == a_.extent(0));
            assert(N_ == a_.extent(1));

            typedef decltype(a_)::element_type a_t[a_.extent(0)][a_.extent(1)];
            a_t &a = *(a_t *)(a_.data_handle());

            assert(N_ == TU_COUNTOF(a[0]));
            TU_ASSERT_AND_RETURN(M_, TU_COUNTOF(a));
        }
    #else
        static size_t pvf2_foo(size_t m, size_t n, int *a_) {
            assert(M_ == m);
            assert(N_ == n);

            typedef int a_t[m][n];
            a_t &a = *(a_t *)a_;

            assert(N_ == TU_COUNTOF(a[0]));
            TU_ASSERT_AND_RETURN(M_, TU_COUNTOF(a));
        }
    #endif
    static size_t TU_UNIT(void) {
        size_t m = M_;
        size_t n = N_;
        int a[m][n];
        #if HAVE_MDSPAN
            return pvf2_foo(std::mdspan(a[0], m, n));
        #else
            return pvf2_foo(m, n, a[0]);
        #endif
    }
#endif
