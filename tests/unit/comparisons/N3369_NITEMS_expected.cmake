# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (NOT ((HAVE_BUILTIN_TYPES_COMPATIBLE_P OR
          HAVE_HIDDEN_BUILTIN_TYPES_COMPATIBLE_P) AND HAVE_TYPEOF))
    return ()
endif ()

function (tu_n3369_nitems_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES "_vla.*_[n0]0")
            list(APPEND pos_base ${b}.run_fail)
        elseif (b MATCHES "_[n0]0")
            list(APPEND pos_base ${b}.build_fail)
        elseif (b MATCHES "_vla_eval2d")
            list(APPEND pos_base ${b}.run_eval_1)
        else ()
            list(APPEND pos_base ${b})
        endif ()
    endforeach ()
    set(neg_base "")
    foreach (b IN ITEMS ${neg_pos})
        list(APPEND neg_base ${b}.build_fail)
    endforeach ()
    if (CMAKE_C_COMPILER_ID STREQUAL NVHPC AND HAVE_BROKEN_VLA)
        string(REGEX REPLACE "pos_vla_0n(;|$)" "pos_vla_0n.compiler_bug\\1"
                             pos_base "${pos_base}")
        string(REGEX REPLACE "pos_vla_eval(;|$)"
                             "pos_vla_eval.compiler_bug\\1"
                             pos_base "${pos_base}")
        string(REGEX REPLACE "pos_vla_eval2d\\.run_eval_1"
                             "pos_vla_eval2d.compiler_bug"
                             pos_base "${pos_base}")
    endif ()
    set(build_div0_Clang pos_alone_n0 pos_struct_n0 pos_zla_n0
                         pos_zla_00 pos_zla_alone_00 pos_zla_struct_00
                         neg_alone_ptr neg_zla_ptr)
    set(run_div0_Clang pos_vla_alone_00 pos_vla_alone_n0
                       pos_vla_struct_00 pos_vla_struct_n0
                       pos_vla_zla_00 pos_vla_zla_n0)
    set(run_fpe_Clang pos_vla_00 pos_vla_n0
                      pos_vla_alone_00 pos_vla_alone_n0
                      pos_vla_struct_00 pos_vla_struct_n0
                      pos_vla_zla_00 pos_vla_zla_n0)
    set(build_div0_GNU pos_alone_n0 pos_struct_n0
                       pos_zla_00 pos_zla_alone_00 pos_zla_struct_00 pos_zla_n0
                       neg_alone_ptr neg_zla_ptr)
    set(run_div0_GNU pos_vla_alone_00 pos_vla_alone_n0
                     pos_vla_struct_00 pos_vla_struct_n0
                     pos_vla_zla_00 pos_vla_zla_n0)
    set(run_fpe_GNU pos_vla_00 pos_vla_n0
                    pos_vla_alone_00 pos_vla_alone_n0
                    pos_vla_struct_00 pos_vla_struct_n0
                    pos_vla_zla_00 pos_vla_zla_n0)
    set(build_div0_IntelLLVM pos_alone_n0 pos_struct_n0 pos_zla_n0
                             pos_zla_00 pos_zla_alone_00 pos_zla_struct_00
                             neg_alone_ptr neg_zla_ptr)
    set(run_div0_IntelLLVM pos_vla_alone_00 pos_vla_alone_n0
                           pos_vla_struct_00 pos_vla_struct_n0
                           pos_vla_zla_00 pos_vla_zla_n0)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        list(APPEND run_fpe_IntelLLVM
                        pos_vla_00 pos_vla_n0
                        pos_vla_alone_00 pos_vla_alone_n0
                        pos_vla_struct_00 pos_vla_struct_n0
                        pos_vla_zla_00 pos_vla_zla_n0)
    endif ()
    set(run_fpe_LCC pos_vla_00 pos_vla_n0
                    pos_vla_struct_00 pos_vla_struct_n0
                    pos_vla_zla_00 pos_vla_zla_n0)
    set(build_div0_NVHPC pos_struct_n0 pos_zla_00 pos_zla_n0
                         pos_zla_struct_00 neg_zla_ptr)
    set(run_div0_NVHPC pos_vla_struct_00 pos_vla_struct_n0
                       pos_vla_zla_00 pos_vla_zla_n0)
    set(run_fpe_NVHPC pos_vla_00 pos_vla_0n pos_vla_n0)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        list(APPEND run_fpe_NVHPC pos_vla_struct_00 pos_vla_struct_n0
                                  pos_vla_zla_00 pos_vla_zla_n0)
    endif ()
    foreach (base IN ITEMS pos_base neg_base)
        foreach (b IN LISTS build_div0_${CMAKE_C_COMPILER_ID})
            string(REPLACE "${b}.build_fail" "${b}.build_fail.build_DIV0"
                   ${base} "${${base}}")
        endforeach ()
        foreach (b IN LISTS run_div0_${CMAKE_C_COMPILER_ID})
            string(REPLACE "${b}.run_fail" "${b}.run_fail.run_DIV0"
                   ${base} "${${base}}")
        endforeach ()
        foreach (b IN LISTS run_fpe_${CMAKE_C_COMPILER_ID})
            string(REPLACE "${b}.run_fail" "${b}.run_fail.run_FPE"
                   ${base} "${${base}}")
        endforeach ()
        foreach (b IN LISTS run_segv_${CMAKE_C_COMPILER_ID})
            string(REPLACE "${b}.run_fail" "${b}.run_fail.run_SEGV"
                   ${base} "${${base}}")
        endforeach ()
    endforeach ()
    set(${expected} "${pos_base};${neg_base}" PARENT_SCOPE)
endfunction ()

tu_n3369_nitems_expected(tu_n3369_nitems_available
                         "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_n3369_nitems_params FALSE
                          "N3369_NITEMS" "_comparisons/N3369_NITEMS.h"
                          tu_n3369_nitems_available)
list(APPEND tu_params_list tu_n3369_nitems_params)
