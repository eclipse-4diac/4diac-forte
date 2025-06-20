#!/bin/bash
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

IGNORE_FILE=".clang-format-ignore"
SRC_DIR="src"

ALL_FILES=$(find "$SRC_DIR" -type f -regex '.*\.\(c\|cpp\|cc\|h\|hpp\)$')

if [[ -f "$IGNORE_FILE" ]]; then
    mapfile -t IGNORE_PATTERNS < "$IGNORE_FILE"
else
    IGNORE_PATTERNS=()
fi

is_ignored() {
    local file=$1
    for pattern in "${IGNORE_PATTERNS[@]}"; do
        if [[ "$file" == $pattern ]] || [[ "$file" == ./"$pattern" ]]; then
            return 0
        fi
        if [[ "$file" == $pattern ]]; then
            return 0
        fi
    done
    return 1
}

CHECK_FILES=()
for file in $ALL_FILES; do
    if ! is_ignored "$file"; then
        CHECK_FILES+=("$file")
    fi
done

if [[ ${#CHECK_FILES[@]} -eq 0 ]]; then
    echo "✅ No files to check"
    exit 0
fi

echo "🔍 Checking formatting on ${#CHECK_FILES[@]} files..."
clang-format --dry-run --Werror "${CHECK_FILES[@]}"