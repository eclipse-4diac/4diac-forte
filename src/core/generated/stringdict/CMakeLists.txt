function(_forte_generate_stringlist)
    forte_generate_stringlist(forte "${FORTE_GENERATED_FILES_DIR}")
endfunction()

function(forte_generate_stringlist target outputdir)
    forte_find_sources_recursive(source_files "${target}")

    set(strings)
    foreach (source IN LISTS source_files)
        file(STRINGS "${source}" use_statements REGEX "USE_STRING_ID\\([a-zA-Z_0-9]*\\)")
        foreach (use IN LISTS use_statements)
            string(REGEX REPLACE ".*USE_STRING_ID\\(([a-zA-Z0-9_]*)\\).*" "\\1" use "${use}")
            list(APPEND strings "${use}")
        endforeach()
    endforeach()

    list(REMOVE_DUPLICATES strings)
    list(SORT strings)
    list(REMOVE_ITEM strings "")

    set(entries "")
    set(string_buf ".")
    foreach (string IN LISTS strings)
        string(LENGTH "${string_buf}" pos)
        string(APPEND entries "ENTRY(${string},${pos}) \\\n")
        string(APPEND string_buf "${string}.")
    endforeach()

    configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/stringdict_generated.h.in" "${outputdir}/stringdict_generated.h")
endfunction()

cmake_language(DEFER DIRECTORY ../../.. CALL cmake_language DEFER DIRECTORY . CALL _forte_generate_stringlist)
