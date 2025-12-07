# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

include(unit_cases.cmake)
include(countof_ns_expected.cmake)

# ./examples-build.sh -c . -- "-DCOUNTOF_NS_EXAMPLES=ON" "-DCOUNTOF_NS_TESTS=ON"
# suncc - 60
# pgcc - 104
# icx - 121
# gcc-11 - 104
# clang-14 - 104
# gcc-mp-15 - 121
# calng-mp-21 - 121
# icc - 104
# MSVC - 26

include(${CMAKE_SOURCE_DIR}/examples/countof_ns_builtin_options.cmake)

countof_ns_expected(expected "${pos_pos}" "${neg_pos}")

foreach (tgt IN ITEMS ${expected})
    list(APPEND lbl "default")
    get_filename_component(base ${tgt} NAME_WE)
    if (base MATCHES "_cxx")
        string(REGEX REPLACE "_cxx" "" base ${base})
        set(src tu_stub_cxx.cpp)
    else ()
        set(src tu_stub.c)
    endif ()
    list(APPEND src tu_stub.h ${base}.h) # countof_ns.h)
    set(hdr ${base}.h)
    add_executable(${tgt} ${src} ${hdr})
    target_link_libraries(${tgt} PRIVATE CountofNS::CountofNS)
    if (tgt MATCHES "[.]c11")
        target_compile_definitions(${tgt} PRIVATE ${inc}
                                   "_COUNTOF_NS_WANT_VLA_C11")
        set(vla_fail_regex "_COUNTOF_NS_VLA_UNSUPPORTED")
    elseif (tgt MATCHES "[.]bltn")
        target_compile_definitions(${tgt} PRIVATE ${inc}
                                   "_COUNTOF_NS_WANT_VLA_BUILTIN")
        target_compile_options(${tgt} PRIVATE
                ${countof_ns_builtin_options_${CMAKE_C_COMPILER_ID}})
        set(vla_fail_regex "_COUNTOF_NS_VLA_UNSUPPORTED")
    else ()
        set(vla_fail_regex "_COUNTOF_NS_WANT_VLA_")
    endif ()
    set(inc "-DTU_UNIT_INC=\"${hdr}\"")
    target_compile_options(${tgt} PRIVATE ${inc})
    if (NOT tgt MATCHES "^neg")
        target_compile_options(${tgt} PRIVATE "${TAC_WERROR}")
    endif ()
    if (tgt MATCHES "[.]build_.*")
        set_target_properties(${tgt} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        add_test(NAME ${tgt}
                 COMMAND ${CMAKE_COMMAND}
                    --build .
                    --target ${tgt}
                    --config "${CMAKE_BUILD_TYPE}"
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        if (tgt MATCHES "[.][^.]*_fail")
            set_tests_properties(${tgt} PROPERTIES WILL_FAIL TRUE)
        endif ()
    elseif (NOT tgt MATCHES "[.][^.]*_fail")
        add_test(NAME ${tgt} COMMAND ${tgt})
    else ()
        set_target_properties(${tgt} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        add_test(${tgt} "${CMAKE_CTEST_COMMAND}"
                 --build-and-test "${CMAKE_SOURCE_DIR}"
                                  "${CMAKE_BINARY_DIR}"
                 --build-generator "${CMAKE_GENERATOR}"
                 --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                 --build-config "${CMAKE_BUILD_TYPE}"
                 --build-nocmake
                 --build-noclean
                 --build-target ${tgt}
                 --test-command "${CMAKE_CURRENT_BINARY_DIR}/${tgt}")
        set_tests_properties(${tgt} PROPERTIES WILL_FAIL TRUE)
    endif ()
    set_tests_properties(${tgt} PROPERTIES
                         FAIL_REGULAR_EXPRESSION "${vla_fail_regex}"
                         LABELS "${lbl}")
endforeach ()

enable_testing()
