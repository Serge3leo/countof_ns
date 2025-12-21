#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Checking identical tables of Russian and English translation.
"""

import argparse

from markdown_table import Md, MdTable

def check_tables(file1: str, file2: str, args: argparse.Namespace) -> bool:
    res = True
    md1 = Md(file1)
    md2 = Md(file2)
    for name in ("c", "c++", "methods", "cases", "result",
                 "compiler-versions-and-extensions", "compiler-id"):
        if args.verbose:
             print(f'Compare "{name}"')
        t1 = MdTable(md1, name)
        t2 = MdTable(md2, name)
        if set(t1.keys.keys()) != set(t2.keys.keys()):
            print(f"{name}: keys diff"
                  f" {set(t1.keys.keys()), set(t2.keys.keys())=}")
            res = False
            continue
        for k in t1.keys.keys():
            if set(t1.keys[k].keys()) != set(t2.keys[k].keys()):
                print(f"{name}: keys['{k}'] diff"
                      f" {set(t1.keys[k].keys()), set(t2.keys[k].keys())=}")
                res = False
                continue
            for kv in t1.keys[k].keys():
                if len(t1.keys[k][kv]) != len(t2.keys[k][kv]):
                    print(f"{name}: number row for keys['{k}']['{kv}'] diff"
                          f" {len(t1.keys[k][kv]), len(t2.keys[k][kv])=}")
                    res = False
                    continue
        if len(t1.table) != len(t2.table):
            print(f"{name}: number row diff {len(t1.table), len(t2.table)=}")
            res = False
            continue
        for r1, r2 in zip(t1.table, t2.table):
            for h in set(r1.keys())&set(r2.keys()):
                if r1[h].strip() != r2[h].strip():
                    print(f"{name}: cell diff"
                          f" {h, r1[h].strip(), r2[h].strip(), r1, r2=}")
                    res = False
                    continue
    return res

if __name__ == '__main__':
    import os
    import sys

    sys.stdout.reconfigure(encoding='utf-8')
    sys.stderr.reconfigure(encoding='utf-8')
    parser = argparse.ArgumentParser()
    parser.add_argument("--en", help="English tables file", nargs='?',
                        default=os.path.join(os.path.dirname(__file__),
                                         "../../docs/comparison_table.md"))
    parser.add_argument("--ru", help="Russian tables file", nargs='?',
                        default=os.path.join(os.path.dirname(__file__),
                                         "../../docs/comparison_table.ru.md"))
    parser.add_argument("--verbose", "-v", help="verbose output",
                        action='count', default=0)
    args = parser.parse_args()
    res = check_tables(args.en, args.ru, args)
    if args.verbose:
        print("Ok" if res else "Fail")
    exit(0 if res else 1)
