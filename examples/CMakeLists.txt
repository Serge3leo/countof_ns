# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (MSVC)
    if (MSVC_VERSION GREATER_EQUAL 1914)
        # https://gitlab.kitware.com/cmake/cmake/-/issues/18837
        string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
    endif ()
endif ()

function (example src sfx opt fail_rex)
    get_filename_component(tgt ${src} NAME_WLE)
    string(APPEND tgt ${sfx})
    foreach (t IN ITEMS ${tgt} ${tgt}.build_fail)
        add_executable(${t} ${src} short_example.h)
        target_link_libraries(${t} PRIVATE CountofNS::CountofNS)
        target_compile_options(${t} PRIVATE ${${opt}})
    endforeach ()
    add_test(NAME ${tgt} COMMAND ${tgt})
    set_tests_properties(${tgt} PROPERTIES
                         FAIL_REGULAR_EXPRESSION "${fail_rex}")
    target_compile_definitions(${tgt}.build_fail PRIVATE EXAMPLE_FAIL)
        # Remove from build, because `-DEXAMPLE_FAIL` cause compilation
        # error.
    set_target_properties(${tgt}.build_fail PROPERTIES EXCLUDE_FROM_ALL TRUE)
        # We are trying build it during the tests.
    add_test(NAME ${tgt}.build_fail
             COMMAND ${CMAKE_COMMAND}
                --build .
                --target ${tgt}.build_fail
                --config "${CMAKE_BUILD_TYPE}"
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        # We shouldn't be able to do this.
    set_tests_properties(${tgt}.build_fail PROPERTIES
                         # TODO Need inverse FIXTURES_SETUP ${tgt}.build_fail
                         WILL_FAIL TRUE)
        # If ${tgt}.build_fail - fail, i.e. unexpected success build, try run
        # it (for debug, etc)
    # TODO add_test(NAME ${tgt}.run_fail COMMAND ${tgt}.build_fail)
    # TODO set_tests_properties(${tgt}.run_fail PROPERTIES
    # TODO                      FIXTURES_REQUIRED ${tgt}.build_fail
    # TODO                      WILL_FAIL TRUE
    # TODO                      LABELS "short_example;examples;default")
    add_test(${tgt}.run_fail "${CMAKE_CTEST_COMMAND}"
             --build-and-test "${CMAKE_SOURCE_DIR}"
                              "${CMAKE_BINARY_DIR}"
             --build-generator "${CMAKE_GENERATOR}"
             --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
             --build-nocmake
             --build-noclean
             --build-target ${tgt}.build_fail
             --test-command ${tgt}.build_fail)
    set_tests_properties(${tgt}.run_fail PROPERTIES
                         DEPEND ${tgt}.build_fail
                         # TODO Need inverse FIXTURES_REQUIRE ${tgt}.build_fail
                         WILL_FAIL TRUE)
    set_tests_properties(${tgt} ${tgt}.build_fail ${tgt}.run_fail PROPERTIES
                         LABELS "${tgt};examples;default")

endfunction ()

if (COUNTOF_NS_SHORT_EXAMPLE)
    example(short_example.c "" "" "")
    example(short_example_cxx.cpp "" "" "")  # TODO remove? stl problem?
endif ()

if (COUNTOF_NS_EXAMPLES)
    include(countof_ns_builtin_options.cmake)

    example(long_example.c "" "" "")
    if (HAVE_VLA AND NOT HAVE_BROKEN_VLA)
        if (ERROR_ON_SIZEOF_POINTER_SUBTRACTION)
                # If the compiler complies with the C11 constraints for
                # pointer subtraction
            set(opt_c11 "-DENABLE_VLA_C11_EXAMPLE")
            example(long_example.c "_c11" opt_c11 "_COUNTOF_NS_VLA_UNSUPPORTED")
        endif ()
        set(opt_bltn "-DENABLE_VLA_BUILTIN_EXAMPLE"
                     ${countof_ns_builtin_options_${CMAKE_C_COMPILER_ID}})
        example(long_example.c "_bltn" opt_bltn "_COUNTOF_NS_VLA_UNSUPPORTED")
    endif ()
    if (CXX_ENABLED)
        example(long_example_cxx.cpp "" "" "")
        if (HAVE_IS_SAME_CXX AND HAVE_VLA_CXX AND NOT HAVE_BROKEN_VLA_CXX)
            set(opt_cxx_bltn "-DENABLE_VLA_BUILTIN_EXAMPLE"
                        ${countof_ns_builtin_options_${CMAKE_C_COMPILER_ID}})
            example(long_example_cxx.cpp "_bltn" opt_cxx_bltn
                                         "_COUNTOF_NS_VLA_UNSUPPORTED")
        endif ()
    endif ()
endif ()

enable_testing()
