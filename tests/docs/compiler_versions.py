#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Check unique hashs over files
"""

import argparse
import os
import re

from markdown_table import Md, MdTable

def compiler_versions(table_fn: str, check_id: str, check_version: str,
                      check_extensions: list[str], verbose: bool) -> bool:
    res = True
    comparison_table = Md(table_fn)
    compiler_versions = MdTable(comparison_table,
                                "### Compiler versions and extensions",
                                "### Compiler ID")
    for r in compiler_versions.table:
        if r['ID'].strip() == check_id:
            break
    else:
        print(f"error: not found {check_id=}")
        return False
    s = {}
    for mm in 'min', 'max':
        t = re.sub('⚛︎.*$', '', r[f'Extensions {mm}'])
        s[mm] = set(re.sub(r'(`|\s)', '', e)
                    for e in t.split('<br>'))
        if '' in s[mm]:
            s[mm].remove('')
    if not (s['min'] <= s['max']):
        print(f"error: table extensions corrupt {s['min'], s['max']=}")
        res = False
    for mm in 'Min', 'Max':
        r[mm] = r[mm].strip()
    if '' == r['Min'] or not s['min']:
        r['Min'] = r['Max']
        s['min'] = s['max']
    if not (r['Min'] <= r['Max']):
        print(f"error: table versions corrupt {r['Min'], r['Max']=}")
        res = False
    if not (r['Min'] <= check_version):
        print(f"error: version too low {r['Min'], check_version=}")
        res = False
    if not (r['Max'] >= check_version):
        print(f"error: version too high {r['Max'], check_version=}")
        res = False
    sext = set(re.sub(r'(`|\s)', '', e)
               for l in check_extensions for e in l.split())
    if not (s['min'] <= sext):
        print(f"error: too few extensions {s['min'], sext=}")
        res = False
    if not (s['max'] >= sext):
        print(f"error: too more extensions {s['max'], sext=}")
        res = False
    if verbose:
        print(f"{r['Min'], check_version, r['Max']=}")
        print(f"{s['min'], sext, s['max']=}")
    return res

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("extensions", nargs='+',
                        help="Compilers autoconf extensions", default="")
    parser.add_argument("--table", help="Tables file", nargs='?',
                        default=os.path.join(os.path.dirname(__file__),
                                             "../../docs/comparison_table.md"))
    parser.add_argument("--id", help="ID for check", required=True)
    parser.add_argument("--version", help="Version for check", required=True)
    parser.add_argument("--verbose", action="store_true", help="verbose output")
    args = parser.parse_args()
    res = compiler_versions(args.table, args.id, args.version, args.extensions,
                            args.verbose)
    if args.verbose:
        print("Ok" if res else "Fail")
    exit(0 if res else 1)
