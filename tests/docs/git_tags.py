#!/usr/bin/env python3
# vim:set sw=4 ts=8 et fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Сергей Леонтьев (leo@sai.msu.ru)

"""
Check unique hashs over files
"""

import argparse
import re

def search_hashs_versions(fname: str, verbose: bool) -> tuple[set, set]:
    hpat = r"\s([0-9A-Fa-f]{32,})\s"
    vpat = r"(([0-9]+\.?)+)($|[^0-9])"
    hashs, versions = set(), set()
    with open(fname, encoding='utf-8') as file:
        content = file.read()
        for f in re.finditer(hpat, content):
            hashs.add(f.group(1))
            if verbose:
                print(f"{fname}: found {f.group(1)} at '{f.group(0)}'")
        for f in re.finditer(r"\s*GIT_TAG\s*" + hpat + r"\s*#\s+v" + vpat,
                             content):
            versions.add(f.group(2))
            if verbose:
                print(f"{fname}: found {f.group(2)} at '{f.group(0)}'")
        for f in re.finditer( r"\n\s*VERSION\s+" + vpat, content):
            versions.add(f.group(1))
            if verbose:
                print(f"{fname}: found {f.group(1)} at '{f.group(0)}'")
    res = (hashs, versions)
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
    hashs, versions = set(), set()
    err = False
    for f in args.files:
        h, v = search_hashs_versions(f, args.verbose)
        if not h:
            print(f"{f}: error: don't have hash")
            err = True
        if not v:
            print(f"{f}: error: don't have version")
            err = True
        hashs |= h
        versions |= v
    if args.verbose:
        print(f"{hashs, versions=}")
    if 1 != len(hashs):
        err = True
        print(f"Error: not unique {hashs=}")
    if 1 != len(versions):
        err = True
        print(f"Error: not unique {versions}")
    if args.verbose:
        print("Fail" if err else "Ok")
    exit(err)
