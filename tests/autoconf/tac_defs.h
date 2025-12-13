// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#ifndef TAC_DEFS_H_5320
#define TAC_DEFS_H_5320

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if __STDC_VERSION__ >= 202311L || __cplusplus
    #define tac_static_assert(...)  static_assert(__VA_ARGS__)
#elif __STDC_VERSION__ >= 201112L
    #define tac_static_assert(...)  _Static_assert(__VA_ARGS__)
#else
    #define tac_static_assert(e, s)  tac_assert((e) && (s))
#endif
#define tac_exit_failure()  ( \
                        (void)printf("Fail by tac_exit_failure() at %s:%d\n", \
                                      __FILE__, __LINE__), \
                        (void)exit(EXIT_FAILURE))
#define tac_assert(e)  ((e) ? (void)0 \
                            : ((void)printf("Assertion failed: %s at %s:%d\n", \
                                            (#e), __FILE__, __LINE__), \
                               (void)tac_exit_failure() \
                              ) \
                       )

#define _TAC_FUNC_DECL(name)  extern void _tac_check_func_##name(void)
#if !TAC_DONT_MAIN
    #define _TAC_MAIN(name)  int main(void) { \
                                _tac_check_func_##name(); \
                                printf("Ok\n"); \
                             }
#else
    #define _TAC_MAIN(name)  void _tac_check_func_stub_##name(void) {}
#endif
#define _TAC_PROTO(name)  void _tac_check_func_##name(void)
#define TAC_CHECK_FUNC(name)  _TAC_FUNC_DECL(name); _TAC_MAIN(name) _TAC_PROTO(name)

#endif  // TAC_DEFS_H_5320
