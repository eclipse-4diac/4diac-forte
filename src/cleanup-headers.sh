#!/bin/bash

find arch core com/opc_ua stdfblib/ita -name \*.h | sed -e 's,\(.*/\), \1 ,' | sort -k 2 | uniq -c -f 1 | grep ' 1 ' | grep -v 'arch/[^/]*/' | while read count path file; do
	rg -c "#include[ 	]*[\"<]([^\">]*/)?$file[\">]" . ../tests | while read source; do
		sed -i -e "s,#include[ 	]*[\"<]\([^\">]*/\)\?$file[\">],#include \"$path$file\"," "${source%:*}"
	done
done

find core com/opc_ua -name \*.h.in | sed -e 's,\(.*/\), \1 ,' | sort -k 2 | uniq -c -f 1 | grep ' 1 ' | while read count path file; do
	file="${file%.in}"
	path="generated/"
	rg -c "#include[ 	]*[\"<]([^\">]*/)?$file[\">]" . ../tests | while read source; do
		sed -i -e "s,#include[ 	]*[\"<]\([^\">]*/\)\?$file[\">],#include \"$path$file\"," "${source%:*}"
	done
done
