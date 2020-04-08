#*******************************************************************************
# Copyright (c) 2010 - 2014 Profactor GmbH, ACIN, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#     Michael Hofmann, Alois Zoitl, Ingo Hegny, Gerhard Ebenhofer, Matthias Plasch - initial API and implementation and/or initial documentation
# *******************************************************************************/

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

set(LENGTH_COUNT 0)
FOREACH(FBLIB_FILE ${FBLIB_STRUCT})
  # Do not pars stringlist, as these files will be generated
  STRING(REGEX MATCH  ".*stringlist\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
  IF(NOT REGEX_STRINGS)
    STRING(REGEX MATCH  ".*forteinit\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
  ENDIF(NOT REGEX_STRINGS)
  IF(NOT REGEX_STRINGS)
    FILE(READ ${FBLIB_FILE} FILE_STRING)
    STRING(REGEX MATCHALL  "g_nStringId([A-Za-z0-9_]*)" REGEX_STRINGS ${FILE_STRING})
    STRING(REGEX MATCHALL  "g_nStringId([0-9][A-Za-z0-9_]*)" REGEX_STRINGS_ERROR ${FILE_STRING})

    FOREACH(STR ${REGEX_STRINGS})
      string(LENGTH ${STR} len)
    math(EXPR len ${len}-11)
      string(SUBSTRING ${STR} 11 ${len} OUTSTR)
    list(APPEND scm_acConstStringBuf ${OUTSTR})
    ENDFOREACH(STR)

    FOREACH(STR_ERR ${REGEX_STRINGS_ERROR})
      list(APPEND ErrorStringBuf "#ERROR Invalid Name used! \n//" ${FBLIB_FILE}:${STR_ERR})
    ENDFOREACH(STR_ERR)

  ENDIF(NOT REGEX_STRINGS)
ENDFOREACH(FBLIB_FILE)

list(REMOVE_DUPLICATES ErrorStringBuf)

list(REMOVE_ITEM scm_acConstStringBuf "NextFreeId")
list(SORT scm_acConstStringBuf)
list(REMOVE_DUPLICATES scm_acConstStringBuf)
SET(STRINGLIST_H "")
SET(STRINGLIST_CPP "")
FOREACH(STR IN ITEMS ${scm_acConstStringBuf})
  string(LENGTH ${STR} len)
  math(EXPR len ${len}+1) # \0 is only one character
  list(APPEND scm_aunIdList ${LENGTH_COUNT})
  SET(STRINGLIST_H "${STRINGLIST_H}const CStringDictionary::TStringId g_nStringId${STR} = ${LENGTH_COUNT};\n")
  SET(STRINGLIST_CPP "${STRINGLIST_CPP}extern const CStringDictionary::TStringId g_nStringId${STR} = ${LENGTH_COUNT};\n")
  math(EXPR LENGTH_COUNT ${LENGTH_COUNT}+${len})
ENDFOREACH(STR)
list(LENGTH scm_aunIdList NUMOFCONSTSTRINGS)

IF(FORTE_LINKED_STRINGDICT)
  SET(STRINGLIST_H)
  SET(STRINGLIST_H "${STRINGLIST_H}extern const CStringDictionary::TStringId g_nStringIdNextFreeId;\n")
  SET(STRINGLIST_H "${STRINGLIST_H}extern const unsigned int cg_nNumOfConstStrings;\n")
  SET(STRINGLIST_CPP "${STRINGLIST_CPP}extern const CStringDictionary::TStringId g_nStringIdNextFreeId = ${LENGTH_COUNT};\n")
  SET(STRINGLIST_CPP "${STRINGLIST_CPP}extern const unsigned int cg_nNumOfConstStrings = ${NUMOFCONSTSTRINGS};\n")
ELSE(FORTE_LINKED_STRINGDICT)
  SET(STRINGLIST_CPP)
  SET(STRINGLIST_H "\n${STRINGLIST_H}const CStringDictionary::TStringId g_nStringIdNextFreeId = ${LENGTH_COUNT};\n")
  SET(STRINGLIST_H "${STRINGLIST_H}const unsigned int cg_nNumOfConstStrings = ${NUMOFCONSTSTRINGS};\n")
ENDIF(FORTE_LINKED_STRINGDICT)


SET(scm_aunIdList_Str "")
SET(FIRST TRUE)
FOREACH(NUM ${scm_aunIdList})
  if(FIRST)
    SET(scm_aunIdList_Str "${scm_aunIdList_Str}${NUM}")
  SET(FIRST FALSE)
  ELSE(FIRST)
    SET(scm_aunIdList_Str "${scm_aunIdList_Str}, ${NUM}")
  ENDIF(FIRST)
ENDFOREACH(NUM)

SET(scm_acConstStringBuf_Str "")
IF(WIN32)
  string(LENGTH "1" len)
  math(EXPR cnt ${len})
ENDIF(WIN32)
FOREACH(STR IN ITEMS ${scm_acConstStringBuf})
  SET(scm_acConstStringBuf_Str "${scm_acConstStringBuf_Str}${STR}\\0")
  IF(WIN32)
  string(LENGTH ${scm_acConstStringBuf_Str} len)
    math(EXPR lenRest ${len})
  math(EXPR bytes ${cnt}*15000)
  IF(${lenRest} GREATER ${bytes})
      SET(scm_acConstStringBuf_Str "${scm_acConstStringBuf_Str}\" \"")
      math(EXPR cnt ${cnt}+1)
    ENDIF(${lenRest} GREATER ${bytes})
  ENDIF(WIN32)
ENDFOREACH(STR)

CONFIGURE_FILE(${FORTE_SOURCE_DIR}/stringlist.h.in ${FORTE_BINARY_DIR}/stringlist_new.h)
CONFIGURE_FILE(${FORTE_SOURCE_DIR}/stringlist.cpp.in ${FORTE_BINARY_DIR}/stringlist_new.cpp)

# only copy files if different
execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FORTE_BINARY_DIR}/stringlist_new.h ${FORTE_BINARY_DIR}/stringlist.h OUTPUT_QUIET ERROR_QUIET )
execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FORTE_BINARY_DIR}/stringlist_new.cpp ${FORTE_BINARY_DIR}/stringlist.cpp OUTPUT_QUIET ERROR_QUIET )
file(REMOVE ${FORTE_BINARY_DIR}/stringlist_new.h)
file(REMOVE ${FORTE_BINARY_DIR}/stringlist_new.cpp)
