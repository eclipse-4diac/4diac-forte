#*******************************************************************************
# Copyright (c) 2011 Profactor GmbH and ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Gerhard Ebenhofer and Ingo Hegny - initial API and implementation and/or initial documentation
# *******************************************************************************/

MESSAGE("Generate Initfunction")
MESSAGE("Source Dir: ${FORTE_SOURCE_DIR}")
MESSAGE("Binary Dir: ${FORTE_BINARY_DIR}")
FILE(STRINGS ${CMAKE_BINARY_DIR}/../file_list.txt FILE_NAMES)

#######################################################################################
# Create Forte-Executeable with all Functionblocks
#######################################################################################

FOREACH(FILE_NAME ${FILE_NAMES})
  # Do not parse typelib, as it holds the definitions
  STRING(REGEX MATCH  ".*typelib\\.(cpp|h).*" REGEX_STRINGS ${FILE_NAME})
  IF(NOT REGEX_STRINGS)
    STRING(REGEX MATCH  ".*forteinit\\.(cpp|h).*" REGEX_STRINGS ${FILE_NAME})
  ENDIF(NOT REGEX_STRINGS)
  IF(NOT REGEX_STRINGS)
    STRING(REGEX MATCH  ".*stringlist\\.(cpp|h).*" REGEX_STRINGS ${FILE_NAME})
  ENDIF(NOT REGEX_STRINGS)
  IF(NOT REGEX_STRINGS)
    STRING(REGEX MATCH  ".*startuphook\\.(cpp|h).*" REGEX_STRINGS ${FILE_NAME})
  ENDIF(NOT REGEX_STRINGS)
  
  IF(NOT REGEX_STRINGS)
    FILE(READ ${FILE_NAME} FILE_CONTENT)
    STRING(REGEX MATCHALL "DEFINE_FIRMWARE_DATATYPE\\(([A-Za-z0-9_]*)" REGEX_STRINGS ${FILE_CONTENT})
    IF(REGEX_STRINGS)
      string(LENGTH ${FILE_NAME} len)
      math(EXPR len ${len}-4)
      string(SUBSTRING ${FILE_NAME} 0 ${len} OUTSTR)
      list(APPEND datatypeIncludeStringBuf ${OUTSTR})
      FOREACH(STR ${REGEX_STRINGS})
        string(LENGTH ${STR} len)
        math(EXPR len ${len}-25)
        string(SUBSTRING ${STR} 25 ${len} OUTSTR)
        list(APPEND datatypeFunctionStringBuf ${OUTSTR})
      ENDFOREACH(STR)
    ENDIF(REGEX_STRINGS)
    STRING(REGEX MATCHALL "DEFINE_FIRMWARE_FB\\(([A-Za-z0-9_]*)" REGEX_STRINGS ${FILE_CONTENT})
    IF(REGEX_STRINGS)
      string(LENGTH ${FILE_NAME} len)
      math(EXPR len ${len}-4)
      string(SUBSTRING ${FILE_NAME} 0 ${len} OUTSTR)
      list(APPEND fbIncludeStringBuf ${OUTSTR})
      FOREACH(STR ${REGEX_STRINGS})
        string(LENGTH ${STR} len)
        math(EXPR len ${len}-19)
        string(SUBSTRING ${STR} 19 ${len} OUTSTR)
        list(APPEND fbFunctionStringBuf ${OUTSTR})
      ENDFOREACH(STR)
    ENDIF(REGEX_STRINGS)
    STRING(REGEX MATCHALL "DEFINE_GENERIC_FIRMWARE_FB\\(([A-Za-z0-9_]*)" REGEX_STRINGS ${FILE_CONTENT})
    IF(REGEX_STRINGS)
      string(LENGTH ${FILE_NAME} len)
      math(EXPR len ${len}-4)
      string(SUBSTRING ${FILE_NAME} 0 ${len} OUTSTR)
      list(APPEND fbIncludeStringBuf ${OUTSTR})
      FOREACH(STR ${REGEX_STRINGS})
        string(LENGTH ${STR} len)
        math(EXPR len ${len}-27)
        string(SUBSTRING ${STR} 27 ${len} OUTSTR)
        list(APPEND fbFunctionStringBuf ${OUTSTR})
      ENDFOREACH(STR)
    ENDIF(REGEX_STRINGS)
    STRING(REGEX MATCHALL "DEFINE_ADAPTER_TYPE\\(([A-Za-z0-9_]*)" REGEX_STRINGS ${FILE_CONTENT})
    IF(REGEX_STRINGS)
      string(LENGTH ${FILE_NAME} len)
      math(EXPR len ${len}-4)
      string(SUBSTRING ${FILE_NAME} 0 ${len} OUTSTR)
      list(APPEND fbIncludeStringBuf ${OUTSTR})
      FOREACH(STR ${REGEX_STRINGS})
        string(LENGTH ${STR} len)
        math(EXPR len ${len}-20)
        string(SUBSTRING ${STR} 20 ${len} OUTSTR)
        list(APPEND fbFunctionStringBuf ${OUTSTR})
      ENDFOREACH(STR)
    ENDIF(REGEX_STRINGS)
  ENDIF(NOT REGEX_STRINGS)
