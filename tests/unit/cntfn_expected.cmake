# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

function (tu_cntfn_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES "pos_vla_func$" AND
            NOT CMAKE_CXX_COMPILER_ID STREQUAL Intel AND
            #NOT CMAKE_CXX_COMPILER_ID STREQUAL LCC AND
            NOT CMAKE_CXX_COMPILER_ID STREQUAL SunPro)  # TODO HAVE_SPAN
            set(ints ${b}.gen.run_c2y ${b}.c11 ${b}.bltn
                     ${b}_cxx.tmpl ${b}_cxx.bltn)
        elseif (b MATCHES "pos_eval")
            if (HAVE___STDC_NO_VLA__)  # TODO: MSVC
                set(ints ${b}.gen ${b}_cxx.tmpl.run_eval_1)
            else ()
                set(ints ${b}.gen ${b}.c11 ${b}.bltn ${b}_cxx.tmpl.run_eval_1)
                if (CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
                    list(APPEND ints ${b}_cxx.bltn)
                else ()
                    list(APPEND ints ${b}_cxx.bltn.run_eval_2)
                endif ()
            endif ()
        elseif (b MATCHES "pos_vla_eval$")
            set(ints ${b}.gen.run_c2y.run_eval_1)
            if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}.c11.run_eval_1 ${b}.bltn.run_eval_1)
            else ()
                list(APPEND ints ${b}.c11 ${b}.bltn)
            endif ()
            list(APPEND ints ${b}_cxx.tmpl.build_fail)
            if (CMAKE_CXX_COMPILER_ID STREQUAL Clang OR
                CMAKE_CXX_COMPILER_ID STREQUAL LCC)
                list(APPEND ints ${b}_cxx.bltn.run_eval_1)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU OR
                    CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}_cxx.bltn.run_eval_-1)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL Intel OR
                    CMAKE_CXX_COMPILER_ID STREQUAL IntelLLVM)
                if (CMAKE_BUILD_TYPE MATCHES "Debug")
                    list(APPEND ints ${b}_cxx.bltn.run_eval_1)
                else ()
                    list(APPEND ints ${b}_cxx.bltn.run_eval_2)
                endif ()
            else ()
                list(APPEND ints ${b}_cxx.bltn)
            endif ()
        elseif (b MATCHES "pos_vla_vla_eval")
            set(ints ${b}.gen.run_c2y.run_eval_3)
            if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}.c11.run_eval_5 ${b}.bltn.run_eval_5)
            elseif (CMAKE_C_COMPILER_ID STREQUAL Intel OR
                    CMAKE_C_COMPILER_ID STREQUAL LCC OR
                    CMAKE_C_COMPILER_ID STREQUAL NVHPC)
                list(APPEND ints ${b}.c11.run_eval_2 ${b}.bltn.run_eval_2)
            else ()
                list(APPEND ints ${b}.c11 ${b}.bltn)
            endif ()
            list(APPEND ints ${b}_cxx.tmpl.build_fail)
            if (CMAKE_CXX_COMPILER_ID STREQUAL Clang OR
                CMAKE_CXX_COMPILER_ID STREQUAL LCC)
                list(APPEND ints ${b}_cxx.bltn.run_eval_3)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU OR
                    CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}_cxx.bltn.run_eval_-1)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL Intel OR
                    CMAKE_CXX_COMPILER_ID STREQUAL IntelLLVM)
                if (CMAKE_BUILD_TYPE MATCHES "Debug")
                    list(APPEND ints ${b}_cxx.bltn.run_eval_3)
                else ()
                    list(APPEND ints ${b}_cxx.bltn.run_eval_4)
                endif ()
            else ()
                list(APPEND ints ${b}_cxx.bltn)
            endif ()
        elseif (b MATCHES "pos_fix_vla_eval")
            set(ints ${b}.gen.run_c2y.run_eval_4)
            if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}.c11.run_eval_4 ${b}.bltn.run_eval_4)
            elseif (CMAKE_C_COMPILER_ID STREQUAL Intel OR
                    CMAKE_C_COMPILER_ID STREQUAL LCC OR
                    CMAKE_C_COMPILER_ID STREQUAL NVHPC)
                list(APPEND ints ${b}.c11.run_eval_3 ${b}.bltn.run_eval_3)
            else ()
                list(APPEND ints ${b}.c11.run_eval_1 ${b}.bltn.run_eval_1)
            endif ()
            list(APPEND ints ${b}_cxx.tmpl.build_fail)
            if (CMAKE_CXX_COMPILER_ID STREQUAL Clang OR
                CMAKE_CXX_COMPILER_ID STREQUAL LCC)
                list(APPEND ints ${b}_cxx.bltn.run_eval_4)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU OR
                    CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}_cxx.bltn)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL Intel OR
                    CMAKE_CXX_COMPILER_ID STREQUAL IntelLLVM)
                if (CMAKE_BUILD_TYPE MATCHES "Debug")
                    list(APPEND ints ${b}_cxx.bltn.run_eval_3)
                else ()
                    list(APPEND ints ${b}_cxx.bltn.run_eval_4)
                endif ()
            else ()
                list(APPEND ints ${b}_cxx.bltn)
            endif ()
        elseif (b MATCHES "vla")
            if (CMAKE_C_COMPILER_ID STREQUAL Clang OR
                CMAKE_C_COMPILER_ID STREQUAL IntelLLVM)
                set(ints ${b}.gen.run_c2y.disable ${b}.c11 ${b}.bltn
                         ${b}_cxx.tmpl.build_fail ${b}_cxx.bltn)
            else ()
                set(ints ${b}.gen.run_c2y ${b}.c11 ${b}.bltn
                         ${b}_cxx.tmpl.build_fail ${b}_cxx.bltn)
            endif ()
        elseif (NOT HAVE___STDC_NO_VLA__)
            set(ints ${b}.gen ${b}.c11 ${b}.bltn
                     ${b}_cxx.tmpl ${b}_cxx.bltn)
        else ()
            # TODO: MSVC
            set(ints ${b}.gen ${b}_cxx.tmpl)
        endif ()
        foreach (i IN ITEMS ${ints})
            if (i MATCHES "struct_n0_cxx")
                list(APPEND pos_base ${i})
            elseif (i MATCHES "_vla.*_n0" AND NOT i MATCHES "\\.build_fail")
                list(APPEND pos_base ${i}.run_0_unexpected)
            elseif (i MATCHES "_n0" AND NOT i MATCHES "_cxx" AND
                    NOT i MATCHES "\\.build_fail")
                list(APPEND pos_base ${i}.build_fail)
            else ()
                list(APPEND pos_base ${i})
            endif ()
        endforeach ()
    endforeach ()

    set(neg_base "")
    foreach (b IN ITEMS ${neg_pos})
        if (NOT HAVE___STDC_NO_VLA__)
            set(ints ${b}.gen.build_fail ${b}.c11.build_fail
                     ${b}.bltn.build_fail
                     ${b}_cxx.tmpl.build_fail ${b}_cxx.bltn.build_fail)
        else ()
            # TODO: MSVC
            set(ints ${b}.gen.build_fail ${b}_cxx.tmpl.build_fail)
        endif ()
        if (NOT NO_ERROR_ON_SIZEOF_POINTER_SUBTRACTION)
            list(APPEND neg_base ${ints})
        else ()
            foreach (i IN ITEMS ${ints})
                if (i MATCHES "\\.c11" AND
                    NOT i MATCHES "__selftest|_other")
                    list(APPEND neg_base ${b}.c11.build_unexpected
                                         ${b}.c11.run_fail)
                else ()
                    list(APPEND neg_base ${i})
                endif ()
            endforeach ()
        endif ()
    endforeach ()

    # Check defaults by _COUNTOF_NS_VLA_UNSUPPORTED/_COUNTOF_NS_USE_GENERIC/...

    if (MSVC)
        list(PREPEND pos_base "cntfn_dflt_gen"
                              "cntfn_dflt_tmpl_cxx")
    elseif (CMAKE_C_COMPILER_ID STREQUAL SunPro)
        if (TAC_POSITIVE_WERROR)
            list(PREPEND pos_base "cntfn_dflt_gen.gen"
                                  "cntfn_dflt_bltn_cxx")
        else ()
            list(PREPEND pos_base "cntfn_dflt_gen"
                                  "cntfn_dflt_bltn_cxx")
        endif ()
    elseif (CMAKE_C_COMPILER_ID STREQUAL PellesC)  # TODO: Not cmake module
        list(PREPEND pos_base "cntfn_dflt_c11")
    else ()
        list(PREPEND pos_base "cntfn_dflt_bltn"
                              "cntfn_dflt_bltn_cxx")
    endif()

    # Check internal or example user defined _countof_ns_ptr_compatible_type()

    file(GLOB pct_hdr RELATIVE "${CMAKE_CURRENT_LIST_DIR}"
         "${CMAKE_CURRENT_LIST_DIR}/cntfn_*_pct.h")

    foreach (cc IN ITEMS ${pct_hdr})
        get_filename_component(c ${cc} NAME_WLE)
        if ("${c}" MATCHES "_pos_")
            list(PREPEND pos_base "${c}.bltn")
        else ()
            list(PREPEND neg_base "${c}.bltn.build_fail")
        endif ()
    endforeach ()

    # Compilers bug and other features

    if (CMAKE_CXX_COMPILER_ID STREQUAL GNU AND
        CMAKE_CXX_COMPILER_VERSION MATCHES "^1[1-6]\\.")  # TODO: bug report
        string(REGEX REPLACE "(pos_vla_(alone|zla)_[0n]0_cxx(\\.bltn|\\.tmpl))"
                             "\\1.compiler_bug.disable"
                             pos_base "${pos_base}")
    endif ()
    if ((CMAKE_CXX_COMPILER_ID STREQUAL Intel AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^2021\\.") OR
        (CMAKE_CXX_COMPILER_ID STREQUAL LCC AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^1\\.2[79]\\.") OR
        (CMAKE_CXX_COMPILER_ID STREQUAL NVHPC AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^25\\."))
        string(REGEX REPLACE "(pos_vla_cv_cxx.bltn)" "\\1.compiler_bug"
                             pos_base "${pos_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL Intel OR
        CMAKE_C_COMPILER_ID STREQUAL LCC OR
        CMAKE_C_COMPILER_ID STREQUAL NVHPC)
            # TODO I don't understand. Why doesn't `_Generic()` reject a VLA
            # array?  Is this a peculiarity of the C language extensions only
            # for NVHPC (pgcc) and the classic Intel (icc)? Or is it the result
            # of optimizations?
        string(REGEX REPLACE "(pos_vla_(struct|zla)_00\\.gen)\\.run_c2y"
                             "\\1"
                             pos_base "${pos_base}")
        string(REGEX REPLACE "(pos_vla_(struct|zla)_n0\\.gen)\\.run_c2y"
                             "\\1.run_fail"
                             pos_base "${pos_base}")
            # HAVE_BROKEN_BUILTIN_TYPES_COMPATIBLE_P
        string(REGEX REPLACE "(cntfn_neg_size_pct.bltn)\\.build_fail"
                             "\\1.build_unexpected"
                             neg_base "${neg_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
            # TODO I don't understand. Why?
        string(REGEX REPLACE "(neg_alone_ptr\\.c11)\\.build_unexpected"
                             "\\1.build_fail"
                             neg_base "${neg_base}")
        string(REGEX REPLACE "(pos_vla_func(|2d)_cxx\\.bltn)"
                             "\\1.compiler_bug"
                             pos_base "${pos_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL NVHPC AND HAVE_BROKEN_VLA)
        string(REGEX REPLACE
            "(pos_[^.]*vla_eval[^_.]*[_.][^_;]*)(\\.(run_eval_[-0-9]+|build_fail))*(;|$)"
            "\\1.compiler_bug\\4"
            pos_base "${pos_base}")
    endif ()

    set(${expected} "${pos_base};${neg_base}" PARENT_SCOPE)
endfunction ()

tu_cntfn_expected(tu_cntfn_available "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_cntfn_params TRUE "countof_ns" "countof_ns.h"
                         tu_cntfn_available)
list(APPEND tu_params_list tu_cntfn_params)
