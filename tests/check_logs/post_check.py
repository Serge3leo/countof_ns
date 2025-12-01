#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""TODO:XXX

"""

import argparse
import os
import pprint
import re
import sys
import tomllib

from junitparser import JUnitXml

def match_any(s: str, *patterns_arrays) -> bool:
    for pa in patterns_arrays:
        for p in pa:
            if re.fullmatch(p, s):
                return True
    return False

class cond:
    def __init__(self, args: "argparse.Namespace", ccfg: map, cond: str):
        self.args = args
        self.cond = cond
        self.must_not = ccfg.get('must_not_' + cond, [])
        self.random = ccfg.get('random_' + cond, [])
        self.stable = ccfg.get('stable_' + cond, [])
        self.unexpected = 0
        self.wanted = 0
        self.matched = 0

    def match(self, case: "junitparser.TestCase"):
        return True

    def extra_print(self, case: "junitparser.TestCase", match: list[tuple]):
        print(f"{case.name}:")
        for m in match:
            print(m[0])

    def check(self, case: "junitparser.TestCase") -> None:
        if m := self.match(case):
            self.matched += 1
            if (match_any(case.name, self.must_not) or
                not match_any(case.name, self.random, self.stable)):
                self.unexpected += 1
                self.extra_print(case, m)
                print(f"{case.name:40} *** Unexpected {self.cond}")
        else:
            if (not match_any(case.name, self.must_not) and
                match_any(case.name, self.stable)):
                self.wanted += 1
                print(f"{case.name:40} ... Don't required {self.cond}")

    def result(self) -> bool:
        return 0 == self.unexpected and 0 == self.wanted

    def report(self) -> str:
        return (f"Unexpected {self.cond}: {self.unexpected}/{self.matched},"
                f" wanted: {self.wanted}")

class failure(cond):
    def __init__(self, args: "argparse.Namespace", ccfg: map):
        super().__init__(args, ccfg, self.__class__.__name__)

    def match(self, case: "junitparser.TestCase"):
        return case.is_failure

    def extra_print(self, case: "junitparser.TestCase", match: None):
        pass

class fpe(cond):
    def __init__(self, args: "argparse.Namespace", ccfg: map):
        super().__init__(args, ccfg, self.__class__.__name__)

    def match(self, case: "junitparser.TestCase"):
        s = str(case.system_out) + str(case.system_err)
        return re.findall("(.*(Floa|Exceptio).*)", s, re.IGNORECASE)

class divide_by_zero_warning(cond):
    def __init__(self, args: "argparse.Namespace", ccfg: map):
        super().__init__(args, ccfg, self.__class__.__name__)

    def match(self, case: "junitparser.TestCase"):
        s = str(case.system_out).replace("-Wno-division-by-zero",
                                         "-Wno-d.....on-by-zero")
        s += str(case.system_err)
        return re.findall("(.*(Divide|Divisi).*)", s, re.IGNORECASE)

class warning(cond):
    def __init__(self, args: "argparse.Namespace", ccfg: map):
        super().__init__(args, ccfg, self.__class__.__name__)

    def match(self, case: "junitparser.TestCase"):
        s = (str(case.system_out).lower()
                .replace("warning: unrecognized #pr",
                         "warn...: unrecognized #pr"))
        s += str(case.system_err)
        return re.findall("(.*( warning: ).*)", s, re.IGNORECASE)

def process_suite(args: "argparse.Namespace",
                  suite: "junitparser.TestSuite") -> bool:
    for c in suite:
        if c.name == "_check_logs_config":
            cfg = tomllib.loads(c.system_out)
            if 'CL_TAG' in cfg and 'CMAKE_C_COMPILER_ID' in cfg:
                break
            print("Error: _check_logs_config don't have "
                  "'CL_TAG'&'CMAKE_C_COMPILER_ID'", file=sys.stderr)
            print(f"{c.system_out=}", file=sys.stderr)
            exit(4)
    else:
        print("Error: don't have _check_logs_config", file=sys.stderr)
        exit(4)
    if args.verbose:
        print(f"Suite _check_logs_config:")
        pprint.pprint(cfg)
        print()
    cl_tag = cfg.get('CL_TAG')
    cmpl_id = cfg.get('CMAKE_C_COMPILER_ID')
    with open(f"{args.dir}/{cmpl_id}.toml", "rb") as f:
        ccfgs = tomllib.load(f)
    if args.verbose:
        print(f"Compiler {cmpl_id} config:")
        pprint.pprint(ccfgs)
        print()
    ccfg = ccfgs['default']
    if 'default' != cl_tag and cl_tag in ccfgs:
        for k, v in ccfgs[cl_tag].items():
            ccfg[k] = ccfg.get(k, []) + v
    skip = ccfg.get('skip', [])
    total, skipped = 0, 0
    conds = []
    for c in failure, fpe, divide_by_zero_warning, warning:
        conds.append(c(args, ccfg))
    for c in suite:
        total += 1
        if match_any(c.name, skip):
            skipped += 1
            if args.verbose:
                print(f"{c.name:40}     Skiped")
            continue
        for cnd in conds:
            cnd.check(c)
    res = True
    for cnd in conds:
        res = res and cnd.result()
    print(f"{'Ok' if res else 'Fail'}, skipped: {skipped} from {total}"
          f" ({cmpl_id}, {cl_tag})")
    for cnd in conds:
        print(f"    {cnd.report()}")
    return res

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--dir",
                        help="directory of compiler ID descriptions",
                        default=os.path.dirname(__file__) + "/expected")
    parser.add_argument("--log",
                        help="JUnit XML log file")
    parser.add_argument("--verbose", action="store_true",
                        help="verbose output")
    parser.add_argument("--selftest", action="store_true",
                        help="check availability")
    args = parser.parse_args()
    if args.selftest:
        exit()
    if not args.log or not args.dir:
        print("Error: Require both dir and log arguments\n", file=sys.stderr)
        parser.print_help()
        exit(3)
    res = True
    for s in JUnitXml.fromfile(args.log):
        res = res and process_suite(args, s)
    exit(0 if res else 1)
