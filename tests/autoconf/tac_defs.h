// vim:set sw=4 ts=8 et fileencoding=utf8:
// SPDX-License-Identifier: BSD-2-Clause
// SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

#ifndef TAC_DEFS_H_5320
#define TAC_DEFS_H_5320

#include <assert.h>
#include <stdio.h>

#if __STDC_VERSION__ >= 202311L || __cplusplus
    #define tac_static_assert(...)  static_assert(__VA_ARGS__)
#elif __STDC_VERSION__ >= 201112L
    #define tac_static_assert(...)  _Static_assert(__VA_ARGS__)
#else
    #define tac_static_assert(e, s)  assert((e) && (s))
#endif
#if __SUNPRO_C || __NVCOMPILER  || TAC_PEDANTIC \
    || (defined(__clang_major__) && __clang_major__ < 16)
    #define tac_crazy_static_assert(e, s)  assert((e) && (s))
#else
    #define tac_crazy_static_assert(e, s)  tac_static_assert((e), s)
#endif

#define _TAC_FUNC_DECL(name)  extern void _tac_check_func_##name(void)
#if !TAC_DONT_MAIN
    #define _TAC_MAIN(name)  int main(void) { _tac_check_func_##name(); }
#else
    #define _TAC_MAIN(name)  void _tac_check_func_stub_##name(void) {}
#endif
#define _TAC_PROTO(name)  void _tac_check_func_##name(void)
#define TAC_CHECK_FUNC(name)  _TAC_FUNC_DECL(name); _TAC_MAIN(name) _TAC_PROTO(name)

#endif  // TAC_DEFS_H_5320
