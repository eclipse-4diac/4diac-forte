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
FILE(GLOB_RECURSE FILES_FOUND "${FORTE_MODULE_DIR}/*CMakeLists.txt")

FOREACH(FILE ${FILES_FOUND})
  string(REPLACE "${FORTE_MODULE_DIR}" "" MODULE_NAME ${FILE})
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
  SET(WRITE_FILE "${WRITE_FILE}ADD_SUBDIRECTORY(\"\${FORTE_MODULE_DIR}${MODULE}\")\n")
  SET(WRITE_FILE "${WRITE_FILE}SET(SOURCE_GROUP \"\${SOURCE_GROUP_BACKUP}\")\n")
ENDFOREACH(MODULE)

file(WRITE ${CMAKE_BINARY_DIR}/src/modules/CMakeLists_new.txt ${WRITE_FILE})

#second check the external module directory
message(STATUS "FORTE_EXTERNAL_MODULES_DIRECTORY: " ${FORTE_EXTERNAL_MODULE_DIR})

IF(EXISTS  ${FORTE_EXTERNAL_MODULE_DIR})
  message(STATUS "external modules dir exists")
  
  FILE(GLOB_RECURSE EXT_FILES_FOUND "${FORTE_EXTERNAL_MODULE_DIR}/*CMakeLists.txt")

  FOREACH(FILE ${EXT_FILES_FOUND})
    string(REPLACE "${FORTE_EXTERNAL_MODULE_DIR}" "" MODULE_NAME ${FILE})
    STRING(REGEX MATCH "^/([^/]*)/CMakeLists.txt$" MODULE_NAME ${MODULE_NAME})
    IF(MODULE_NAME)
      string(REPLACE "/CMakeLists.txt" "" MODULE_NAME "${MODULE_NAME}")
      string(REPLACE "/" "" MODULE_NAME "${MODULE_NAME}")
      LIST(APPEND EXT_MODULE_LIST ${MODULE_NAME})
    ENDIF(MODULE_NAME)
  ENDFOREACH(FILE)  
  
  SET(EXT_WRITE_FILE "")
  
  FOREACH(MODULE ${EXT_MODULE_LIST})
    SET(EXT_WRITE_FILE "${EXT_WRITE_FILE}SET(SOURCE_GROUP_BACKUP \"\${SOURCE_GROUP}\")\n")
    SET(EXT_WRITE_FILE "${EXT_WRITE_FILE}SET(SOURCE_GROUP \"\${SOURCE_GROUP}/${MODULE}\")\n")
    SET(EXT_WRITE_FILE "${EXT_WRITE_FILE}ADD_SUBDIRECTORY(\"\${FORTE_EXTERNAL_MODULES_DIRECTORY}/${MODULE}\" \"\${CMAKE_CURRENT_BINARY_DIR}/ext_${MODULE}\")\n")
    SET(EXT_WRITE_FILE "${EXT_WRITE_FILE}SET(SOURCE_GROUP \"\${SOURCE_GROUP_BACKUP}\")\n")
  ENDFOREACH(MODULE)
  
  file(APPEND ${CMAKE_BINARY_DIR}/src/modules/CMakeLists_new.txt ${EXT_WRITE_FILE})  
endif()

  
execute_process( COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_BINARY_DIR}/src/modules/CMakeLists_new.txt ${CMAKE_BINARY_DIR}/src/modules/CMakeLists.txt RESULT_VARIABLE test_not_successful OUTPUT_QUIET ERROR_QUIET )  

IF(test_not_successful)
  file(REMOVE ${CMAKE_BINARY_DIR}/src/modules/CMakeLists.txt)
  file(RENAME ${CMAKE_BINARY_DIR}/src/modules/CMakeLists_new.txt ${CMAKE_BINARY_DIR}/src/modules/CMakeLists.txt)
ENDIF( test_not_successful)  
file(REMOVE ${CMAKE_BINARY_DIR}/src/modules/CMakeLists_new.txt) 