#*******************************************************************************
# Copyright (c) 2011 - 2018 ACIN, nxtControl, Profactor GmbH, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Ingo Hegny, Alois Zoitl, Stanislav Meduna, Micheal Hofmann, 
# *   Martin Melik-Merkumians  - initial API and implementation and/or initial documentation
# *   Martin Melik-Merkumians  - adds subdirectory for arch tests
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\tests)

#######################################################################################
# functions for test generation
#######################################################################################
MACRO(forte_test_add_subdirectory DIRECTORY)
  SET(SOURCE_GROUP_BACKUP ${SOURCE_GROUP})
  SET(SOURCE_GROUP ${SOURCE_GROUP}\\${DIRECTORY})
  add_subdirectory(${DIRECTORY})
  SET(SOURCE_GROUP ${SOURCE_GROUP_BACKUP})
ENDMACRO(forte_test_add_subdirectory)

FUNCTION(forte_test_add_sourcefile_with_path_cpp)
  FOREACH(ARG ${ARGV})
  SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_TEST_SOURCE_CPP ${ARG})
    SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_TEST_SOURCE_CPP_GROUP ${SOURCE_GROUP})
  ENDFOREACH(ARG)
ENDFUNCTION(forte_test_add_sourcefile_with_path_cpp)

FUNCTION(forte_test_add_sourcefile_cpp)
  FOREACH(ARG ${ARGV})
  forte_test_add_sourcefile_with_path_cpp(${CMAKE_CURRENT_SOURCE_DIR}/${ARG})
  ENDFOREACH(ARG)
ENDFUNCTION(forte_test_add_sourcefile_cpp)

FUNCTION(forte_test_add_link_directories)
  FOREACH(ARG ${ARGV})
    SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_TEST_LINK_DIRECTORIES ${ARG})
  ENDFOREACH(ARG)
ENDFUNCTION(forte_test_add_link_directories)

FUNCTION(forte_test_add_inc_directories)
  FOREACH(ARG ${ARGV})
    SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_TEST_INC_DIRECTORIES ${ARG})
  ENDFOREACH(ARG)
ENDFUNCTION(forte_test_add_inc_directories)

FUNCTION(forte_test_add_inc_system_directories)
  FOREACH(ARG ${ARGV})
    SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_TEST_INC_SYSTEM_DIRECTORIES ${ARG})
  ENDFOREACH(ARG)
ENDFUNCTION(forte_test_add_inc_system_directories)

#######################################################################################
# add subdirectories
#######################################################################################

forte_test_add_sourcefile_cpp(forte_boost_tester.cpp)

forte_test_add_subdirectory(arch)
forte_test_add_subdirectory(core)
forte_test_add_subdirectory(modules)
forte_test_add_subdirectory(stdfblib)

#######################################################################################
# set library path
#######################################################################################
forte_test_add_link_directories(${FORTE_TESTS_LINK_DIRS})
forte_test_add_link_directories(${CMAKE_BINARY_DIR}/src)

get_property(LINK_DIRECTORIES GLOBAL PROPERTY FORTE_LINK_DIRECTORIES)
GET_PROPERTY(FORTE_TEST_LINK_DIR GLOBAL PROPERTY FORTE_TEST_LINK_DIRECTORIES)

LIST(APPEND LINK_DIRECTORIES ${FORTE_TEST_LINK_DIR})

LIST(LENGTH LINK_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES LINK_DIRECTORIES)
ENDIF(len GREATER 0)
LINK_DIRECTORIES(${LINK_DIRECTORIES})

#######################################################################################
# Setup Forte-Executeable with all Functionblocks
#######################################################################################

GET_PROPERTY(FORTE_TEST_SOURCE_H GLOBAL PROPERTY FORTE_SOURCE_H)
  
GET_PROPERTY(SOURCE_H              GLOBAL PROPERTY FORTE_TEST_SOURCE_H)
GET_PROPERTY(SOURCE_H_GROUP        GLOBAL PROPERTY FORTE_TEST_SOURCE_H_GROUP)

GET_PROPERTY(SOURCE_CPP              GLOBAL PROPERTY FORTE_TEST_SOURCE_CPP)
GET_PROPERTY(SOURCE_CPP_GROUP_STRUCT GLOBAL PROPERTY FORTE_TEST_SOURCE_CPP_GROUP)

SET(WRITE_FILE "")
FOREACH(FILE ${SOURCE_CPP} ${SOURCE_H})
  SET(WRITE_FILE "${WRITE_FILE}${FILE}\n")
ENDFOREACH(FILE)
FILE(WRITE ${CMAKE_BINARY_DIR}/file_test_list.txt "${WRITE_FILE}")
  
#######################################################################################
# Create Exe File
#######################################################################################

ADD_EXECUTABLE(forte_test $<TARGET_OBJECTS:FORTE_LITE> ${SOURCE_H} ${SOURCE_CPP})

