#!/bin/bash
set -e

SRC_DIR="src"

cd "$(git rev-parse --show-toplevel)"
find "$SRC_DIR" -type f -regex '.*\.\(c\|cpp\|cc\|h\|hpp\)' -exec clang-format --dry-run --Werror "{}" +