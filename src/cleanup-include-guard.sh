#!/bin/bash

rg -t h -m 1 -A 1 '#ifndef (_.*|.*_H|.*_)$' . | while read line1; do
	read line2
	read line3
	file="${line1%%:*}"
	symbol="${line1##* }"
	if [ "$line2" != "$file-#define $symbol" -o "${line3%--}" != "" ]; then
		echo "$file is malformed"
	else
		linenum="$(grep -n '#endif' "$file" | tail -n 1)"
		linenum="${linenum%%:*}"
		sed -i -e "s/#ifndef $symbol/#pragma once/; /#define $symbol/ d;$linenum d" "$file"
	fi
done