#######################################################################################
# Add definitions
#######################################################################################

GET_PROPERTY(DEFINITION GLOBAL PROPERTY FORTE_DEFINITION)
add_definitions(${DEFINITION}) #using target_compile_definitions put the definitions at the beginning giving problems sometimes because cmake adds -O3.

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
 ADD_DEFINITIONS (-DBOOST_TEST_DYN_LINK)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
 ADD_DEFINITIONS (-DBOOST_TEST_NO_LIB)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")

add_dependencies(forte_test FORTE_LITE)
add_dependencies(forte_test forte_stringlist_generator)

SET_TARGET_PROPERTIES(forte_test PROPERTIES LINKER_LANGUAGE CXX)

get_target_property(existing_link_flags_test forte_test LINK_FLAGS)
if(existing_link_flags_test)
  set_property(GLOBAL APPEND PROPERTY FORTE_TEST_LINK_FLAGS ${existing_link_flags_test})
endif()

GET_PROPERTY(link_flags_test GLOBAL PROPERTY FORTE_TEST_LINK_FLAGS)

#add forte_test to the list of test to be executed by ctest
add_test(forte_test ${EXECUTABLE_OUTPUT_PATH}/forte_test)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
  set(FORTE_TEST_CODE_COVERAGE_ANALYSIS OFF CACHE BOOL "Perform code coverage analyis with GCOV and presentation with LCOV")
  mark_as_advanced(FORTE_TEST_CODE_COVERAGE_ANALYSIS)
  if(FORTE_TEST_CODE_COVERAGE_ANALYSIS)
      INCLUDE(${FORTE_BUILDSUPPORT_DIRECTORY}/GCov.cmake)    
      SETUP_GCOV(TestCoverage ctest coverage)
  endif() 
endif()

#######################################################################################
# Generate stringlist for every source file
#######################################################################################
if(FORTE_LINKED_STRINGDICT)
  set(ENABLE_GENERATED_SOURCE_CPP ON)
  ADD_DEPENDENCIES (forte_test forte_stringlist_externals)
endif(FORTE_LINKED_STRINGDICT)

#######################################################################################
# Enable autogenerated source files
#######################################################################################
if(ENABLE_GENERATED_SOURCE_CPP)
   target_compile_definitions(forte_test PUBLIC "-DFORTE_ENABLE_GENERATED_SOURCE_CPP")
endif(ENABLE_GENERATED_SOURCE_CPP)

#######################################################################################
# add includes
#######################################################################################
forte_test_add_inc_system_directories(${FORTE_TESTS_INC_DIRS})
forte_test_add_inc_system_directories(./)

GET_PROPERTY(INCLUDE_DIRECTORIES GLOBAL PROPERTY FORTE_INCLUDE_DIRECTORIES)
GET_PROPERTY(FORTE_TEST_INC GLOBAL PROPERTY FORTE_TEST_INC_DIRECTORIES)

LIST(APPEND INCLUDE_DIRECTORIES ${FORTE_TEST_INC})

LIST(LENGTH INCLUDE_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES INCLUDE_DIRECTORIES)
  LIST(REVERSE INCLUDE_DIRECTORIES) # bugfix, for replaced include files
ENDIF(len GREATER 0)

GET_PROPERTY(INCLUDE_SYSTEM_DIRECTORIES GLOBAL PROPERTY FORTE_INCLUDE_SYSTEM_DIRECTORIES)
GET_PROPERTY(INCLUDE_TEST_SYSTEM_DIRECTORIES GLOBAL PROPERTY FORTE_TEST_INC_SYSTEM_DIRECTORIES)
LIST(APPEND INCLUDE_TEST_SYSTEM_DIRECTORIES ${INCLUDE_SYSTEM_DIRECTORIES})

LIST(LENGTH INCLUDE_TEST_SYSTEM_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES INCLUDE_TEST_SYSTEM_DIRECTORIES)
  LIST(REVERSE INCLUDE_TEST_SYSTEM_DIRECTORIES) # bugfix, for replaced include files
ENDIF(len GREATER 0)

target_include_directories(forte_test PUBLIC ${INCLUDE_DIRECTORIES})
INCLUDE_DIRECTORIES(SYSTEM ${INCLUDE_TEST_SYSTEM_DIRECTORIES})

#######################################################################################
# Link Libraries to the Executeable
#######################################################################################

#Link flags
get_property(LINK_TEST_LIBRARY GLOBAL PROPERTY FORTE_LINK_LIBRARY)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    SET_PROPERTY(GLOBAL APPEND PROPERTY LINK_TEST_LIBRARY "-lboost_unit_test_framework -lboost_prg_exec_monitor -lboost_test_exec_monitor")
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

TARGET_LINK_LIBRARIES(forte_test ${LINK_TEST_LIBRARY})

