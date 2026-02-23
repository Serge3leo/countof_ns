# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

function (tu_cntfn_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES "pos_func$" AND HAVE_BROKEN_FUNC_PARAMETER)
            set(ints ${b}.kr.run_fail.compiler_bug
                     ${b}.gen.run_fail.compiler_bug
                     ${b}.c11.run_fail.compiler_bug
                     ${b}.bltn.run_fail.compiler_bug
                     ${b}_cxx.tmpl ${b}_cxx.bltn)
        elseif (b MATCHES "pos_vla_func$" AND HAVE_BROKEN_FUNC_PARAMETER)
            set(ints ${b}.kr.build_fail.compiler_bug
                     ${b}.gen.build_fail.compiler_bug
                     ${b}.c11.build_fail.compiler_bug
                     ${b}.bltn.build_fail.compiler_bug
                     ${b}_cxx.tmpl ${b}_cxx.bltn)
        elseif (b MATCHES "pos_vla_func$" AND
                NOT CMAKE_CXX_COMPILER_ID STREQUAL Intel AND
                #NOT CMAKE_CXX_COMPILER_ID STREQUAL LCC AND
                NOT CMAKE_CXX_COMPILER_ID STREQUAL SunPro)  # TODO HAVE_SPAN
            set(ints ${b}.kr ${b}.gen.run_c2y ${b}.c11 ${b}.bltn
                     ${b}_cxx.tmpl ${b}_cxx.bltn)
        elseif (b MATCHES "pos_eval$")
            if (HAVE___STDC_NO_VLA__)  # TODO: MSVC
                if (CMAKE_C_COMPILER_VERSION VERSION_LESS "19.39")
                    set(ints ${b}.kr ${b}_cxx.tmpl)
                else ()
                    set(ints ${b}.kr ${b}.gen ${b}_cxx.tmpl)
                endif ()
            else ()
                set(ints ${b}.kr ${b}.gen ${b}.c11 ${b}.bltn
                         ${b}_cxx.tmpl ${b}_cxx.bltn)
            endif ()
        elseif (b MATCHES "pos_vla_eval$")
            if (HAVE_BROKEN_SIZEOF)
                set(ints ${b}.kr.run_eval_-1 ${b}.gen.run_c2y.run_eval_1
                         ${b}.c11.run_eval_-1 ${b}.bltn.run_eval_-1)
            else ()
                set(ints ${b}.kr ${b}.gen.run_c2y.run_eval_1)
                if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
                    list(APPEND ints ${b}.c11.run_eval_1 ${b}.bltn.run_eval_1)
                else ()
                    list(APPEND ints ${b}.c11 ${b}.bltn)
                endif ()
            endif ()
            list(APPEND ints ${b}_cxx.tmpl.disable)
            if (CMAKE_CXX_COMPILER_ID STREQUAL GNU OR
                    CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}_cxx.bltn.run_eval_-1)
            else ()
                list(APPEND ints ${b}_cxx.bltn)
            endif ()
        elseif (b MATCHES "pos_vla_vla_eval$")
            if (HAVE_BROKEN_SIZEOF)
                set(ints ${b}.kr.run_eval_-1 ${b}.gen.run_c2y.run_eval_3
                         ${b}.c11.run_eval_-1 ${b}.bltn.run_eval_-1)
            else ()
                if(CMAKE_C_COMPILER_ID STREQUAL Intel OR
                   CMAKE_C_COMPILER_ID STREQUAL LCC OR
                   CMAKE_C_COMPILER_ID STREQUAL SunPro)
                    set(ints ${b}.kr.run_eval_2)
                else ()
                    set(ints ${b}.kr.run_eval_1)
                endif ()
                list(APPEND ints ${b}.gen.run_c2y.run_eval_3)
                if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
                    list(APPEND ints ${b}.c11.run_eval_5 ${b}.bltn.run_eval_5)
                elseif (CMAKE_C_COMPILER_ID STREQUAL Intel OR
                        CMAKE_C_COMPILER_ID STREQUAL LCC)
                    list(APPEND ints ${b}.c11.run_eval_2 ${b}.bltn.run_eval_2)
                else ()
                    list(APPEND ints ${b}.c11.run_eval_1 ${b}.bltn.run_eval_1)
                endif ()
            endif ()
            list(APPEND ints ${b}_cxx.tmpl.disable)
            if (CMAKE_CXX_COMPILER_ID STREQUAL Clang OR
                CMAKE_CXX_COMPILER_ID STREQUAL IntelLLVM)
                list(APPEND ints ${b}_cxx.bltn.run_eval_1)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU OR
                    CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}_cxx.bltn.run_eval_-1)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL Intel OR
                    CMAKE_CXX_COMPILER_ID STREQUAL LCC)
                list(APPEND ints ${b}_cxx.bltn.run_eval_2)
            else ()
                list(APPEND ints ${b}_cxx.bltn)
            endif ()
        elseif (b MATCHES "pos_fix_vla_eval$")
            if(CMAKE_C_COMPILER_ID STREQUAL Intel OR
               CMAKE_C_COMPILER_ID STREQUAL LCC)
                set(ints ${b}.kr.run_eval_3)
            elseif (NOT HAVE_BROKEN_SIZEOF)
                set(ints ${b}.kr.run_eval_2)
            else ()
                set(ints ${b}.kr.run_eval_0)
            endif ()
            list(APPEND ints ${b}.gen.run_c2y.run_eval_4)
            if (CMAKE_C_COMPILER_ID STREQUAL Intel OR
                CMAKE_C_COMPILER_ID STREQUAL LCC)
                list(APPEND ints ${b}.c11.run_eval_3 ${b}.bltn.run_eval_3)
            elseif (CMAKE_C_COMPILER_ID STREQUAL SunPro)
                list(APPEND ints ${b}.c11.run_eval_4 ${b}.bltn.run_eval_4)
            elseif (NOT HAVE_BROKEN_SIZEOF)
                list(APPEND ints ${b}.c11.run_eval_2 ${b}.bltn.run_eval_2)
            else ()
                list(APPEND ints ${b}.c11.run_eval_0 ${b}.bltn.run_eval_0)
            endif ()
            list(APPEND ints ${b}_cxx.tmpl.disable)
            if (CMAKE_CXX_COMPILER_ID STREQUAL Clang OR
                CMAKE_CXX_COMPILER_ID STREQUAL IntelLLVM)
                list(APPEND ints ${b}_cxx.bltn.run_eval_2)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL Intel OR
                    CMAKE_CXX_COMPILER_ID STREQUAL LCC)
                list(APPEND ints ${b}_cxx.bltn.run_eval_3)
            else ()
                list(APPEND ints ${b}_cxx.bltn)
            endif ()
        elseif (b MATCHES "pos_type$")
            continue ()
            # Disable, can't implement now
            if (MSVC)
                if (CMAKE_C_COMPILER_VERSION VERSION_LESS "19.39")
                    set(ints ${b}.kr.build_fail ${b}.gen.build_fail
                             ${b}_cxx.tmpl.build_fail)
                else ()
                    set(ints ${b}.kr.build_fail ${b}.gen
                             ${b}_cxx.tmpl.build_fail)
                endif ()
            else ()
                set(ints ${b}.kr.build_fail ${b}.gen ${b}.c11 ${b}.bltn
                         ${b}_cxx.tmpl.build_fail ${b}_cxx.bltn.build_fail)
            endif ()
        elseif (b MATCHES "vla")
            if (CMAKE_C_COMPILER_ID STREQUAL Clang OR
                CMAKE_C_COMPILER_ID STREQUAL IntelLLVM)
                set(ints ${b}.kr ${b}.gen.run_c2y.disable ${b}.c11 ${b}.bltn
                         ${b}_cxx.tmpl.build_fail ${b}_cxx.bltn)
            elseif (CMAKE_CXX_COMPILER_ID STREQUAL SunPro)
                set(ints ${b}.kr ${b}.gen.run_c2y.disable ${b}.c11 ${b}.bltn
                         ${b}_cxx.tmpl.disable ${b}_cxx.bltn)
            else ()
                set(ints ${b}.kr ${b}.gen.run_c2y ${b}.c11 ${b}.bltn
                         ${b}_cxx.tmpl.build_fail ${b}_cxx.bltn)
            endif ()
        elseif (NOT HAVE___STDC_NO_VLA__)
            set(ints ${b}.kr ${b}.gen ${b}.c11 ${b}.bltn
                     ${b}_cxx.tmpl ${b}_cxx.bltn)
        elseif (CMAKE_C_COMPILER_VERSION VERSION_LESS "19.39")
            set(ints ${b}.kr ${b}_cxx.tmpl)
        else()
            set(ints ${b}.kr ${b}.gen ${b}_cxx.tmpl)
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
            set(ints ${b}.kr.build_fail ${b}.gen.build_fail
                     ${b}.c11.build_fail ${b}.bltn.build_fail
                     ${b}_cxx.tmpl.build_fail ${b}_cxx.bltn.build_fail)
        elseif (CMAKE_C_COMPILER_VERSION VERSION_LESS "19.39")
            set(ints ${b}.kr.build_fail ${b}_cxx.tmpl.build_fail)
        else()
            set(ints ${b}.kr.build_fail ${b}.gen.build_fail
                     ${b}_cxx.tmpl.build_fail)
        endif ()
        foreach (i IN ITEMS ${ints})
            if (((NO_ERROR_ON_SIZEOF_POINTER_SUBTRACTION AND
                  i MATCHES "\\.c11") OR
                 i MATCHES "\\.kr") AND
                NOT i MATCHES "__selftest|_other")
                if (i MATCHES "\\.c11")
                    list(APPEND neg_base ${b}.c11.build_unexpected
                                         ${b}.c11.run_fail)
                elseif ((CMAKE_C_COMPILER_ID STREQUAL Clang OR
                         CMAKE_C_COMPILER_ID STREQUAL GNU OR
                         CMAKE_C_COMPILER_ID STREQUAL IntelLLVM) AND
                        i MATCHES
      "neg_((alone_|cv_|)ptr|func|vla_(func|(|zla_)ptr)|zla_(|vla_)ptr)\\.kr")
                    list(APPEND neg_base ${i})
                else ()
                    list(APPEND neg_base ${b}.kr.build_unexpected
                                         ${b}.kr.run_fail)
                endif ()
            else ()
                list(APPEND neg_base ${i})
            endif ()
        endforeach ()
    endforeach ()

    # Check defaults by _COUNTOF_NS_VLA_UNSUPPORTED/_COUNTOF_NS_USE_GENERIC/...

    if (MSVC)
        if (CMAKE_C_COMPILER_VERSION VERSION_LESS "19.39")
            list(PREPEND pos_base "cntfn_dflt_kr"
                                  "cntfn_dflt_tmpl_cxx")
        else ()
            list(PREPEND pos_base "cntfn_dflt_gen"
                                  "cntfn_dflt_tmpl_cxx")
        endif ()
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

    # Check internal or example user defined _countof_ns_must_compatible()

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
         CMAKE_CXX_COMPILER_VERSION MATCHES "^1\\.2[789]\\.") OR
        (CMAKE_CXX_COMPILER_ID STREQUAL NVHPC AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^25\\."))
        string(REGEX REPLACE "(pos_vla_cv_cxx.bltn)" "\\1.compiler_bug"
                             pos_base "${pos_base}")
    endif ()
    if (#CMAKE_C_COMPILER_ID STREQUAL Intel OR
        #CMAKE_C_COMPILER_ID STREQUAL LCC OR
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
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL Intel OR
        CMAKE_C_COMPILER_ID STREQUAL LCC OR
        CMAKE_C_COMPILER_ID STREQUAL NVHPC)
            # HAVE_BROKEN_BUILTIN_TYPES_COMPATIBLE_P
        string(REGEX REPLACE "(cntfn_neg_size_pct.bltn)\\.build_fail"
                             "\\1.build_unexpected"
                             neg_base "${neg_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL Intel OR
        CMAKE_C_COMPILER_ID STREQUAL LCC)
        string(REGEX REPLACE "(pos_type\\.(c11|gen|bltn))"
                             "\\1.compiler_bug"  # TODO
                             pos_base "${pos_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL OrangeC)
        string(REGEX REPLACE "pos_vla_vla_eval.gen.run_c2y.run_eval_3"
                             "pos_vla_vla_eval.gen.run_c2y.run_eval_1"
                             pos_base "${pos_base}")
        string(REGEX REPLACE "(neg_alone_ptr\\.(kr|c11))\\.build_unexpected"
                             "\\1.build_fail"
                             neg_base "${neg_base}")
        string(REGEX REPLACE "(neg_[^.]*\\.gen)\\.build_fail"
                             "\\1.build_unexpected.compiler_bug"
                             neg_base "${neg_base}")
        string(REGEX REPLACE "(neg_(|vla_)func\\.bltn)\\.build_fail"
                             "\\1.build_unexpected.compiler_bug"
                             neg_base "${neg_base}")
            # TODO: XXX: Why !HAVE_BROKEN_BUILTIN_TYPES_COMPATIBLE_P ?
        string(REGEX REPLACE "(cntfn_neg_size_pct.bltn)\\.build_fail"
                             "\\1.build_unexpected"
                             neg_base "${neg_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
        # TODO Move up? I don't understand. Why?
        string(REGEX REPLACE "(neg_alone_ptr\\.(kr|c11))\\.build_unexpected"
                             "\\1.build_fail"
                             neg_base "${neg_base}")
        string(REGEX REPLACE "(pos_vla_func(|2d)_cxx\\.bltn)"
                             "\\1.compiler_bug"
                             pos_base "${pos_base}")
        string(REGEX REPLACE "(pos_type\\.(c11|gen|bltn))"
                             "\\1.compiler_bug"  # TODO
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
set(tu_cntfn_params TRUE "countof_ns" "countof_ns/countof_ns.h"
                         tu_cntfn_available)
list(APPEND tu_params_list tu_cntfn_params)
