#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Checking tables of expected test case results
"""

import argparse
import os
import re

from markdown_table import Md, MdTable

ETC_modules = {
        "countof_ns": {
                ".c11": "countof_ns<sup>c11</sup>",
                ".bltn": "countof_ns<sup>bltn</sup>",
                "_cxx": "countof_ns<sup>tmpl</sup>",
                "_cxx.bltn": "countof_ns<sup>bltn</sup>",
                "": "countof_ns<sup>gen</sup>",
            },
        "ms_countof": {
                "_cxx": "ms_countof",
                "": "ms_countof",
            },
        "std_size": {
                "_cxx": "std::size",
            },
        "stdc_countof": {
                "": "countof",
            },
        "ALX_COUNTOF": {
                "": "COUNTOF",
            },
        "JZMG_ARRAY_LEN": {
                "_cxx": "ARRAY_LEN",
                "": "ARRAY_LEN",
            },
        "LNX_ARRAY_SIZE": {
                "": "ARRAY_SIZE",
            },
    }
Method = "Method"
ID = "Compiler<br>ID"
Opt = "Opt"
ExampleSkip = "e.g.:"
    # With the usual layout, the first and last columns are empty.
services = { Method, ID, Opt, "" }
terms = [
        (r'pos_.*\.build_fail',         '❌'),
        (r'pos_.*\.run_0_unexpected',   '☸️'),
        (r'pos_.*\.run_fail',           '⚠️'),
        (r'pos_.*',                     '✅'),
        (r'neg_.*\.run_fail',           '❌'),
        (r'neg_.*\.run_0_unexpected',   '☸️'),
        (r'neg_.*\.build_unexpected',   '⚠️'),
        (r'neg_.*\.build_fail',         '✅'),
        (r'neg_.*',                     '**TODO**'),
    ]
FPE = '💥<sub>-FPE</sub>'
SEGV = '💥<sub>-SEGV</sub>'
DIV0 = '💣<sub>-DIV0</sub>'
comments = [
        # Editorial
        'n/a',
        'Broken, N/A',
        '⚠️<sub>TODO</sub>',
        # Unimplemented
        FPE,  # TODO need check by logs
        SEGV,  # TODO need check by logs
        DIV0,  # TODO need check by logs
        # Informal
        '<sub>+Cmpl</sub>',
        '<sub>-Inv</sub>',
        '<sub>+Expect</sub>',
        '<sub>+Inv0</sub>',
    ]

def check_expected(table_fn: str, check_id: str, args: "Namespace") -> bool:
    res = True
    comparison_table = Md(table_fn)
    c_base = MdTable(comparison_table,
                     "## C language standard",
                     "## Extensions of C language")
    c_ext = MdTable(comparison_table,
                    "## Extensions of C language",
                    "## Extensions of C++ language")
    cxx_ext = MdTable(comparison_table,
                    "## Extensions of C++ language",
                    "## Terms and definitions")
    if args.verbose:
        for mt in c_base, c_ext, cxx_ext:
            print(f"{mt.begin, mt.header_len, len(mt.table)}")
            print(mt.table[0])
            print(mt.table[1])
            print(f"~~~~  End fragment {mt.begin} ~~~~")
        print("~~~~~~~~~~~~")
    for mt in c_base, c_ext, cxx_ext:
        #print(f"{mt.begin}: info: {mt.headers, mt.table=}")
        # Add key (ID, Method) -> row
        mt.id_meth = {}
        last = ""
        for r in mt.table:
            for _, c in r.items():
                if c.strip():
                    break
            else:
                if args.verbose:
                    print(f"{mt.begin}: info: skip empty")
                continue
            if ("" == r[Method].strip() and "" == r[ID].strip() and
                ExampleSkip == r[Opt].strip()):
                if args.verbose:
                    print(f"{mt.begin}: info: skip {ExampleSkip=}")
                continue
            meth = r[Method].strip()
            if "" == meth:
                meth = last
            else:
                last = meth
            cid = r[ID].strip()
            if not cid in mt.id_meth:
                mt.id_meth[cid] = {}
            kid = mt.id_meth[cid]
            #print(f"{mt.begin}: {meth, cid, kid, r=}")
            if meth in kid:
                print(f"{mt.begin}: error: duplicate {meth, cid=}")
                res = False
            kid[meth] = r
        # Add key <base name test case> -> columns header
        mt.case_hdr = {}
        for h in mt.headers:
            if h in services:
                continue
            for bnc in [re.sub(r'(`|\s)', '', c) for c in h.split('<br>')]:
                #print(f"{mt.begin}: info: {bnc, h=}")
                if "" == bnc:
                    if args.verbose:
                        print(f"{mt.begin}: info: skip empty {bnc, h=}")
                    continue
                if bnc in mt.case_hdr:
                    print(f"{mt.begin}: error: duplicate {bnc, h=}")
                    res = False
                mt.case_hdr[bnc] = h
    for k in ETC_modules.keys():
        if not k in args.__dict__ or args.__dict__[k] is None:
            if args.verbose:
                print(f"not '{k}' in args.__dict__")
            continue
        ...  # Выбор из ETC_modules[k]
        for ec in args.__dict__[k].split(';'):
            m = re.match(r"([^.]*)(\.c11|\.bltn|)($|\..*?)", ec)
            if not m:
                print(f"{k}: unrecognized case: {ec, m=}")
                res = False
                continue
            elif m.group(1).endswith("_cxx"):
                m = re.match(r"([^.]*)(_cxx.bltn|_cxx)($|\..*)", ec)
                if not m:
                    print(f"{k}: unrecognized case: {ec, m=}")
                    res = False
                    continue
                rxx_ext = cxx_ext.id_meth[check_id][ETC_modules[k][m.group(2)]]
                if m.group(1) not in cxx_ext.case_hdr:
                    print(f"{ETC_modules[k][m.group(2)]}: {check_id}: error: not test case {m.group(1), ec=}")
                    res = False
                    continue
                cell = rxx_ext[cxx_ext.case_hdr[m.group(1)]]
            else:
                r_base = c_base.id_meth[check_id][ETC_modules[k][m.group(2)]]
                r_ext = c_ext.id_meth[check_id][ETC_modules[k][m.group(2)]]
                if m.group(1) in c_base.case_hdr:
                    if m.group(1) in c_ext.case_hdr:
                        print(f"{k}: error: duplicate case"
                            f" {m.group(1), c_base.case_hdr, c_ext.case_hdr=}")
                        res = False
                    if not c_base.case_hdr[m.group(1)] in r_base:
                        print(f"{k}: {ec, m.group(1),
                              c_base.case_hdr[m.group(1)], r_base=}")
                        res = False  # TODO
                        continue
                    cell = r_base[c_base.case_hdr[m.group(1)]]
                elif m.group(1) in c_ext.case_hdr:
                    cell = r_ext[c_ext.case_hdr[m.group(1)]]
                else:
                    print(f"{k}: error: case not found"
                          f" {m.group(1), m.group(2),
                          ETC_modules[k][m.group(2)], ec=}")
                    res = False
                    continue
            cell = re.sub(r'(`|\s)', '', cell)
            for c in comments:
                cell = cell.replace(c, "")
            for p, s in terms:
                if re.match(p, ec):
                    if not s in cell:
                        # TODO print(f"{p, s=}")
                        print(f"{ETC_modules[k][m.group(2)]}: {check_id}:"
                              f" error: not result {ec, m.group(1), s, cell=}")
                        res = False
                    break
                #else:
                #    print(f"Not match {p, s=}")
            ...  # TODO: all cell used
    return res

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--table", help="Tables file", nargs='?',
                        default=os.path.join(os.path.dirname(__file__),
                                             "../../docs/comparison_table.md"))
    parser.add_argument("--id", help="ID for check", required=True)
    for k in ETC_modules.keys():
        parser.add_argument(f"--{k}", help=f"{k} expected test cases")
    parser.add_argument("--verbose", action="store_true", help="verbose output")
    args = parser.parse_args()
    res = check_expected(args.table, args.id, args)
    if args.verbose:
        print("Ok" if res else "Fail")
    exit(0 if res else 1)
