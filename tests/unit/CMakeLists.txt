# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

set(CACHE COMPARISONS TYPE BOOL HELP "Add comparisons tests" VALUE FALSE)

# Positive and check tests must compile and successful execute (for VLA used
# runtime asserts). They should return Ok, a small number, and debugging
# information (Warning: TODO: If there is a build error, it's still about
# "Required regular expression not found...").

# TODO For compassions Visual Studio generator is not reliable, "NMake
# Makefiles" is working as expected is working as expected.

file(GLOB check_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "check_*.h")
file(GLOB pos_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "pos_*.h")
set(pos_pass_regexp "Ok [0-9] TU_[A-Z_]*ASSERT_AND_RETURN")

# Negative tests must don't compile. But, for comparison purposes, we are
# trying to find out what a wrongly compiled test does.

file(GLOB neg_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "neg_*.h")
set(neg_not_terrifying_regexp "Fail 0 desired=")

function (process_test tgt lbl dflt pos src hdr inc)
    if ("${dflt}")
        list(APPEND lbl "default")
    endif ()
    add_executable("${tgt}" "${src}" "${hdr}")
    target_compile_options("${tgt}" PRIVATE "${inc}")
    target_link_libraries("${tgt}" PRIVATE CounofNS::CounofNS)
    if (pos)
        target_compile_options("${tgt}" PRIVATE "${TAC_WERROR}")
        if (NOT "${dflt}" OR
           (${tgt} MATCHES "_vla" AND NOT ${tgt} MATCHES "_c11$") OR
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
        set_tests_properties("${tgt}" PROPERTIES
                             PASS_REGULAR_EXPRESSION "${pos_pass_regexp}")
    else ()
        set_target_properties("${tgt}" PROPERTIES EXCLUDE_FROM_ALL TRUE)
        add_test(NAME "${tgt}"
                 COMMAND ${CMAKE_COMMAND}
                    --build .
                    --target "${tgt}"
                    --config $<CONFIGURATION>
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        set_tests_properties("${tgt}" PROPERTIES WILL_FAIL TRUE)

        add_test(NAME "${tgt}_0_fail"
                 COMMAND ${CMAKE_COMMAND}
                         -DCMD=${CMAKE_CURRENT_BINARY_DIR}/${tgt}
                         -P ${CMAKE_CURRENT_SOURCE_DIR}/exec_if_exists.cmake)
                         # TODO https://stackoverflow.com/a/3071370/8585880
        set_tests_properties("${tgt}_0_fail" PROPERTIES
                        PASS_REGULAR_EXPRESSION "${neg_not_terrifying_regexp}"
                        DEPENDS "${tgt}"
                        REQUIRED_FILES "${CMAKE_CURRENT_BINARY_DIR}/${tgt}"
                        LABELS "${lbl}")
    endif ()
    set_tests_properties("${tgt}" PROPERTIES LABELS "${lbl}")
endfunction ()

function (process_hdr params hdr pos werror)
    list(GET params 0 dflt)
    list(GET params 1 langs)
    list(GET params 2 tu_countof_func)
    list(GET params 3 tu_countof_inc)
    string(REGEX REPLACE ".h$" "" base "${hdr}")

    if (("${hdr}" MATCHES "_vla" AND NOT HAVE_VLA) OR
        ("${hdr}" MATCHES "_zla" AND NOT HAVE_ZERO_LENGTH_ARRAYS) OR
        ("${hdr}" MATCHES "_struct" AND NOT HAVE_EMPTY_STRUCTURE) OR
        ("${hdr}" MATCHES "_alone" AND NOT HAVE_ALONE_FLEXIBLE_ARRAY))
        return()
    endif ()

    if (NOT "${dflt}")
        set(base "${tu_countof_func}-${base}")
    endif ()
    set(inc "-DTU_UNIT_INC=\"${hdr}\""
            "-DTU_COUNTOF_FUNC=${tu_countof_func}"
            "-DTU_COUNTOF_INC=\"${tu_countof_inc}\"")
    list(APPEND hdr "${CMAKE_SOURCE_DIR}/include/${tu_countof_inc}")
    if (langs MATCHES "CLANG")
        process_test("${base}" "${tu_countof_func}" "${dflt}" "${pos}"
                               tu_stub.c "${hdr}" "${inc}")
    endif ()
    if (langs MATCHES "CLANG" AND "${dflt}" AND HAVE_VLA)
        process_test("${base}_c11" "${tu_countof_func}" "${dflt}" "${pos}"
                                   tu_stub.c "${hdr}" "${inc}")
        target_compile_options("${base}_c11" PRIVATE -D_COUNTOF_NS_WANT_C11_VLA)
    endif ()
    if (langs MATCHES "CXX" AND CXX_ENABLED AND NOT "${base}" MATCHES "_vla")
        process_test("${base}_cxx" "${tu_countof_func}" "${dflt}" "${pos}"
                                   tu_stub_cxx.cpp "${hdr}" "${inc}")
    endif ()
endfunction ()

set(countof_ns_params TRUE "CLANG CXX"
    "countof_ns" "countof_ns.h")
set(cmp_list countof_ns_params)

if (COMPARISONS)
    if (HAVE_COUNTOF)
        set(countof_params FALSE "CLANG"
            "stdc_countof" "_comparisons/stdc_countof.h")
        list(APPEND cmp_list countof_params)
    endif()

    try_compile(HAVE_STD_SIZE SOURCE_FROM_CONTENT have_std_size.cpp [=[
            #include <vector>
            int a[1];
            static_assert(1 == std::size(a));
            int main(void) {}
            ]=])
    if (HAVE_STD_SIZE)
        set(std_size_params FALSE "CXX"
            "std_size" "_comparisons/std_size.h")
        list(APPEND cmp_list std_size_params)
    endif ()

    set(ARRAY_LEN_params FALSE "CLANG CXX"
        "JZMG_ARRAY_LEN" "_comparisons/JZMG_ARRAY_LEN.h")
    list(APPEND cmp_list ARRAY_LEN_params)

    if (HAVE_EMPTY_STRUCTURE AND ERROR_ON_STRUCT_STATIC_ASSERT)
        # Strictly: _Static_assert&__builtin_types_compatible_p, but redundant
        set(array_size_params FALSE "CLANG"
            "LNX_ARRAY_SIZE" "_comparisons/lnx_array_size.h")
        list(APPEND cmp_list array_size_params)
    endif ()

    try_compile(HAVE_ALX_COUNTOF
                SOURCE_FROM_CONTENT builtin_types_compatible_p.c [=[
            #include <assert.h>
            #if __has_builtin(__builtin_types_compatible_p)
                // ALX_COUNTOF() use `static_assert()`, not `_Static_assert()`
                static_assert(1);
                int main(void) {}
            #endif
            ]=])
    if (HAVE_ALX_COUNTOF)
        set(COUNTOF_params FALSE "CLANG"
            "ALX_COUNTOF" "_comparisons/ALX_COUNTOF.h")
        list(APPEND cmp_list COUNTOF_params)
    endif ()

    set(ms_countof_params FALSE "CLANG CXX"
        "ms_countof" "_comparisons/ms_countof.h")
    list(APPEND cmp_list ms_countof_params)
endif()

foreach (params IN ITEMS ${cmp_list})
    foreach (h IN LISTS check_hdr pos_hdr)
        process_hdr("${${params}}" "${h}" TRUE "${TAC_WERROR}")
    endforeach ()
    foreach (h IN LISTS neg_hdr)
        process_hdr("${${params}}" "${h}" FALSE "")
    endforeach ()
endforeach ()

enable_testing()
