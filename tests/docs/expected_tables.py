#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Checking tables of expected test case results
"""

import argparse
import re

from markdown_table import Md, MdTable
from compiler_versions import compiler_versions, cv_result

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
        "alx_countof": {
                "": "ALX_COUNTOF",
            },
        "jzmg_array_len": {
                "_cxx": "JZMG_ARRAY_LEN",
                "": "JZMG_ARRAY_LEN",
            },
        "lnx_array_size": {
                "": "LNX_ARRAY_SIZE",
            },
    }
Method = "Method"
ID = "Compiler<br>ID"
Opt = "Opt"
OptSkips = ["e.g.:", "**Hdrs:**"]
    # With the usual layout, the first and last columns are empty.
services = { Method, ID, Opt, "" }
terms = [
        (r'(pos|neg).*\.run_FPE',       '💥<sub>-FPE</sub>'),
        (r'(pos|neg).*\.run_SEGV',      '💥<sub>-SEGV</sub>'),
        (r'(pos|neg).*\.run_DIV0',      '💣<sub>wDIV0</sub>'),
        (r'(pos|neg).*\.build_DIV0',    '💣<sub>wDIV0</sub>'),
        (r'(pos|neg).*\.compiler_bug',  '🪲'),
        (r'pos_.*\.build_fail',         '❌'),
        (r'pos_.*\.run_0_unexpected',   '☸️<sub>Inv0</sub>'),
        (r'pos_.*\.run_fail',           '⚠️<sub>-Inv</sub>'),
        (r'pos_[^.]*(\.c11|\.bltn|)$',  '✅'),
        (r'neg_.*\.run_fail',           '❌<sub>-Inv</sub>'),
        (r'neg_.*\.run_0_unexpected',   '☸️i<sub>Inv0</sub>'),
        (r'neg_.*\.build_unexpected',   '⚠️'),
        (r'neg_.*\.build_fail',         '✅'),
        (r'neg_[^.]*(\.c11|\.bltn|)$',  '**TODO**'),
    ]
FPE = '💥<sub>-FPE</sub>'
SEGV = '💥<sub>-SEGV</sub>'
DIV0 = '💣<sub>-DIV0</sub>'
comments = [
        # Editorial
        'n/a...',
        'n/a',
        'Broken,N/A',
        '⚠️<sub>TODO</sub>',
        '<br>'
    ]

def check_expected(table_fn: str, check_id: str,
                   args: argparse.Namespace) -> bool:
    last_unused = compiler_versions(table_fn, check_id,
                                   args.version, args.extensions, args.verbose)
    res_unused = not (last_unused >= cv_result.last_extensions)
    msg_unused = "warning" if res_unused else "error"
    if args.verbose:
        print(f"{last_unused, res_unused, msg_unused=}")
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
                r[Opt].strip() in OptSkips):
                if args.verbose:
                    print(f"{mt.begin}: info: skip {r[Opt].strip()=}")
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
            for h in mt.headers:
                if h in services:
                    continue
                cell = r[h]
                orig = cell
                cell = re.sub(r'(`|\s)', '', cell)
                for c in comments:
                    cell = cell.replace(c, "")
                r[h] = {'orig': orig, 'cell': cell, 'cnt': 0}
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
        if (not k in args.__dict__ or args.__dict__[k] is None or
            not args.__dict__[k]):
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
                if not ETC_modules[k][m.group(2)] in cxx_ext.id_meth[check_id]:
                    print(f"{k}: not in C++ Extension table"
                      f" {m.group(2), ETC_modules[k][m.group(2)], check_id=}")
                    res = False
                    continue
                rxx_ext = cxx_ext.id_meth[check_id][ETC_modules[k][m.group(2)]]
                if m.group(1) not in cxx_ext.case_hdr:
                    print(f"{ETC_modules[k][m.group(2)]}: {check_id}: error: not test case {m.group(1), ec=}")
                    res = False
                    continue
                cell = rxx_ext[cxx_ext.case_hdr[m.group(1)]]
            else:
                if not ETC_modules[k][m.group(2)] in c_base.id_meth[check_id]:
                    print(f"{k}: not in C Language table"
                      f" {m.group(2), ETC_modules[k][m.group(2)], check_id=}")
                    res = False
                    continue
                if not ETC_modules[k][m.group(2)] in c_ext.id_meth[check_id]:
                    print(f"{k}: not in C Extension table"
                      f" {m.group(2), ETC_modules[k][m.group(2)], check_id=}")
                    res = False
                    continue
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
            for p, s in terms:
                if re.search(p, ec):
                    if not s in cell['orig']:
                        print(f"{ETC_modules[k][m.group(2)]}: {m.group(2)}:"
                              f" {check_id}:"
                              f" error: not result {ec, m.group(1), s, cell=}")
                        res = False
                    else:
                        cell['cell'] = cell['cell'].replace(s, "")
            #else:
            #    print(f"{ETC_modules[k][m.group(2)]}: {check_id}:"
            #          f" Unknown unmatched {ec=}")
            #    res = False
    for mt in c_base, c_ext, cxx_ext:
        for m, r in mt.id_meth[check_id].items():
            for k in args.__dict__:
                if args.__dict__[k] and k in m:
                    break
            else:
                continue
            for h in mt.headers:
                if h in services:
                    continue
                if r[h]['cell']:
                    print(f"{mt.begin} {m}: {check_id}: {msg_unused}:"
                          f" left unused {h, r[h]=}")
                    res &= res_unused
    return res

if __name__ == '__main__':
    import os
    import sys

    sys.stdout.reconfigure(encoding='utf-8')
    sys.stderr.reconfigure(encoding='utf-8')
    parser = argparse.ArgumentParser()
    parser.add_argument("extensions", nargs='+',
                        help="Compilers autoconf extensions", default="")
    parser.add_argument("--table", help="Tables file", nargs='?',
                        default=os.path.join(os.path.dirname(__file__),
                                             "../../docs/comparison_table.md"))
    parser.add_argument("--id", help="ID for check", required=True)
    parser.add_argument("--version", help="Version for check", required=True)
    for k in ETC_modules.keys():
        parser.add_argument(f"--{k}", help=f"{k} expected test cases")
    parser.add_argument("--verbose", action="store_true", help="verbose output")
    args = parser.parse_args()
    res = check_expected(args.table, args.id, args)
    if args.verbose:
        print("Ok" if res else "Fail")
    exit(0 if res else 1)
