# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set(CACHE COMPARISONS TYPE BOOL HELP "Add comparisons tests" VALUE FALSE)

file(GLOB positive_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "positive_*.h")
file(GLOB negative_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "negative_*.h")

function (process_test tgt lbl dflt pos src hdr inc)
    add_executable("${tgt}" "${src}" "${hdr}")
    target_compile_options("${tgt}" PRIVATE "${inc}")
    target_link_libraries("${tgt}" PRIVATE CounofNS::CounofNS)
    if (pos)
        target_compile_options("${tgt}" PRIVATE "${TAC_WERROR}")
        if (NOT "${dflt}" OR
           (${tgt} MATCHES "_vla_" AND NOT ${tgt} MATCHES "_vla$") OR
           (${tgt} MATCHES "_n0" AND NOT ${tgt} MATCHES "_cxx$") OR
           (${tgt} MATCHES "_cv" AND NOT HAVE_CV_TYPEOF))
            set_target_properties("${tgt}" PROPERTIES EXCLUDE_FROM_ALL TRUE)
            add_test("${tgt}" "${CMAKE_CTEST_COMMAND}"
                     --build-and-test "${CMAKE_SOURCE_DIR}"
                                      "${CMAKE_BINARY_DIR}"
                     --build-generator "${CMAKE_GENERATOR}"
                     --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                     --build-nocmake
                     --build-noclean
                     --build-target "${tgt}"
                     --test-command "${CMAKE_CURRENT_BINARY_DIR}/${tgt}")
        else ()
            add_test(NAME "${tgt}" COMMAND "${tgt}")
        endif ()
    else ()
        set_target_properties("${tgt}" PROPERTIES EXCLUDE_FROM_ALL TRUE)
        add_test("${tgt}" "${CMAKE_CTEST_COMMAND}"
                 --build-and-test "${CMAKE_SOURCE_DIR}"
                                  "${CMAKE_BINARY_DIR}"
                 --build-generator "${CMAKE_GENERATOR}"
                 --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                 --build-nocmake
                 --build-noclean
                 --build-target "${tgt}")
        set_property(TEST "${tgt}" PROPERTY WILL_FAIL TRUE)
    endif ()
    if ("${dflt}")
        list(APPEND lbl "default")
    endif ()
    set_tests_properties("${tgt}" PROPERTIES LABELS "${lbl}")
endfunction ()

function (process_hdr params hdr pos werror)
    list(GET params 0 dflt)
    list(GET params 1 have)
    list(GET params 2 cxx)
    list(GET params 3 tu_countof_func)
    list(GET params 4 tu_countof_inc)
    string(REGEX REPLACE ".h$" "" base "${hdr}")

    if (NOT "${have}" OR  # TODO
       ("${hdr}" MATCHES "_vla_" AND NOT HAVE_VLA) OR
       ("${hdr}" MATCHES "_zla_" AND NOT HAVE_ZERO_LENGTH_ARRAYS) OR
       ("${hdr}" MATCHES "_struct_" AND NOT HAVE_EMPTY_STRUCTURE) OR
       ("${hdr}" MATCHES "_alone_" AND NOT HAVE_ALONE_FLEXIBLE_ARRAY))
        return()
    endif ()

    if (NOT "${dflt}")
        set(base "${tu_countof_func}-${base}")
    endif ()
    set(inc "-DTU_UNIT_INC=\"${hdr}\""
            "-DTU_COUNTOF_FUNC=${tu_countof_func}"
            "-DTU_COUNTOF_INC=\"${tu_countof_inc}\"")
    list(APPEND hdr "${CMAKE_SOURCE_DIR}/include/${tu_countof_inc}")
    process_test("${base}" "${tu_countof_func}" "${dflt}" "${pos}"
                           tu_stub.c "${hdr}" "${inc}")
    if (HAVE_VLA AND "${dflt}")
        process_test("${base}_vla" "${tu_countof_func}" "${dflt}" "${pos}"
                                   tu_stub.c "${hdr}" "${inc}")
        target_compile_options("${base}_vla" PRIVATE -D_COUNTOF_NS_WANT_C11_VLA)
    endif ()
    if (CXX_ENABLED AND NOT "${base}" MATCHES "_vla_" AND "${cxx}")
        process_test("${base}_cxx" "${tu_countof_func}" "${dflt}" "${pos}"
                                   tu_stub_cxx.cpp "${hdr}" "${inc}")
    endif ()
endfunction ()

try_compile(HAVE_ALX_COUNTOF
            SOURCE_FROM_CONTENT builtin_types_compatible_p.c [=[
        #include <assert.h>
        #if __has_builtin(__builtin_types_compatible_p)
            // JZMG_ARRAY_LEN() use `static_assert()`, not `_Static_assert()`
            static_assert(1);
            int main(void) {}
        #endif
        ]=])
if (HAVE_EMPTY_STRUCTURE AND ERROR_ON_STRUCT_STATIC_ASSERT)
    # Strictly: _Static_assert&__builtin_types_compatible_p, but redundant
    set(HAVE_LNX_ARRAY_SIZE TRUE)
endif ()

set(ARRAY_LEN_params FALSE TRUE TRUE
    "JZMG_ARRAY_LEN" "_comparisons/JZMG_ARRAY_LEN.h")
set(array_size_params FALSE "${HAVE_LNX_ARRAY_SIZE}" FALSE
    "LNX_ARRAY_SIZE" "_comparisons/lnx_array_size.h")
set(COUNTOF_params FALSE "${HAVE_ALX_COUNTOF}" FALSE
    "ALX_COUNTOF" "_comparisons/ALX_COUNTOF.h")
set(countof_params FALSE "${HAVE_COUNTOF}" FALSE
    "stdc_countof" "_comparisons/stdc_countof.h")
set(countof_ns_params TRUE TRUE TRUE
    "countof_ns" "countof_ns.h")
set(ms_countof_params FALSE TRUE TRUE
    "ms_countof" "_comparisons/ms_countof.h")

foreach (params IN ITEMS countof_ns_params countof_params ARRAY_LEN_params
                         COUNTOF_params array_size_params ms_countof_params)
    list(GET ${params} 0 dflt)
    list(GET ${params} 1 have)
    list(GET ${params} 3 tu_countof_func)
    if (NOT dflt)
        if (NOT COMPARISONS)
            continue ()
        endif ()
        if (NOT have)
            set(tgt "${tu_countof_func}-Don_t_have")
            add_test(NAME "${tgt}" COMMAND ${CMAKE_COMMAND} -E false)
            set_tests_properties("${tgt}" PROPERTIES SKIP_RETURN_CODE 1)
            continue ()
        endif ()
    endif ()
    foreach (h IN LISTS positive_hdr)
        process_hdr("${${params}}" "${h}" TRUE "${TAC_WERROR}")
    endforeach ()
    foreach (h IN LISTS negative_hdr)
        process_hdr("${${params}}" "${h}" FALSE "")
    endforeach ()
endforeach ()

enable_testing()
