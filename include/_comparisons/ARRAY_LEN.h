// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: CC-BY-SA-4.0
// SPDX-FileCopyrightText: 2023 James Z.M. Gao (https://stackoverflow.com/users/7704140/james-z-m-gao)

#ifndef ARRAY_LEN_H_4790
#define ARRAY_LEN_H_4790

// TODO: May be, for wide comparisons:
#if !__cplusplus && _ARRAY_LEN_wide_comparisons
    #ifdef __STDC_VERSION__ >= 202311L
        #include <type_traits>
        #define _ARRAY_LEN_typeof(t)  typeof(t)
    #else
        #define _ARRAY_LEN_typeof(t)  __typeof__(t)
    #endif
#endif

// https://stackoverflow.com/a/77001872/8585880
// https://stackoverflow.com/a/77028426/8585880
//
// with typeof in c and template matching in c++:

#ifndef __cplusplus
   /* C version */
#  define ARRAY_LEN_UNSAFE(X) (sizeof(X)/sizeof(*(X)))
#  define ARRAY_LEN(X) (ARRAY_LEN_UNSAFE(X) + 0 * sizeof((typeof(*X)(*[1])[ARRAY_LEN_UNSAFE(X)]){0} - (typeof(X)**)0))
#else
   /* C++ version */
   template <unsigned int N> class __array_len_aux    { public: template <typename T, unsigned int M> static const char (&match_only_array(T(&)[M]))[M]; };
   template <>               class __array_len_aux<0> { public: template <typename T>                 static const char (&match_only_array(T(&)))[0]; };
#  define ARRAY_LEN(X) sizeof(__array_len_aux<sizeof(X)>::match_only_array(X))
#endif

#endif  // ARRAY_LEN_H_4790
