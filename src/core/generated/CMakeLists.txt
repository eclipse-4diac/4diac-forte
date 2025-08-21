#*******************************************************************************
# Copyright (c) 2025 OFFIS e.V.
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#     OFFIS e.V. - initial rewrite
# *******************************************************************************/

set(FORTE_GENERATED_FILES_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(FORTE_GENERATED_FILES_DIR "${FORTE_GENERATED_FILES_DIR}" PARENT_SCOPE)
cmake_language(DEFER DIRECTORY .. CALL set FORTE_GENERATED_FILES_DIR "${FORTE_GENERATED_FILES_DIR}" PARENT_SCOPE)
target_include_directories(forte-global INTERFACE ${FORTE_GENERATED_FILES_DIR}/..)

function(forte_generate_config_flag_include variable)
    if (${variable})
        file(GENERATE
            OUTPUT "${FORTE_GENERATED_FILES_DIR}/config/${variable}.h"
            CONTENT "#define ${variable}\n"
        )
    else()
        file(GENERATE
            OUTPUT "${FORTE_GENERATED_FILES_DIR}/config/${variable}.h"
            CONTENT "//#undef ${variable}\n"
        )
    endif()
endfunction()

# optional second arg specifies c-syntax value to use instead of the symbol's value
function(forte_generate_config_value_include symbol)
    if (ARGN)
        set(value "${ARGN}")
    else()
        set(value "${${symbol}}")
    endif()

    file(GENERATE
        OUTPUT "${FORTE_GENERATED_FILES_DIR}/config/${symbol}.h"
        CONTENT "#define ${symbol} ${value}\n"
    )
endfunction()

# This doesn't properly escape string contents. configure_file doesn't do that either, so it's propably fine for our use case.
function(forte_generate_config_string_include symbol)
    file(GENERATE
        OUTPUT "${FORTE_GENERATED_FILES_DIR}/config/${symbol}.h"
        CONTENT "#define ${symbol} \"${${symbol}}\"\n"
    )
endfunction()

function(_forte_evaluate_trivial_generator_expressions var)
    # evaluate a few trivial generator expressions, mainly $<BOOL:${FORTE_FOO_ENABLED}> and $<IF:...>
    set(file "${${var}}")
    string(REGEX REPLACE "\\\$<BOOL:(0|OFF|FALSE|)>" "0" file "${file}")
    string(REGEX REPLACE "\\\$<BOOL:[^>]*>" "1" file "${file}")
    string(REGEX REPLACE "\\\$<NOT:(0|OFF|FALSE|)>" "1" file "${file}")
    string(REGEX REPLACE "\\\$<NOT:[^>]*>" "0" file "${file}")
    string(REGEX REPLACE "\\\$<0:([^>]*)>" "" file "${file}")
    string(REGEX REPLACE "\\\$<1:([^>]*)>" "\\1" file "${file}")
    string(REGEX REPLACE "\\\$<IF:0,([^>,]*),([^>,]*)>" "\\2" file "${file}")
    string(REGEX REPLACE "\\\$<IF:1,([^>,]*),([^>,]*)>" "\\1" file "${file}")
    set(${var} "${file}" PARENT_SCOPE)
endfunction()

add_subdirectory(config)
