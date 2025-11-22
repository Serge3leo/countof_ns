// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: CC-BY-SA-4.0
// SPDX-FileCopyrightText: 2025 alx - recommends codidact (http://www.alejandro-colomar.es/)

#ifndef COUNTOF_H_5700
#define COUNTOF_H_5700

// https://stackoverflow.com/a/57537491/8585880
//
// Update: Allow the macro to be used at file scope:
//
// Unfortunately, the ({}) gcc extension cannot be used at file scope. To be
// able to use the macro at file scope, the static assertion must be inside
// sizeof(struct {}). Then, multiply it by 0 to not affect the result. A cast
// to (int) might be good to simulate a function that returns (int)0 (in this
// case it is not necessary, but then it is reusable for other things).
//
// Additionally, the definition of ARRAY_BYTES() can be simplified a bit.

#include <assert.h>
#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define is_same_type(a, b)    __builtin_types_compatible_p(a, b)
#define is_same_typeof(a, b)  is_same_type(typeof(a), typeof(b))
#define decay(a)              (&*(a))
#define is_array(arr)         (!is_same_typeof(arr, decay(arr)))
#define must_be(e)                                                      \
(                                                                       \
        0 * (int)sizeof(                                                \
                struct {                                                \
                        static_assert(e);                               \
                        char ISO_C_forbids_a_struct_with_no_members__;  \
                }                                                       \
        )                                                               \
)
#define must_be_array(arr)      must_be(is_array(arr))

#define COUNTOF(arr)            (sizeof(arr) / sizeof((arr)[0]) + must_be_array(arr))
#define ARRAY_BYTES(arr)        (sizeof(arr) + must_be_array(arr))

// TODO: May be, for wide comparisons:
#if __STDC_VERSION__ < 202311L && _COUNTOF_wide_comparisons
    #ifdef __cplusplus
        #include <type_traits>
        #define _COUNTOF_typeof(t)  std::remove_reference_t<decltype(t)>
    #else
        #define _COUNTOF_typeof(t)  __typeof__(t)
    #endif

    #undef is_same_typeof
    #define is_same_typeof(a, b)  is_same_type(_COUNTOF_typeof(a),  \
                                               _COUNTOF_typeof(b))
#endif

#endif  // COUNTOF_H_5700
