# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

message("TAC_DEFINITIONS=${TAC_DEFINITIONS}\n"
        "TAC_ADD_DEFINITIONS=${TAC_ADD_DEFINITIONS}\n"
        "TAC_ADD_C_DEFINITIONS=${TAC_ADD_C_DEFINITIONS}\n"
        "TAC_HAVE_ADD_DEFINITIONS=${TAC_HAVE_ADD_DEFINITIONS} ")

add_definitions(#${TAC_HAVE_ADD_DEFINITIONS}  # TODO: remove for EXAMPLE_FAIL
                ${TAC_ADD_DEFINITIONS}
                ${TAC_DEFINITIONS})
foreach(t IN ITEMS short_example short_example_cxx  # TODO function?
                   long_example long_example_vla long_example_cxx)
    if((NOT ${t} MATCHES "_vla$" OR "${TAC_DEFINITIONS}" MATCHES "HAVE_VLA")
       AND (NOT ${t} MATCHES "_cxx$" OR NOT "${TAC_SKIP_CXX}"))
        list(APPEND tgts "${t}" "${t}_fail")
    endif()
endforeach()
foreach(t IN ITEMS ${tgts})
    string(REGEX REPLACE "_fail$" "" s "${t}")
    string(REGEX REPLACE "_vla$" "" s "${s}")
    string(REGEX REPLACE "_cxx$" "" h "${s}")
    if("${t}" MATCHES "_cxx")
        add_executable("${t}" "${s}.cpp" "${h}.h")
        target_compile_options("${t}" PRIVATE "${TAC_ADD_CXX_DEFINITIONS}")
    else()
        add_executable("${t}" "${s}.c" "${h}.h")
        target_compile_options("${t}" PRIVATE "${TAC_ADD_C_DEFINITIONS}")
    endif()
    if("${t}" MATCHES "_vla")
        target_compile_options("${t}" PRIVATE "-DENABLE_VLA_EXAMPLE")
    endif()
    if("${t}" MATCHES "_fail")
        target_compile_options("${t}" PRIVATE "-DEXAMPLE_FAIL")
        set_target_properties("${t}" PROPERTIES EXCLUDE_FROM_ALL TRUE)
    else()
        target_compile_options("${t}" PRIVATE ${TAC_HAVE_ADD_DEFINITIONS})
    endif()
    target_link_libraries("${t}" PRIVATE CounofNS::CounofNS)
endforeach()
