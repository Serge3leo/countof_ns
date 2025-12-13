#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Check unique hashs over files
"""

import argparse
import re

def search_hashs(fname: str, verbose: bool) -> set:
    res = set()
    with open(fname, encoding='utf-8') as file:
        for f in re.finditer(r"\s([0-9A-Fa-f]{32,})\s", file.read()):
            res.add(f.group(1))
            if verbose:
                print(f"{fname}: {f.group(1)}")
    if verbose:
        print(f"{fname}: res={res}")
    return res

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("files", nargs='+',
                        help="Files for hash search", default="")
    parser.add_argument("--verbose", action="store_true",
                        help="verbose output")
    args = parser.parse_args()
    hashs = set()
    err = False
    for f in args.files:
        h = search_hashs(f, args.verbose)
        if not h:
            print(f"{f}: error: don't have hash")
            err = True
        hashs |= h
    if args.verbose:
        print(f"{hashs =}")
        print("Ok" if 1 == len(hashs) and not err else "Fail")
    exit(0 if 1 == len(hashs) and not err else 1)
