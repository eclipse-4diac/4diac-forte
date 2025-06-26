#!/bin/sh

for i in "$@"; do
	if [ -d "$i" ]; then
		"$0" "$i"/*
	elif grep g_nStringId "$i" > /dev/null; then
		case "$i" in
		*.sh|*~|*.cmake|*.txt) ;;
		*)
			use_statements="$(sed -e 's/g_nStringId/\ng_nStringId/g;s/^[^\n]*\n\?//;s/g_nStringId\([a-zA-Z0-9_]*\)[^\n]*/USE_STRING_ID(\1);\\/g;/^$/d' < "$i" | sort -u)"
			sed -i~ -e "/FORTE_ENABLE_GENERATED_SOURCE_CPP/{N;N;s/.*/\\n${use_statements}n/};s/g_nStringId\\([a-zA-Z0-9_]*\\)/STRID(\\1)/g" "$i";;
		esac
	fi
done
