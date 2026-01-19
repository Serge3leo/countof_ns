


#if __STDC_VERSION__ >= 202311L
    #define _countof_ns_assert  static_assert
#else
    #define _countof_ns_assert  _Static_assert
#endif
    // Проверка, что `a` это массив содержащий `_countof_ns_unsafe(a)`
    // элементов (для расширений VLA, в случае 0 == sizeof(*(a)), число
    // элементов не ограничивается)
#define _countof_ns_must_array(a)  \
            (0*sizeof(struct { int _countof_ns; _countof_ns_assert( \
                __builtin_types_compatible_p( \
                    _countof_ns_typeof(a) *, \
                    _countof_ns_typeof(*(a))(*)[_countof_ns_unsafe(a)] \
                ), "countof_ns: " #a " : Must be array"); }))
