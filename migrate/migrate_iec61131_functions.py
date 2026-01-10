#!/usr/bin/env python3
#############################################################################
# Copyright (c) 2026 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

import glob
import os
import re
import sys
from typing import Any

SOURCE_EXTENSIONS = ('.h', '.cpp', '.hpp')
EXCLUDED_DIRS = ('.git', 'build', 'iec61131_functions')
EXCLUDED_FILES = ('forte_st_util.h', 'iec61131_functions.h')

FUNC_REGEX = re.compile(r'\b(func_[A-Za-z0-9_]+)\b')
INCLUDE_REGEX = re.compile(r'#include\s+["<]')
FORTE_INCLUDE_REGEX = r'#include\s+["<]forte/(.*)[">]'
IEC61131_FUNCTIONS_INCLUDE_REGEX = re.compile(r'#include\s+["<](forte/)?iec61131_functions\.h[">]')
ST_UTIL_SYMBOLS = [
    'ST_IGNORE_OUT_PARAM',
    'ST_EXTEND_LIFETIME',
    'COutputParameter',
    'CAnyBitOutputParameter',
    'COutputGuard'
]
ST_UTIL_PATTERN = re.compile(r'\b(' + '|'.join(ST_UTIL_SYMBOLS) + r')\b')


def migrate_folder(target_folder, functions_include_dir):
    available_functions = _find_available_functions(functions_include_dir)

    for root, dirs, files in os.walk(target_folder):
        if any(excluded in root for excluded in EXCLUDED_DIRS):
            continue

        for filename in files:
            if filename.endswith(SOURCE_EXTENSIONS) and filename not in EXCLUDED_FILES:
                file_path = os.path.join(root, filename)
                _process_file(file_path, available_functions)


def _find_available_functions(functions_include_dir) -> set[str]:
    header_pattern = os.path.join(functions_include_dir, "func_*.h")
    available_functions = {os.path.splitext(os.path.basename(f))[0] for f in glob.glob(header_pattern)}
    print(f"Found {len(available_functions)} available function headers.")
    return available_functions


def _process_file(file_path, available_functions):
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    # extract all existing forte includes
    existing_includes = set(re.findall(FORTE_INCLUDE_REGEX, content))
    has_legacy_include = 'iec61131_functions.h' in existing_includes
    used_functions = set(FUNC_REGEX.findall(content)) & available_functions
    used_st_util = bool(ST_UTIL_PATTERN.search(content))

    # if neither ic61131_functions.h is included, nor any functions/st_util are used, skip
    if not any([has_legacy_include, used_functions, used_st_util]):
        return

    additional_includes = _get_additional_includes(used_functions, used_st_util, existing_includes)
    if not additional_includes:
        return

    original_lines = content.splitlines(keepends=True)
    new_lines = _generate_new_lines(original_lines, additional_includes, has_legacy_include, file_path)
    _write_file_if_changed(file_path, original_lines, new_lines)


def _get_additional_includes(used_functions: set[Any], used_st_util: bool, existing_includes: set[str]) -> list[Any]:
    includes = [
        f'#include "forte/iec61131_functions/{func_name}.h"\n'
        for func_name in used_functions
        if f'iec61131_functions/{func_name}.h' not in existing_includes
    ]

    if used_st_util and 'forte_st_util.h' not in existing_includes:
        includes.append('#include "forte/forte_st_util.h"\n')

    includes.sort()
    return includes


def _find_last_include_index(lines: list[str]) -> int:
    last_idx = -1
    for i, line in enumerate(lines):
        if INCLUDE_REGEX.search(line):
            last_idx = i
    return last_idx


def _generate_new_lines(lines: list[str], additional_includes: list[Any], has_legacy_include: bool,
                        file_path: str) -> list[Any]:
    if has_legacy_include:
        result = []
        for line in lines:
            if IEC61131_FUNCTIONS_INCLUDE_REGEX.search(line):
                result.extend(additional_includes)
            else:
                result.append(line)
        return result

    last_include_idx = _find_last_include_index(lines)
    if last_include_idx == -1:
        print(f"Skipping {file_path}: no existing includes")
        return lines

    return lines[:last_include_idx + 1] + additional_includes + lines[last_include_idx + 1:]


def _write_file_if_changed(file_path, original_lines: list[str], new_lines: list[Any]):
    if new_lines != original_lines:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.writelines(new_lines)
        print(f"Updated {file_path}")


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python migrate_iec61131_functions.py <target_folder> <path/to/forte/iec61131_functions>")
        sys.exit(1)

    migrate_folder(sys.argv[1], sys.argv[2])
