# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

if (MSVC)
    if (MSVC_VERSION GREATER_EQUAL 1914)
        # https://gitlab.kitware.com/cmake/cmake/-/issues/18837
        string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus")
    endif ()
endif ()

function (example src opt fail_rex max_example_fail)
    get_filename_component(tgt ${src} NAME_WLE)
    set(build_fails "")
    foreach (i RANGE 1 ${max_example_fail})
        list(APPEND build_fails ${tgt}.build_fail-${i})
    endforeach ()
    foreach (t IN ITEMS ${tgt} ${build_fails})
        add_executable(${t} ${src} short_example.h)
        target_link_libraries(${t} PRIVATE CountofNS::CountofNS)
        target_compile_options(${t} PRIVATE ${opt})
    endforeach ()
    if (NOT ${tgt} MATCHES "short_example")
        target_compile_options(${tgt} PRIVATE "${TAC_WERROR}")
    endif ()
    add_test(NAME ${tgt} COMMAND ${tgt})
    set_tests_properties(${tgt} PROPERTIES
                         FAIL_REGULAR_EXPRESSION "${fail_rex}")
    set(run_fails "")
    foreach (build_fail IN ITEMS ${build_fails})
        string(REGEX REPLACE "(^.*)\\.build_fail-([0-9]+)" "\\1.run_fail-\\2"
                             run_fail ${build_fail})
        list(APPEND run_fails ${run_fail})
        string(REGEX REPLACE "^.*\\.build_fail-([0-9]+)" "EXAMPLE_FAIL=\\1"
                             example_fail_def ${build_fail})
        target_compile_definitions(${build_fail} PRIVATE ${example_fail_def})
            # Remove from build, because `-DEXAMPLE_FAIL` cause compilation
            # error.
        set_target_properties(${build_fail} PROPERTIES EXCLUDE_FROM_ALL TRUE)
            # We are trying build it during the tests.
        add_test(NAME ${build_fail}
                 COMMAND ${CMAKE_COMMAND}
                    --build .
                    --target ${build_fail}
                    --config "${CMAKE_BUILD_TYPE}"
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
            # We shouldn't be able to do this.
        set_tests_properties(${build_fail} PROPERTIES
                             # TODO Need inverse FIXTURES_SETUP ${tgt}.build_fail
                             WILL_FAIL TRUE)
            # If ${tgt}.build_fail - fail, i.e. unexpected success build, try run
            # it (for debug, etc)
        # TODO add_test(NAME ${tgt}.run_fail COMMAND ${tgt}.build_fail)
        # TODO set_tests_properties(${tgt}.run_fail PROPERTIES
        # TODO                      FIXTURES_REQUIRED ${tgt}.build_fail
        # TODO                      WILL_FAIL TRUE
        # TODO                      LABELS "short_example;examples;default")
        add_test(${run_fail} "${CMAKE_CTEST_COMMAND}"
                 --build-and-test "${CMAKE_SOURCE_DIR}"
                                  "${CMAKE_BINARY_DIR}"
                 --build-generator "${CMAKE_GENERATOR}"
                 --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                 --build-nocmake
                 --build-noclean
                 --build-target ${build_fail}
                 --test-command "${CMAKE_CURRENT_BINARY_DIR}/${build_fail}")
        set_tests_properties(${run_fail} PROPERTIES
                             DEPEND ${build_fail}
                             # TODO Need inverse FIXTURES_REQUIRE ${tgt}.build_fail
                             WILL_FAIL TRUE)
    endforeach ()
    set_tests_properties(${tgt} ${build_fails} ${run_fails} PROPERTIES
                         LABELS "${tgt};examples;default")

endfunction ()

if (COUNTOF_NS_SHORT_EXAMPLE)
    example(short_example.c "" "" 1)
    if (CXX_ENABLED)
        example(short_example_cxx.cpp "" "" 1)
    endif ()
endif ()

if (COUNTOF_NS_EXAMPLES)
    include(countof_ns_builtin_options.cmake)

    if (NOT HAVE___STDC_NO_VLA__)
        set(opt_c ${countof_ns_builtin_c_options_${CMAKE_C_COMPILER_ID}})
        set(fail_rex "_COUNTOF_NS_VLA_UNSUPPORTED")
    else ()
        set(opt_c "")
        set(fail_rex "")
    endif ()
    example(long_example.c "${opt_c}" "${fail_rex}" 1)
    if (CXX_ENABLED)
        if (HAVE_VLA_CXX)
            set(opt_cxx
                ${countof_ns_builtin_cxx_options_${CMAKE_CXX_COMPILER_ID}})
        else ()
            set(opt_cxx "")
        endif ()
        example(long_example_cxx.cpp "${opt_cxx}" "${fail_rex}" 1)
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL SunPro)
        # Duplication is possible, for the sake of independence from
        # tests/autoconf/autoconf.cmake
        list(APPEND opt_c "-features=zla" "-DHAVE_ZLA")
        list(APPEND opt_cxx "-features=zla" "-DHAVE_ZLA")
    endif ()
    if (HAVE___STDC_NO_VLA__)
        if (HAVE___TYPEOF__ OR HAVE_TYPEOF)
            example(godbolt_c.c "" "" 4)  # MSVC - 4 (not VLA&ZLA)
        else ()
            example(godbolt_c.c "" "" 1)
        endif ()
    else ()
        example(godbolt_c.c "${opt_c}" "${fail_rex}" 7)
        if (HAVE_BROKEN_FUNC_PARAMETER)
            message("godbolt_c.build_fail-3 - unexpected build due of"
                    " broken sizeof functions arguments")
            set_tests_properties("godbolt_c.build_fail-3" PROPERTIES
                                 WILL_FAIL FALSE)
        endif ()
    endif ()
    if (CXX_ENABLED)
        if (NOT HAVE_VLA_CXX)
            example(godbolt_cxx.cpp "" "" 4)  # MSVC - 4 (not VLA&ZLA)
        else ()
            example(godbolt_cxx.cpp "${opt_cxx}" "${fail_rex}" 6)
        endif ()
    endif ()
endif ()

enable_testing()
