#*******************************************************************************
# Copyright (c) 2010, 2023 Profactor GmbH, ACIN, fortiss GmbH
#                          Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#     Michael Hofmann, Alois Zoitl, Ingo Hegny, Gerhard Ebenhofer, Matthias Plasch - initial API and implementation and/or initial documentation
#     Martin Jobst - fixed handling of empty string constants
# *******************************************************************************/

cmake_policy(SET CMP0007 NEW)

MESSAGE(Generate Stringlist)
MESSAGE("Source Dir: ${FORTE_SOURCE_DIR}")
MESSAGE("Binary Dir: ${FORTE_BINARY_DIR}")
FILE(STRINGS ${FORTE_BINARY_DIR}/file_list.txt FBLIB_STRUCT)


if(EXISTS ${FORTE_BINARY_DIR}/file_test_list.txt)
  Message("adding testlist")
  FILE(STRINGS ${FORTE_BINARY_DIR}/file_test_list.txt FBLIB_TEST_STRUCT)

  LIST(APPEND FBLIB_STRUCT ${FBLIB_TEST_STRUCT})
  LIST(REMOVE_DUPLICATES FBLIB_STRUCT)
endif()

#######################################################################################
# Create Forte-Executeable with all Functionblocks
#######################################################################################

LIST(LENGTH FBLIB_STRUCT len)

list(APPEND ErrorStringBuf " ")

set(LENGTH_COUNT 1)
FOREACH(FBLIB_FILE ${FBLIB_STRUCT})
  # Do not pars stringlist, as these files will be generated
  STRING(REGEX MATCH  ".*stringlist\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
  IF(NOT REGEX_STRINGS)
    STRING(REGEX MATCH  ".*forteinit\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
  ENDIF(NOT REGEX_STRINGS)
  IF(NOT REGEX_STRINGS)
    STRING(REGEX MATCH  ".*startuphook\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
  ENDIF(NOT REGEX_STRINGS)
  
  IF(NOT REGEX_STRINGS)
    FILE(READ ${FBLIB_FILE} FILE_STRING)
    STRING(REGEX MATCHALL  "g_nStringId([A-Za-z0-9_]*)" REGEX_STRINGS ${FILE_STRING})
    STRING(REGEX MATCHALL  "g_nStringId([0-9][A-Za-z0-9_]*)" REGEX_STRINGS_ERROR ${FILE_STRING})

    FOREACH(STR ${REGEX_STRINGS})
      string(LENGTH ${STR} len)
    math(EXPR len ${len}-11)
      string(SUBSTRING ${STR} 11 ${len} OUTSTR)
    list(APPEND scmConstStringBuf "${OUTSTR}")
    ENDFOREACH(STR)

    FOREACH(STR_ERR ${REGEX_STRINGS_ERROR})
      list(APPEND ErrorStringBuf "#ERROR Invalid Name used! \n//" ${FBLIB_FILE}:${STR_ERR})
    ENDFOREACH(STR_ERR)

  ENDIF(NOT REGEX_STRINGS)
ENDFOREACH(FBLIB_FILE)

list(REMOVE_DUPLICATES ErrorStringBuf)

list(REMOVE_ITEM scmConstStringBuf "NextFreeId")
list(SORT scmConstStringBuf)
list(REMOVE_DUPLICATES scmConstStringBuf)
SET(STRINGLIST_H "")
SET(STRINGLIST_CPP "")
FOREACH(STR IN LISTS scmConstStringBuf)
  string(LENGTH "${STR}" len)
  math(EXPR len ${len}+1) # \0 is only one character
  list(APPEND scmIdList ${LENGTH_COUNT})
  SET(STRINGLIST_H "${STRINGLIST_H}const CStringDictionary::TStringId g_nStringId${STR} = ${LENGTH_COUNT};\n")
  SET(STRINGLIST_CPP "${STRINGLIST_CPP}extern const CStringDictionary::TStringId g_nStringId${STR} = ${LENGTH_COUNT};\n")
  math(EXPR LENGTH_COUNT ${LENGTH_COUNT}+${len})
ENDFOREACH(STR)
list(LENGTH scmIdList NUMOFCONSTSTRINGS)

IF(FORTE_LINKED_STRINGDICT)
  SET(STRINGLIST_H)
  SET(STRINGLIST_H "${STRINGLIST_H}extern const CStringDictionary::TStringId g_nStringIdNextFreeId;\n")
  SET(STRINGLIST_H "${STRINGLIST_H}extern const unsigned int cgNumOfConstStrings;\n")
  SET(STRINGLIST_CPP "${STRINGLIST_CPP}extern const CStringDictionary::TStringId g_nStringIdNextFreeId = ${LENGTH_COUNT};\n")
  SET(STRINGLIST_CPP "${STRINGLIST_CPP}extern const unsigned int cgNumOfConstStrings = ${NUMOFCONSTSTRINGS};\n")
ELSE(FORTE_LINKED_STRINGDICT)
  SET(STRINGLIST_CPP)
  SET(STRINGLIST_H "\n${STRINGLIST_H}const CStringDictionary::TStringId g_nStringIdNextFreeId = ${LENGTH_COUNT};\n")
  SET(STRINGLIST_H "${STRINGLIST_H}const unsigned int cgNumOfConstStrings = ${NUMOFCONSTSTRINGS};\n")
ENDIF(FORTE_LINKED_STRINGDICT)


SET(scmIdList_Str "")
SET(FIRST TRUE)
FOREACH(NUM IN LISTS scmIdList)
  if(FIRST)
    SET(scmIdList_Str "${scmIdList_Str}${NUM}")
  SET(FIRST FALSE)
  ELSE(FIRST)
    SET(scmIdList_Str "${scmIdList_Str}, ${NUM}")
  ENDIF(FIRST)
ENDFOREACH(NUM)

SET(scmConstStringBuf_Str "\\")
FOREACH(STR IN LISTS scmConstStringBuf)
  SET(scmConstStringBuf_Str "${scmConstStringBuf_Str}${STR}\\")
ENDFOREACH(STR)

string(REGEX REPLACE "(.)" ",'\\1'" scmConstStringBuf_Str "${scmConstStringBuf_Str}")
string(REPLACE "\\" "\\0" scmConstStringBuf_Str "${scmConstStringBuf_Str}")
string(SUBSTRING "${scmConstStringBuf_Str}" 1 -1 scmConstStringBuf_Str)

CONFIGURE_FILE(${FORTE_SOURCE_DIR}/stringlist.h.in ${FORTE_BINARY_DIR}/stringlist_new.h)
CONFIGURE_FILE(${FORTE_SOURCE_DIR}/stringlist.cpp.in ${FORTE_BINARY_DIR}/stringlist_new.cpp)

# only copy files if different
execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FORTE_BINARY_DIR}/stringlist_new.h ${FORTE_BINARY_DIR}/stringlist.h OUTPUT_QUIET ERROR_QUIET )
execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FORTE_BINARY_DIR}/stringlist_new.cpp ${FORTE_BINARY_DIR}/stringlist.cpp OUTPUT_QUIET ERROR_QUIET )
file(REMOVE ${FORTE_BINARY_DIR}/stringlist_new.h)
file(REMOVE ${FORTE_BINARY_DIR}/stringlist_new.cpp)
