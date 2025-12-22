#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Check unique hashs over files
"""

import argparse
import os

from markdown_table import Md, MdTable

def compiler_id(table_fn: str, check_id: str, verbose: bool) -> bool:
    comparison_table = Md(table_fn)
    compiler_id = MdTable(comparison_table, "compiler-id")
    r = compiler_id.get(ID=check_id)
    if verbose:
        print(f"{check_id, r=}")
    return len(r["CC"]) > 0

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", help="Tables file", nargs='?',
                        default=os.path.join(os.path.dirname(__file__),
                                             "../../docs/comparison_table.md"))
    parser.add_argument("--id", help="ID for check", required=True)
    parser.add_argument("--verbose", action="store_true", help="verbose output")
    args = parser.parse_args()
    res = compiler_id(args.file, args.id, args.verbose)
    if args.verbose:
        print("Ok" if res else "Fail")
    exit(0 if res else 1)
