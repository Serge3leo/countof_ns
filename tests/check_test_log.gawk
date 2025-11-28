# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

# TODO ctest FIXTURES_CLEANUP and/or:
# https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake
# https://github.com/iboB/icm/blob/master/icm_build_failure_testing.cmake

BEGIN {
    IGNORECASE = 1;
    test_case = "";
    test_case_cnt = 0;
    test_case_flt = 0;
    test_case_flt_msg = "";
    test_case_div = 0;
    test_case_div_msg = "";

    total_case = 0;
    total_div = 0;
    total_flt = 0;
}

/^[0-9]*\/[0-9]* Testing: / {
    if (0 != test_case_cnt) {
        total_case++;
        if (0 == test_case_div) {
            print test_case, ":", test_case_flt_msg;
        } else {
            l = match(test_case_div_msg, "(Divide|Divisi)");
            sub("[[].*$", "", test_case_div_msg)
            sub("is undefined", "", test_case_div_msg);
            print test_case, ":", substr(test_case_div_msg, RSTART);
        }
    }

    test_case = $3;
    test_case_cnt = 0;
    test_case_flt = 0;
    test_case_flt_msg = "";
    test_case_div = 0;
    test_case_div_msg = "";
}

END {
    if (0 != test_case_cnt) {
        total_case++;
        if (0 == test_case_div) {
            print test_case, ":", test_case_flt_msg;
        } else {
            l = match(test_case_div_msg, "(Divide|Divisi)");
            sub("[[].*$", "", test_case_div_msg)
            sub("is undefined", "", test_case_div_msg);
            print test_case, ":", substr(test_case_div_msg, RSTART);
        }
    }

    print "Total float/div test cases:", total_case,
          " (float:", total_flt, ", div:", total_div, "strings)" 
}

/Divide|Divisi/ {
    test_case_cnt++;
    test_case_div++;
    test_case_div_msg = $0;
    total_div++;
}

/Floa|Exceptio/ {
    test_case_cnt++;
    test_case_flt++;
    test_case_flt_msg = $0;
    total_flt++;
}
