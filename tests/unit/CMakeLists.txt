# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

file(GLOB positive_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "positive_*.h")
file(GLOB negative_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "negative_*.h")

function(process_test tgt pos src hdr inc)
    add_executable("${tgt}" "${src}" "${hdr}")
    target_compile_options("${tgt}" PRIVATE "${inc}")
    target_link_libraries("${tgt}" PRIVATE CounofNS::CounofNS)
    if(pos)
        target_compile_options("${tgt}" PRIVATE "${TAC_WERROR}")
        if((${tgt} MATCHES "_vla_" AND NOT ${tgt} MATCHES "_vla$") OR
           (${tgt} MATCHES "_n0" AND NOT ${tgt} MATCHES "_cxx$") OR
           (${tgt} MATCHES "_cv" AND NOT HAVE_CV_TYPEOF))
            set_target_properties("${tgt}" PROPERTIES EXCLUDE_FROM_ALL TRUE)
            if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC"  OR
                   CMAKE_GENERATOR STREQUAL "NMake Makefiles")
                add_test("${tgt}" "${CMAKE_CTEST_COMMAND}"
                         --build-and-test "${CMAKE_SOURCE_DIR}"
                                          "${CMAKE_BINARY_DIR}"
                         --build-generator "${CMAKE_GENERATOR}"
                         --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                         --build-nocmake
                         --build-noclean
                         --build-target "${tgt}"
                         --test-command "${tgt}")
            endif()
        else()
            add_test(NAME "${tgt}" COMMAND "${tgt}")
        endif()
    else()
        set_target_properties("${tgt}" PROPERTIES EXCLUDE_FROM_ALL TRUE)
        if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC"  OR
               CMAKE_GENERATOR STREQUAL "NMake Makefiles")
            # MSVC - для генераторов Visual Studio не работает, для "NMake
            # Makefiles"
            add_test("${tgt}" "${CMAKE_CTEST_COMMAND}"
                     --build-and-test "${CMAKE_SOURCE_DIR}"
                                      "${CMAKE_BINARY_DIR}"
                     --build-generator "${CMAKE_GENERATOR}"
                     --build-makeprogram "${CMAKE_MAKE_PROGRAM}"
                     --build-nocmake
                     --build-noclean
                     --build-target "${tgt}")
            set_property(TEST "${tgt}" PROPERTY WILL_FAIL TRUE)
        endif()
    endif()
endfunction()

function(process_hdr hdr pos werror)
    string(REGEX REPLACE ".h$" "" base "${hdr}")
    set(inc "-DTU_UNIT_INC=\"${hdr}\"")

    if(("${hdr}" MATCHES "_vla_" AND NOT HAVE_VLA) OR
       ("${hdr}" MATCHES "_zla_" AND NOT HAVE_ZERO_LENGTH_ARRAYS) OR
       ("${hdr}" MATCHES "_struct_" AND NOT HAVE_EMPTY_STRUCTURE) OR
       ("${hdr}" MATCHES "_alone_" AND NOT HAVE_ALONE_FLEXIBLE_ARRAY))
        return()
    endif()
    process_test("${base}" "${pos}" tu_stub.c "${hdr}" "${inc}")
    if(HAVE_VLA)
        process_test("${base}_vla" "${pos}" tu_stub.c "${hdr}" "${inc}")
        target_compile_options("${base}_vla" PRIVATE -D_COUNTOF_NS_WANT_C11_VLA)
    endif()
    if(CXX_ENABLED AND NOT "${base}" MATCHES "_vla_")
        process_test("${base}_cxx" "${pos}" tu_stub_cxx.cpp "${hdr}" "${inc}")
    endif()
endfunction()

foreach(h IN LISTS positive_hdr)
    process_hdr("${h}" TRUE "${TAC_WERROR}")
endforeach()
foreach(h IN LISTS negative_hdr)
    process_hdr("${h}" FALSE "")
endforeach()

enable_testing()
