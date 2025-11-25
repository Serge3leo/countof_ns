# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# TODO: Более умный анализ
# short_example{,_cxx}_fail - 5 серий, и он же включается в long_example
# Остальные зависят
set(p_sect "below @{[^}@]*[Ee][Rr][Rr][Oo][Rr][^}@]*}@ Must error above")
set(p_short_example "${p_sect}.*${p_sect}.*${p_sect}.*${p_sect}.*${p_sect}")

foreach(t IN ITEMS short_example short_example_cxx  # TODO function?
                   long_example long_example_vla long_example_cxx)
    if((NOT ${t} MATCHES "_vla$" OR "${TAC_DEFINITIONS}" MATCHES "HAVE_VLA")
       AND (NOT ${t} MATCHES "_cxx$" OR NOT "${TAC_SKIP_CXX}"))
        add_test(NAME "${t}" COMMAND "${t}")
        if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"  OR
               CMAKE_GENERATOR STREQUAL "NMake Makefiles")
            # MSVC - для генераторов Visual Studio не работает, для "NMake
            # Makefiles" - туда-сюда, сообщения есть.
            add_test("${t}_fail" "${CMAKE_CTEST_COMMAND}"
                     --build-and-test "${CMAKE_SOURCE_DIR}"
                                      "${CMAKE_BINARY_DIR}"
                     --build-generator "${CMAKE_GENERATOR}"
                     --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                     --build-nocmake
                     --build-noclean
                     --build-target "${t}_fail")
            if(CMAKE_CXX_COMPILER_ID STREQUAL "Intel"  OR
               CMAKE_CXX_COMPILER_ID STREQUAL "NVHPC"  OR
               CMAKE_CXX_COMPILER_ID STREQUAL "PGI"  OR
               CMAKE_CXX_COMPILER_ID STREQUAL "SunPro")
                # TODO
                # icc - `#pragma message...` идут после всех ошибок причём без
                # номеров строк
                # pgcc - `#pragma message...` не пашет
                # suncc - `warning: unrecognized #pragma ignored: message...`
                set_property(TEST "${t}_fail" PROPERTY WILL_FAIL TRUE)
            else()
                set_property(TEST "${t}_fail" PROPERTY PASS_REGULAR_EXPRESSION
                             "${p_short_example}")
            endif()
        endif()
    endif()
endforeach()

enable_testing()
