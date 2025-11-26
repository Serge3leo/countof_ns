# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

add_definitions(${TAC_ADD_DEFINITIONS} ${TAC_DEFINITIONS})

file(GLOB positive_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "positive_*.h")
file(GLOB negative_hdr RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "negative_*.h")

function(process_test tgt pos)
    if(pos)
        if((${tgt} MATCHES "_vla_" AND NOT ${tgt} MATCHES "_vla$") OR
           (${tgt} MATCHES "_n0" AND NOT ${tgt} MATCHES "_cxx$") OR
           (${tgt} MATCHES "_cv" AND
            NOT TAC_DEFINITIONS MATCHES "HAVE_CV_TYPEOF"))
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

    if(("${hdr}" MATCHES "_vla_" AND
        NOT TAC_DEFINITIONS MATCHES "HAVE_VLA") OR
       ("${hdr}" MATCHES "_zla_" AND
        NOT TAC_DEFINITIONS MATCHES "HAVE_ZERO_LENGTH_ARRAYS") OR
       ("${hdr}" MATCHES "_struct_" AND
        NOT TAC_DEFINITIONS MATCHES "HAVE_EMPTY_STRUCTURE") OR
       ("${hdr}" MATCHES "_alone_" AND
        NOT TAC_DEFINITIONS MATCHES "HAVE_ALONE_FLEXIBLE_ARRAY"))
        return()
    endif()
    add_executable("${base}" tu_stub.c "${hdr}")
    target_compile_options("${base}" PRIVATE "${inc}")
    target_link_libraries("${base}" PRIVATE CounofNS::CounofNS)
    process_test("${base}" "${pos}")

    if(TAC_DEFINITIONS MATCHES "HAVE_VLA")
        add_executable("${base}_vla" tu_stub.c "${hdr}")
        target_compile_options("${base}_vla" PRIVATE "${inc}" -D_COUNTOF_NS_WANT_C11_VLA)
        target_link_libraries("${base}_vla" PRIVATE CounofNS::CounofNS)
        process_test("${base}_vla" "${pos}")
    endif()

    if(NOT TAC_SKIP_CXX AND NOT "${base}" MATCHES "_vla_")
        add_executable("${base}_cxx" tu_stub_cxx.cpp "${hdr}")
        target_compile_options("${base}_cxx" PRIVATE "${inc}")
        target_link_libraries("${base}_cxx" PRIVATE CounofNS::CounofNS)
        process_test("${base}_cxx" "${pos}")
    endif()
endfunction()

foreach(h IN LISTS positive_hdr)
    process_hdr("${h}" TRUE "${TAC_HAVE_ADD_DEFINITIONS}")
endforeach()
foreach(h IN LISTS negative_hdr)
    process_hdr("${h}" FALSE "")
endforeach()

enable_testing()
