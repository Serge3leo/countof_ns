# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

cmake_minimum_required (VERSION 3.25)
project(example_fetch_content_countof_ns
    VERSION 0.1.0
    DESCRIPTION "Example use countof_ns with cmake FetchContent"
    LANGUAGES C)

set(CMAKE_C_STANDARD_REQUIRED 99)  # With C extension `__typeof__()` et al.
set(CMAKE_C_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
    CountofNS
    GIT_REPOSITORY https://github.com/Serge3leo/countof_ns.git
    GIT_TAG  71aa5528b84dbdc168a4750e4621a209ae5903c2 # v0.1.0-pre-examples
        # This is not the last tag, it is the tag immediately preceding the
        # v0.1.0-pre tag.
)
FetchContent_MakeAvailable(CountofNS)

foreach(tgt IN ITEMS shortest shortest.build_fail)
    add_executable(${tgt} shortest.c)
    target_link_libraries(${tgt} PRIVATE CountofNS::CountofNS)
    if (NOT tgt MATCHES "[.]build_fail")
        add_test(NAME ${tgt} COMMAND ${tgt})
    else ()
        target_compile_definitions(${tgt} PRIVATE EXAMPLE_FAIL)
            # Remove from build, because `-DEXAMPLE_FAIL` cause compilation
            # error.
        set_target_properties("${tgt}" PROPERTIES EXCLUDE_FROM_ALL TRUE)
            # We are trying build it during the tests.
        add_test("${tgt}" "${CMAKE_CTEST_COMMAND}"
                     --build-and-test "${CMAKE_SOURCE_DIR}"
                                      "${CMAKE_BINARY_DIR}"
                     --build-generator "${CMAKE_GENERATOR}"
                     --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                     --build-nocmake
                     --build-noclean
                     --build-target "${tgt}")
            # We shouldn't be able to do this.
        set_tests_properties(${tgt} PROPERTIES WILL_FAIL TRUE)
    endif ()
endforeach ()

enable_testing()
