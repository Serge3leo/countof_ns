# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

function (example src sfx opt fail_rex)
    get_filename_component(tgt ${src} NAME_WLE)
    string(APPEND tgt ${sfx})
    foreach (t IN ITEMS ${tgt} ${tgt}_must_fail)
        add_executable(${t} ${src} short_example.h)
        target_link_libraries(${t} PRIVATE CountofNS::CountofNS)
        target_compile_options(${t} PRIVATE ${${opt}})
    endforeach ()
    add_test(NAME ${tgt} COMMAND ${tgt})
    set_tests_properties(${tgt} PROPERTIES
                         FAIL_REGULAR_EXPRESSION "${fail_rex}")
    target_compile_definitions(${tgt}_must_fail PRIVATE EXAMPLE_FAIL)
        # Remove from build, because `-DEXAMPLE_FAIL` cause compilation
        # error.
    set_target_properties(${tgt}_must_fail PROPERTIES EXCLUDE_FROM_ALL TRUE)
        # We are trying build it during the tests.
    add_test(${tgt}_must_fail "${CMAKE_CTEST_COMMAND}"
                 --build-and-test "${CMAKE_SOURCE_DIR}"
                                  "${CMAKE_BINARY_DIR}"
                 --build-generator "${CMAKE_GENERATOR}"
                 --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                 --build-nocmake
                 --build-noclean
                 --build-target ${tgt}_must_fail)
        # We shouldn't be able to do this.
    set_tests_properties(${tgt}_must_fail PROPERTIES WILL_FAIL TRUE)
    set_tests_properties(${tgt} ${tgt}_must_fail PROPERTIES
                         LABELS "short_example;examples;default")
endfunction ()

example(short_example.c "" "" "")
example(short_example_cxx.cpp "" "" "")  # TODO remove? stl problem?

if(COUNTOF_NS_EXAMPLES)
    include(countof_ns_builtin_options.cmake)

    example(long_example.c "" "" "")
    if (HAVE_VLA AND NOT HAVE_BROKEN_VLA)
        if (ERROR_ON_SIZEOF_POINTER_SUBTRACTION)
            set(opt_c11 "-DENABLE_VLA_C11_EXAMPLE")
            example(long_example.c "_c11" opt_c11 "_COUNTOF_NS_VLA_UNSUPPORTED")
        endif ()
        set(opt_bltn "-DENABLE_VLA_BUILTIN_EXAMPLE"
                     ${countof_ns_builtin_options_${CMAKE_C_COMPILER_ID}})
        example(long_example.c "_bltn" opt_bltn "_COUNTOF_NS_VLA_UNSUPPORTED")
    endif ()
    if (CXX_ENABLED)
        example(long_example_cxx.cpp "" "" "")
    endif ()
endif ()

enable_testing()
