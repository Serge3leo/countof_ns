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

from junitparser import JUnitXml, TestCase


def match_any(s: str, *patterns_arrays) -> bool:
    for pa in patterns_arrays:
        for p in pa:
            if re.fullmatch(p, s):
                return True
    return False

# class condTestCase (TestCase):
#     with `cond_output` attribute

def condTestCase(case: TestCase) -> "condTestCase":
    case.cond_output = str(case.system_out) + str(case.system_err)
    return case

test_retry = 0
use_re_compile = True # False # True
if use_re_compile:
    pass
    # print(f"Use re.compile(), {test_retry=}")
    # examples/junit-logs/IntelLLVM-verbose.xml
    # Use re.compile(), test_retry=0
    # user	0m7,445s
    # Use re.compile(), test_retry=3
    # user	0m29,409s
    # Our time: (29.4-7.4)/3 = 7.3
else:
    pass
    # print(f"Use re.sub()/re.findall(), {test_retry=}")
    # examples/junit-logs/IntelLLVM-verbose.xml
    # Use re.sub()/re.findall(), test_retry=0
    # user	0m7,476s
    # Use re.sub()/re.findall(), test_retry=3
    # user	0m29,163s
    # Our time: (29.2-7.5)/3 = 7.2

class cond:
    def __init__(self, args: "argparse.Namespace", ccfg: map, cond: str):
        self.args = args
        self.cond = cond
        self.ignore = ccfg.get('ignore_' + cond, [])
        self.must_not = ccfg.get('must_not_' + cond, [])
        self.random = ccfg.get('random_' + cond, [])
        self.stable = ccfg.get('stable_' + cond, [])
        self.unexpected = 0
        self.wanted = 0
        self.matched = 0
        if use_re_compile:
            if self.pattern:
                self.cpattern = re.compile(self.pattern, flags=re.IGNORECASE)
                del self.pattern
            if not self.ignore:
                self.cignore = None
            else:
                self.cignore = re.compile("(" + "|".join(self.ignore) + ")",
                                          flags=re.IGNORECASE)
                del self.ignore


    def match_censorship(self, case: "condTestCase") -> bool:
        if use_re_compile:
            if self.cignore:
                case.cond_output = self.cignore.sub(" :-CENSORED-: ",
                                                    case.cond_output)
        else:
            for ip in self.ignore:
                case.cond_output = re.sub(ip, " :-CENSORED-: ",
                                          case.cond_output, flags=re.IGNORECASE)
        # TODO re.split() ? 
        if use_re_compile:
            m = self.cpattern.findall(case.cond_output)
            case.cond_output = self.cpattern.sub(" :-MATCHED-: ",
                                                 case.cond_output)
        else:
            m = re.findall(self.pattern, case.cond_output, flags=re.IGNORECASE)
            case.cond_output = re.sub(self.pattern, " :-MATCHED-: ",
                                      case.cond_output, flags=re.IGNORECASE)
        return m

    def extra_print(self, case: "condTestCase", match: list[tuple]):
        print(f"{case.name}:")
        for m in match:
            print(m[0] if isinstance(m, tuple) else m)

    def check(self, case: "condTestCase") -> None:
        m = self.match_censorship(case)
        for _ in range(test_retry):
            _1 = self.match_censorship(case)
        if m:
            self.matched += 1
            if (match_any(case.name, self.must_not) or
                not match_any(case.name, self.random, self.stable)):
                self.unexpected += 1
                self.extra_print(case, m)
                print(f"{case.name:40} *** Unexpected {self.cond}")
        else:
            if (not match_any(case.name, self.must_not) and
                not match_any(case.name, self.random) and
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
        self.pattern = None
        super().__init__(args, ccfg, self.__class__.__name__)

    def match_censorship(self, case: "condTestCase"):
        return case.is_failure

    def extra_print(self, case: "condTestCase", match: None):
        pass


class fpe(cond):
    def __init__(self, args: "argparse.Namespace", ccfg: map):
        self.pattern = "(.*(Floa|Exceptio).*)"
        super().__init__(args, ccfg, self.__class__.__name__)

    def match_censorship(self, case: "condTestCase"):
        m = []
        for r in case.result:
            if r.message == 'NUMERICAL':
                m.append(str(r))
        return m + super().match_censorship(case)

class segmentation_fault(cond):
    def __init__(self, args: "argparse.Namespace", ccfg: map):
        self.pattern = r"(.*(segmentation|(^|\s)fault).*)"
        super().__init__(args, ccfg, self.__class__.__name__)

    def match_censorship(self, case: "condTestCase"):
        m = []
        for r in case.result:
            if r.message == 'SEGFAULT':
                m.append(str(r))
        return m + super().match_censorship(case)


class divide_by_zero_warning(cond):
    def __init__(self, args: "argparse.Namespace", ccfg: map):
        self.pattern = "(.*(Divide|Divisi).*)"
        super().__init__(args, ccfg, self.__class__.__name__)


class warning(cond):
    def __init__(self, args: "argparse.Namespace", ccfg: map):
        self.pattern = "(.*( warning: ).*)"
        super().__init__(args, ccfg, self.__class__.__name__)


def process_suite(args: "argparse.Namespace",
                  suite: "junitparser.TestSuite") -> bool:
    for c in suite:
        if c.name == "_check_logs_config":
            if args.verbose:
                print(f"Suite _check_logs_config:")
                print(c.system_out)
                print()
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
    conds = []
    for c in failure, fpe, segmentation_fault, divide_by_zero_warning, warning:
        conds.append(c(args, ccfg))
    total, skipped = 0, 0
    for c in suite:
        total += 1
        c = condTestCase(c)
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
    parser.add_argument("log", nargs='?',
                        help="JUnit XML log file", default="")
    parser.add_argument("--dir",
                        help="directory of compiler ID descriptions",
                        default=os.path.join(os.path.dirname(__file__),
                                             "expected"))
    parser.add_argument("--verbose", action="store_true",
                        help="verbose output")
    parser.add_argument("--selftest", action="store_true",
                        help="check availability")
    args = parser.parse_args()
    if args.selftest:
        exit()
    if not args.log:
        print("Error: Require log file\n", file=sys.stderr)
        parser.print_help()
        exit(3)
    res = True
    for s in JUnitXml.fromfile(args.log):
        res = res and process_suite(args, s)
    exit(0 if res else 1)
