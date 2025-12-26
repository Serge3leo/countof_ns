# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

function (tu_countof_ns_expected expected pos_pos neg_pos)
    set(pos_base "")
    foreach (b IN ITEMS ${pos_pos})
        if (b MATCHES "vla")
            set(ints ${b}.gen.build_fail ${b}.c11 ${b}.bltn
                     ${b}_cxx.tmpl.build_fail ${b}_cxx.bltn)
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
    if (CMAKE_CXX_COMPILER_ID STREQUAL GNU AND
        CMAKE_CXX_COMPILER_VERSION MATCHES "^1[1-5]\\.")
        string(REGEX REPLACE "(pos_vla_(alone|zla)_[0n]0_cxx(\\.bltn|\\.tmpl))"
                             "\\1.compiler_bug"
                             pos_base "${pos_base}")
    endif ()
    if ((CMAKE_CXX_COMPILER_ID STREQUAL Intel AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^2021\\." ) OR
        (CMAKE_CXX_COMPILER_ID STREQUAL LCC AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^1\\.27\\.") OR
        (CMAKE_CXX_COMPILER_ID STREQUAL NVHPC AND
         CMAKE_CXX_COMPILER_VERSION MATCHES "^25\\.9\\." ))
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
        string(REGEX REPLACE "(pos_vla_(struct|zla)_00\\.gen)\\.build_fail"
                             "\\1"
                             pos_base "${pos_base}")
        string(REGEX REPLACE "(pos_vla_(struct|zla)_n0\\.gen)\\.build_fail"
                             "\\1.run_fail"
                             pos_base "${pos_base}")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
            # TODO I don't understand. Why?
        string(REGEX REPLACE "(neg_alone_ptr\\.c11)\\.build_unexpected"
                             "\\1.build_fail"
                             neg_base "${neg_base}")
    endif ()

    set(${expected} "${pos_base};${neg_base}" PARENT_SCOPE)
endfunction ()

tu_countof_ns_expected(tu_countof_ns_available "${tu_pos_pos}" "${tu_neg_pos}")
set(tu_countof_ns_params TRUE "countof_ns" "countof_ns.h"
                         tu_countof_ns_available)
list(APPEND tu_params_list tu_countof_ns_params)
