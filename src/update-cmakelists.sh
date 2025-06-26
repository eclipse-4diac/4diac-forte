#!/bin/bash
nl="
"

lc() {
	echo -n "${1@L}"
}
trim() {
	local result="${1#"${1%%[![:space:]]*}"}"
	echo -n "${result%"${1##*[![:space:]]}"}"
}
trimcomment() {
	echo -n "$(trim "${1%%#*}")"
}
push_out() {
	output="$output$nl$*"
}
last_line() {
	echo "${output##*$nl}"
}
before_last_line() {
	local tmp="${output%$nl*}"
	echo "${tmp##*$nl}"
}
pop_out() {
	output="${output%$nl*}"
}
is_last_comment() {
	[[ "$(last_line)" =~ ^\ *\# ]]
}
is_before_last_comment() {
	[[ "$(before_last_line)" =~ ^\ *\# ]]
}
add_source() {
	local file="$1"
	if [ "${lastif:0:1}" = "!" ]; then
		file="\$<\$<NOT:\$<BOOL:\${${lastif#!}}>>:$file>"
	elif [ -n "$lastif" ]; then
		file="\$<\$<BOOL:\${$lastif}>:$file>"
	fi
	sources="$sources    ${file}$nl"
}
cmake_arg() {
	local args="${line#*(}" n="$1" newargs lastarg
	args="$(trim "${args%#*}")"
	args="${args%)} "
	while ((n > 0)); do
		args="$(trim "$args") "
		lastarg=""
		case "$args" in
			\"*) newargs="${args#\"*\" }";;
			\'*) newargs="${args#\'*\' }";;
			*) newargs="${args#* }";;
		esac
		lastarg="$(trim "${args:0:$((${#args}-${#newargs}))}")"
		args="$newargs"
		((n -= 1))
	done
	echo "$lastarg"
}

find "$@" -name CMakeLists.txt | while read file; do
	library="${file%/CMakeLists.txt}"
	library="forte-${library//\//-}"
	{ cat "$file"; echo; } | {
		skiplines="~~~~~~~~~~~~~~~~~~~~"
		while IFS="" read -r line; do
			normline="${line@L}"
			normline="$continueprefix$(trim "$normline")"
			args="${line#*(}"
			args="${args%)*}"
###################################

case "$normline" in
	add_library?forte-* | forte_register_* | set?forte_generated_files_dir* | function?forte_add_modules* | function?forte_set_process_interface* | function?forte_register_* | function?_forte_* | function?forte_opcua_register* )
		echo -e "\\rFile seems processed: $file" >&2;
		exit 0;;

	# manually refactored
	"file(generate")
		push_out "$line"
		skiplines=""
		skip=3;;

	"${skiplines}"*)
		push_out "$line"
		((skip -= 1))
		((skip > 0)) || skiplines="~~~~~~~~~~~~~~~~~~";;

	# changed call
	"forte_add_handler("*)
		push_out "forte_register_handler($(cmake_arg 1) $(cmake_arg 2).h)";;

	# changed call
	"forte_add_startup_hook("*)
		push_out "forte_register_startup_hook($(cmake_arg 1))";;

	# changed call
	"forte_add_ecet("*)
		push_out "forte_register_ecet($(cmake_arg 1) $(cmake_arg 2).h)";;

	# changed call
	"forte_opcua_add_type("*)
		push_out "forte_opcua_register_type($(cmake_arg 1) $(cmake_arg 2) $(cmake_arg 3))";;

	# changed call
	"forte_opcua_add_namespace("*)
		push_out "forte_opcua_register_namespace($(cmake_arg 1) $(cmake_arg 2))";;

	# changed call
	"forte_add_device("*)
		push_out "forte_register_device($(cmake_arg 1))";;

	# changed call
	"forte_add_io("*)
		push_out "forte_register_io($(cmake_arg 1) OFF $(cmake_arg 2))"
		removebool="FORTE_IO_$(cmake_arg 1)"
		removebool="${removebool@L}";;

	"if(${removebool})"* | "if (${removebool})"* \
	| "endif(${removebool:-!})" | "endif (${removebool:-!})") ;;

	# changed call
	"forte_add_module("*)
		push_out "${line/_add_/_register_}"
		removebool="FORTE_MODULE_$(cmake_arg 1)"
		removebool="${removebool@L}";;

	# changed call
	"forte_add_timerhandler("*)
		push_out "forte_register_timerhandler($(cmake_arg 1) $(cmake_arg 2).h)";;

	# changed call
	"forte_add_network_layer("*)
		push_out "forte_register_com_layer($(cmake_arg 1) $(cmake_arg 2) $(cmake_arg 3) $(cmake_arg 4) $(cmake_arg 5).h $(cmake_arg 6))"
		lastif="FORTE_COM_$(cmake_arg 1)"
		add_source "$(cmake_arg 5).cpp"
		add_source "$(cmake_arg 5).h"
		lastif=""
		boolopt="FORTE_COM_$(cmake_arg 1)"
		boolopt="${boolopt@L}";;

	# moved to an entirely different mechanism
	# TODO: corresponding source code refactoring
	"set(forte_build_"*) ;;

	"forte_add_definition(-dwpcap)" \
	| "forte_add_definition(\"-dua_"* \
	| "forte_add_definition(-dforte_"* \
	| "forte_add_definition(\"-dforte_"*)
		arg="$(cmake_arg 1)"
		arg="${arg#\"}"
		arg="${arg%\"}"
		arg="${arg#-D}"
		push_out "target_compile_definitions(forte-global INTERFACE $arg)";;

	# transform to option() and different source guard
	"set(forte_"*" cache bool"*)
		push_out "option($(cmake_arg 1) $(cmake_arg 5) $(cmake_arg 2))"
		if [ -z "$lastif" ]; then
			boolopt="$(cmake_arg 1)"
			boolopt="${boolopt@L}"
		fi;;

	"if(${boolopt})"* | "if (${boolopt})"* )
		push_out "if (NOT ${boolopt@U})"
		push_out "    return()"
		push_out "endif()"
		push_out "";;

	"endif(${boolopt:-!})" | "endif (${boolopt:-!})")
		boolopt="";;

	# architecture guards
	"if(\"\${forte_architecture}\" strequal "* | "if (\"\${forte_architecture}\" strequal "*)
		archopt="$(cmake_arg 3)"
		push_out "if (NOT FORTE_ARCHITECTURE STREQUAL $archopt)"
		push_out "    return()"
		push_out "endif()"
		push_out "";;

	"endif(\"\${forte_architecture}\" strequal "* | "endif (\"\${forte_architecture}\" strequal"* ) ;;

	# record conditional
	"if(forte_"* | "if (forte_"* | "if("*"_found)" | "if ("*"_found)" )
		[[ $normline =~ \(.*\  ]] || lastif="$(cmake_arg 1)"
		push_out "${line/if(/if (}";;

	"endif()" )
		if [ -n "$boolopt" ]; then
			boolopt=""
		elif [ "$(trim "$(last_line)")" = "if ($lastif)" ]; then
			pop_out
		else
			push_out "$line"
		fi
		lastif="";;

	"endif($(lc "${lastif#!}"))" )
		if [ "$(trim "$(last_line)")" = "if ($lastif)" ]; then
			pop_out
		else
			push_out "$line"
		fi
		lastif="";;

	"else()")
		push_out "$line"
		lastif="${lastif+!}$lastif";;

	# fix space
	'if('*)
		push_out "${line/(/ (}";;

	# fix space
	'endif ()')
		lastif=""
		push_out "${line/ (/(}";;


	# keep as-is
	'#'* \
	| '' \
	| "set("*" cache string"* \
	| "set("*" cache path"* \
	| "set("*" cache filepath"* \
	| "set(forte_trace_"*"cmake_current_source_dir"* \
	| "configure_file"*"forte_generated_files_dir"* \
	| "message(fatal_error"* \
	| "message(status"* \
	| "message("*deprecated* \
	| forte_set_process_interface* \
    | forte_generate_config_* \
    | "find_program("* \
    | "add_dependencies("* \
    | "target_include_directories("* \
	| add_subdirectory* \
	| add_custom_target* \
	| mark_as_advanced* \
	| include\(* \
	| find_library\(* \
	| find_package\(* \
	| pkg_check_modules\(* \
	| option\(* \
	| return\(\) \
	| elseif\ \(* \
	| if\ \(* )
		push_out "$line";;

	# remove
	'forte_add_include_directories(${cmake_current_source_dir})'* \
	| set\(source_group* )
		push_out "";;

	# transform into target include setting
	'forte_add_include_directories('* \
	| 'forte_add_include_system_directories('*)
		includedirs="$includedirs $(cmake_arg 1)";;

	# transform into target include setting
	'forte_add_link_directories('*)
		linkdirs="$linkdirs $(cmake_arg 1)";;

	# transform to library
	forte_add_sourcefile*)
		is_last_comment && ! is_before_last_comment && sources="$sources$(last_line)$nl" && pop_out
		tline="$(trimcomment "$continueprefix$line")"
		ext="${tline%%(*}"
		ext="${ext##*_}"
		sfiles="${tline#*(}"
		sfiles="${sfiles%)*}"
		for sfile in $sfiles; do
			case "$ext" in
			h) add_source "${sfile%.$ext}.$ext";;
			c|cpp) add_source "${sfile%.$ext}.$ext"; has_source=1;;
			hcpp) add_source "$sfile.h"; add_source "$sfile.cpp"; has_source=1;;
			hc) add_source "$sfile.h"; add_source "$sfile.c"; has_source=1;;
			esac
		done
		if [[ $tline =~ \)$ ]]; then
			continueprefix=""
		else
			continueprefix="${normline%%(*}("
		fi
		;;

	# no special handling anymore
	forte_add_subdirectory*)
		push_out "${line/forte_/}";;

	# no special handling anymore
	forte_add_link_library*)
		push_out "target_link_libraries(forte PUBLIC ${line#*(}";;

	*) errors="$errors${nl}    Unmatched: $line";;
esac

###################################
		done
		if [ -n "$sources" ]; then
			push_out "add_library($library OBJECT"
			push_out "$sources)"
			[ -z "$has_source" ] && push_out "set_property(TARGET $library PROPERTY LINKER_LANGUAGE CXX)"
			push_out "target_link_libraries(forte PRIVATE $library)"
		fi
		for dir in $includedirs; do
			push_out "target_include_directories($library PUBLIC $dir)"
		done
		for dir in $linkdirs; do
			push_out "target_link_directories($library PUBLIC $dir)"
		done
		output="${output//$nl$nl$nl/$nl}"
		if [ -n "$errors" ]; then
			echo "ERRORS in $file:$errors"
		else
			echo -ne '\r'
			echo -n "$file"
			echo -ne '\e[K'
			echo "${output#$nl}" > "$file"
			cmake-format \
				--tab-size 4 \
				--separate-ctrl-name-with-space true \
				--separate-fn-name-with-space false \
				--dangle-parens true \
				--enable-sort true \
				--line-width 78 \
			  	--min-prefix-chars 40 \
			  	--max-subgroups-hwrap 2 \
				"$file" -o "$file.new" && mv "$file.new" "$file"
		fi
	}
done
echo