ENDFOREACH(FILE_NAME)

list(SORT datatypeFunctionStringBuf)
list(REMOVE_DUPLICATES datatypeFunctionStringBuf)
SET(DATATYPE_FUNCTION_STRING "")
FOREACH(STR ${datatypeFunctionStringBuf})
  SET(DATATYPE_FUNCTION_STRING "${DATATYPE_FUNCTION_STRING}  CIEC_${STR}::dummyInit();\n")  
ENDFOREACH(STR)

list(SORT datatypeIncludeStringBuf)
list(REMOVE_DUPLICATES datatypeIncludeStringBuf)
SET(DATATYPE_INCLUDE_STRING "")
FOREACH(STR ${datatypeIncludeStringBuf})
  SET(DATATYPE_INCLUDE_STRING "${DATATYPE_INCLUDE_STRING}#include <${STR}.h>\n")
ENDFOREACH(STR)

list(SORT fbFunctionStringBuf)
list(REMOVE_DUPLICATES fbFunctionStringBuf)
SET(FB_FUNCTION_STRING "")
FOREACH(STR ${fbFunctionStringBuf})
  SET(FB_FUNCTION_STRING "${FB_FUNCTION_STRING}  ${STR}::dummyInit();\n")  
ENDFOREACH(STR)

list(SORT fbIncludeStringBuf)
list(REMOVE_DUPLICATES fbIncludeStringBuf)
SET(FB_INCLUDE_STRING "")
FOREACH(STR ${fbIncludeStringBuf})
  SET(FB_INCLUDE_STRING "${FB_INCLUDE_STRING}#include <${STR}.h>\n")
ENDFOREACH(STR)

CONFIGURE_FILE(${FORTE_SOURCE_DIR}/src/forteinit.cpp.in ${FORTE_BINARY_DIR}/forteinit_new.cpp)
CONFIGURE_FILE(${FORTE_SOURCE_DIR}/src/forteinit.h.in ${FORTE_BINARY_DIR}/forteinit_new.h)

# only copy files if different
execute_process( COMMAND ${CMAKE_COMMAND} -E compare_files ${FORTE_BINARY_DIR}/forteinit_new.cpp ${FORTE_BINARY_DIR}/forteinit.cpp RESULT_VARIABLE test_not_successful_cpp OUTPUT_QUIET ERROR_QUIET )
execute_process( COMMAND ${CMAKE_COMMAND} -E compare_files ${FORTE_BINARY_DIR}/forteinit_new.h ${FORTE_BINARY_DIR}/forteinit.h RESULT_VARIABLE test_not_successful_h OUTPUT_QUIET ERROR_QUIET )

IF(test_not_successful_cpp)
  file(REMOVE ${FORTE_BINARY_DIR}/forteinit.cpp)
  file(RENAME ${FORTE_BINARY_DIR}/forteinit_new.cpp ${FORTE_BINARY_DIR}/forteinit.cpp)
ENDIF(test_not_successful_cpp)

IF(test_not_successful_h)
  file(REMOVE ${FORTE_BINARY_DIR}/forteinit.h)
  file(RENAME ${FORTE_BINARY_DIR}/forteinit_new.h ${FORTE_BINARY_DIR}/forteinit.h)
ENDIF(test_not_successful_h)

file(REMOVE ${FORTE_BINARY_DIR}/forteinit_new.cpp)
file(REMOVE ${FORTE_BINARY_DIR}/forteinit_new.h)
