# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

function (tu_ms_countof_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES "pos_struct_n0")
            list(APPEND pos_base ${b}_cxx)
        elseif (b MATCHES "_vla.*_[n0]0")
            list(APPEND pos_base ${b}.run_fail)
        elseif (b MATCHES "_n0")
            list(APPEND pos_base ${b}.build_fail ${b}_cxx)
        elseif (b MATCHES "_00")
            list(APPEND pos_base ${b}.build_fail ${b}_cxx.build_fail)
        elseif (b MATCHES "_0n")
            list(APPEND pos_base ${b} ${b}_cxx.build_fail)
        elseif (b MATCHES "_vla_eval2d")
            list(APPEND pos_base ${b}.run_eval_1)
        elseif (b MATCHES "_vla")
            list(APPEND pos_base ${b})
        else ()
            list(APPEND pos_base ${b} ${b}_cxx)
        endif ()
    endforeach ()
    set(neg_base "")
    foreach (b IN ITEMS ${neg_pos})
        if (b MATCHES "(__selftest$|_other$|_alone_ptr$)")
            list(APPEND neg_base ${b}.build_fail ${b}_cxx.build_fail)
        else ()
            list(APPEND neg_base ${b}.build_unexpected ${b}.run_fail
                                 ${b}_cxx.build_fail)
        endif ()
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
    if (CMAKE_C_COMPILER_ID STREQUAL Intel AND
        NOT CMAKE_BUILD_TYPE MATCHES "Debug")
            # Zero is result of zero-by-zero division on old Intel.
        string(REGEX REPLACE "(struct|zla)_00.run_fail"
                             "\\1_00.run_0_unexpected"
                             pos_base "${pos_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
        string(REGEX REPLACE "(neg_zla_ptr)\\.build_unexpected"
                             "\\1.build_fail.build_DIV0"
                             neg_base "${neg_base}")
        string(REGEX REPLACE "(neg_zla_ptr)\\.run_fail($|;)"
                             ""
                             neg_base "${neg_base}")
    endif ()
    set(build_div0_Clang pos_alone_n0 pos_zla_00 pos_zla_alone_00 pos_zla_n0
                         pos_zla_struct_00 neg_alone_ptr neg_zla_ptr)
    set(run_div0_Clang pos_vla_alone_00 pos_vla_alone_n0
                       pos_vla_struct_00 pos_vla_struct_n0
                       pos_vla_zla_00 pos_vla_zla_n0
                       neg_zla_ptr)
    set(run_fpe_Clang pos_vla_alone_00 pos_vla_alone_n0
                      pos_vla_00 pos_vla_n0
                      pos_vla_struct_00 pos_vla_struct_n0
                      pos_vla_zla_00 pos_vla_zla_n0
                      neg_vla_zla_ptr neg_zla_vla_ptr)
    set(build_div0_GNU pos_alone_n0 pos_zla_00 pos_zla_alone_00 pos_zla_n0
                       pos_zla_struct_00 neg_alone_ptr neg_zla_ptr)
    set(run_div0_GNU pos_vla_alone_00 pos_vla_alone_n0
                     pos_vla_struct_00 pos_vla_struct_n0
                     pos_vla_zla_00 pos_vla_zla_n0
                     pos_zla_alone_00 pos_zla_n0 pos_zla_struct_00
                     neg_zla_ptr)
    set(run_fpe_GNU pos_vla_00 pos_vla_n0
                    pos_vla_alone_00 pos_vla_alone_n0
                    pos_vla_struct_00 pos_vla_struct_n0
                    pos_vla_zla_00 pos_vla_zla_n0
                    neg_zla_ptr neg_vla_zla_ptr neg_zla_vla_ptr)
    set(build_div0_Intel pos_zla_00 pos_zla_n0 pos_zla_struct_00)
    set(run_div0_Intel pos_vla_struct_n0 pos_vla_zla_n0 neg_zla_ptr)
    set(run_fpe_Intel pos_vla_00
                      neg_vla_zla_ptr neg_zla_ptr neg_zla_vla_ptr)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        list(APPEND run_fpe_Intel pos_vla_n0
                                  pos_vla_zla_00 pos_vla_zla_n0)
    endif ()
    set(build_div0_IntelLLVM pos_alone_n0 pos_zla_00 pos_zla_alone_00 pos_zla_n0
                             pos_zla_struct_00 neg_alone_ptr)
    set(run_div0_IntelLLVM neg_zla_ptr
                           pos_vla_alone_00 pos_vla_alone_n0
                           pos_vla_struct_00 pos_vla_struct_n0
                           pos_vla_zla_00 pos_vla_zla_n0)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        list(APPEND run_fpe_IntelLLVM
                        pos_vla_00 pos_vla_n0
                        pos_vla_alone_00 pos_vla_alone_n0
                        pos_vla_struct_00 pos_vla_struct_n0
                        pos_vla_zla_00 pos_vla_zla_n0
                        neg_vla_zla_ptr
                        neg_zla_vla_ptr)
    else ()
        set(run_segv_IntelLLVM neg_vla_zla_ptr neg_zla_ptr neg_zla_vla_ptr)
    endif ()
    set(build_div0_LCC pos_zla_00 pos_zla_n0 pos_zla_struct_00)
    set(run_fpe_LCC pos_vla_00 pos_vla_n0 pos_vla_struct_00
                    pos_vla_struct_n0 pos_vla_zla_00 pos_vla_zla_n0
                    neg_vla_zla_ptr neg_zla_ptr neg_zla_vla_ptr)
    set(build_div0_NVHPC pos_zla_00 pos_zla_n0 pos_zla_struct_00)
    set(div0_NVHPC pos_vla_struct_00 pos_vla_struct_n0
                   pos_vla_zla_00 pos_vla_zla_n0
                   neg_zla_ptr)
    set(run_fpe_NVHPC pos_vla_00 pos_vla_0n pos_vla_n0)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        list(APPEND build_div0_NVHPC pos_vla_struct_00 pos_vla_struct_n0
                                     pos_vla_zla_00 pos_vla_zla_n0
                                     neg_zla_ptr)
        list(APPEND run_fpe_NVHPC pos_vla_struct_00 pos_vla_struct_n0
                                  pos_vla_zla_00 pos_vla_zla_n0
                                  neg_vla_zla_ptr
                                  neg_zla_ptr
                                  neg_zla_vla_ptr)
    endif ()
    set(run_fpe_SunPro pos_vla_00 pos_vla_n0 neg_vla_zla_ptr neg_zla_vla_ptr)
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

tu_ms_countof_expected(tu_ms_countof_available
                           "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_ms_countof_params FALSE
                          "ms_countof" "_comparisons/ms_countof.h"
                          tu_ms_countof_available)
list(APPEND tu_params_list tu_ms_countof_params)
