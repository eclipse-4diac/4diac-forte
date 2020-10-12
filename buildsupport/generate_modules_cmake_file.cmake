#################################################################################
# Copyright (c) 2011, 2016 Profactor GmbH, fortiss GmbH
# 
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Michael Hofmann - initial API and implementation and/or initial documentation
#    Alois Zoitl     - added support for externam modules directory
#################################################################################

#First find all the internal forte modules
FOREACH(FORTE_MODULE_DIR ${FORTE_MODULE_LIST})
  string(COMPARE EQUAL "${FORTE_MODULE_DIR}" external _isExternal)
  if(_isExternal AND EXISTS ${FORTE_EXTERNAL_MODULES_DIRECTORY})
    set(FORTE_MODULE_PATH ${FORTE_EXTERNAL_MODULES_DIRECTORY}/)
  elseif(_isExternal AND NOT EXISTS ${FORTE_EXTERNAL_MODULES_DIRECTORY})
    continue()
  else()
    SET(FORTE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${FORTE_MODULE_DIR}/)
  endif()
  FILE(GLOB_RECURSE FILES_FOUND "${FORTE_MODULE_PATH}/*CMakeLists.txt")
  message(STATUS "FORTE_MODULE_DIR: " ${FORTE_MODULE_PATH})
  FOREACH(FILE ${FILES_FOUND})
    STRING(REPLACE "${FORTE_MODULE_PATH}" "" MODULE_NAME ${FILE})
    STRING(REGEX MATCH "^/([^/]*)/CMakeLists.txt$" MODULE_NAME ${MODULE_NAME})
    IF(MODULE_NAME)
      string(REPLACE "/CMakeLists.txt" "" MODULE_NAME "${MODULE_NAME}")
      string(REPLACE "/" "" MODULE_NAME "${MODULE_NAME}")
      LIST(APPEND MODULE_LIST ${MODULE_NAME})
    ENDIF(MODULE_NAME)
  ENDFOREACH(FILE)
  SET(WRITE_FILE "")
  FOREACH(MODULE ${MODULE_LIST})
    SET(WRITE_FILE "${WRITE_FILE}SET(SOURCE_GROUP_BACKUP \"\${SOURCE_GROUP}\")\n")
    SET(WRITE_FILE "${WRITE_FILE}SET(SOURCE_GROUP \"\${SOURCE_GROUP}/${MODULE}\")\n")
    if(_isExternal)
      SET(WRITE_FILE "${WRITE_FILE}ADD_SUBDIRECTORY(\"\${FORTE_EXTERNAL_MODULES_DIRECTORY}/${MODULE}\" \"\${CMAKE_CURRENT_BINARY_DIR}/external/${MODULE}\")\n")
    else()
      SET(WRITE_FILE "${WRITE_FILE}ADD_SUBDIRECTORY(\"\${FORTE_MODULE_PATH}${MODULE}\")\n")
    endif()
    SET(WRITE_FILE "${WRITE_FILE}SET(SOURCE_GROUP \"\${SOURCE_GROUP_BACKUP}\")\n")
  ENDFOREACH(MODULE)
  set(MODULE_LIST )
  file(WRITE ${CMAKE_BINARY_DIR}/src/${FORTE_MODULE_DIR}/CMakeLists_new.txt ${WRITE_FILE})
ENDFOREACH(FORTE_MODULE_DIR)

foreach(FORTE_MODULE_DIR ${FORTE_MODULE_LIST})
  string(COMPARE EQUAL "${FORTE_MODULE_DIR}" external _isExternal)
  if (_isExternal AND NOT EXISTS ${FORTE_EXTERNAL_MODULES_DIRECTORY})
    continue()
  endif()
  execute_process( COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_BINARY_DIR}/src/${FORTE_MODULE_DIR}/CMakeLists_new.txt ${CMAKE_BINARY_DIR}/src/${FORTE_MODULE_DIR}/CMakeLists.txt RESULT_VARIABLE test_not_successful OUTPUT_QUIET ERROR_QUIET )
  if(test_not_successful)
    file(REMOVE ${CMAKE_BINARY_DIR}/src/${FORTE_MODULE_DIR}/CMakeLists.txt)
    file(RENAME ${CMAKE_BINARY_DIR}/src/${FORTE_MODULE_DIR}/CMakeLists_new.txt ${CMAKE_BINARY_DIR}/src/${FORTE_MODULE_DIR}/CMakeLists.txt)
  ENDIF( test_not_successful)
  file(REMOVE ${CMAKE_BINARY_DIR}/src/${FORTE_MODULE_DIR}/CMakeLists_new.txt)
endforeach(FORTE_MODULE_DIR)