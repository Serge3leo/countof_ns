#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Check unique hashs over files
"""

import re
import enum

from markdown_table import Md, MdTable

class cv_result(enum.IntEnum):
    fail = 0
    good = 1
    last_extensions = 2
    last_version = 3

def versiontuple(v):
    return tuple(map(int, (v.split("."))))

def compiler_versions(table_fn: str, check_id: str, check_version: str,
                      check_extensions: list[str], verbose: bool) -> cv_result:
    res = cv_result.good
    comparison_table = Md(table_fn)
    compiler_versions = MdTable(comparison_table,
                                "compiler-versions-and-extensions")
    r = compiler_versions.get(ID=check_id)
    s = {}
    for mm in 'min', 'max':
        t = re.sub('⚛︎.*$', '', r[f'Extensions {mm}'])
        s[mm] = set(re.sub(r'(`|\s)', '', e)
                    for e in t.split('<br>'))
        if '' in s[mm]:
            s[mm].remove('')
    for mm in 'Min', 'Max':
        r[mm] = r[mm].strip()
    if '' == r['Min'] or not s['min']:
        r['Min'] = r['Max']
        s['min'] = s['max']
    if not (versiontuple(r['Min']) <= versiontuple(r['Max'])):
        print(f"error: table versions corrupt {r['Min'], r['Max']=}")
        res = cv_result.fail
    if not (versiontuple(r['Min']) <= versiontuple(check_version)):
        print(f"error: version too low {r['Min'], check_version=}")
        res = cv_result.fail
    if not (versiontuple(r['Max']) >= versiontuple(check_version)):
        print(f"error: version too high {r['Max'], check_version=}")
        res = cv_result.fail
    sext = set(re.sub(r'(`|\s)', '', e)
               for l in check_extensions for e in l.split())
    if not (sext <= (s['min']|s['max'])):
        print(f"error: too more extensions"
              f" {sext-(s['min']|s['max']), (s['min']|s['max'])-sext, sext=}")
        res = cv_result.fail
    if s['max'] == sext and cv_result.fail != res:
        res = cv_result.last_extensions
    if r['Max'] == check_version and cv_result.fail != res:
        res = cv_result.last_version
    if verbose:
        print(f"{r['Min'], check_version, r['Max']=}")
        print(f"{s['min'], sext, s['max']=}")
        print(f"{res=}")
    return res

if __name__ == '__main__':
    import argparse
    import os
    import sys

    sys.stdout.reconfigure(encoding='utf-8')
    sys.stderr.reconfigure(encoding='utf-8')
    parser = argparse.ArgumentParser()
    parser.add_argument("extensions", nargs='+',
                        help="Compilers autoconf extensions", default="")
    parser.add_argument("--file", help="Tables file", nargs='?',
                        default=os.path.join(os.path.dirname(__file__),
                                             "../../docs/comparison_table.md"))
    parser.add_argument("--id", help="ID for check", required=True)
    parser.add_argument("--version", help="Version for check", required=True)
    parser.add_argument("--verbose", action="store_true", help="verbose output")
    args = parser.parse_args()
    res = compiler_versions(args.file, args.id, args.version, args.extensions,
                            args.verbose)
    if args.verbose:
        print("Ok" if res else "Fail")
    exit(0 if res else 1)
