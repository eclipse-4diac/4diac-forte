#*******************************************************************************
# Copyright (c) 2011 - 2024 ACIN, nxtControl, Profactor GmbH, fortiss GmbH, Jose Cabral
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

if (NOT FORTE_TESTS)
  return()
endif()

if (FORTE_ARCHITECTURE STREQUAL "Posix")
  option(FORTE_TEST_CODE_COVERAGE_ANALYSIS "Perform code coverage analyis with GCOV and presentation with LCOV" OFF)
  mark_as_advanced(FORTE_TEST_CODE_COVERAGE_ANALYSIS)
  option(FORTE_TEST_SANITIZE "Perform address sanitizer analysis" OFF)
endif()


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

FUNCTION(forte_test_add_link_library)
  FOREACH(ARG ${ARGV})
    SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_TEST_LIBRARIES ${ARG})
  ENDFOREACH(ARG)
ENDFUNCTION(forte_test_add_link_library)

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

GET_PROPERTY(LINK_DIRECTORIES GLOBAL PROPERTY FORTE_TEST_LINK_DIRECTORIES)
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
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/file_test_list.txt "${WRITE_FILE}")
  
#######################################################################################
# Create Exe File
#######################################################################################

ADD_EXECUTABLE(forte_test ${SOURCE_H} ${SOURCE_CPP})
get_property(forte_libs TARGET forte PROPERTY LINK_LIBRARIES)
target_link_libraries(forte_test ${forte_libs})
target_compile_features(forte_test PUBLIC cxx_std_20)

target_include_directories(forte_test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

#######################################################################################
# Add definitions
#######################################################################################

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
 ADD_DEFINITIONS (-DBOOST_TEST_NO_LIB)
 if(MINGW)
  #force MINGW to statically link libc and lib stdc++ so that no mingw dlls are needed for forte_test.exe
  TARGET_LINK_OPTIONS(forte_test PRIVATE -static-libgcc -static-libstdc++)
 endif()
endif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")

SET_TARGET_PROPERTIES(forte_test PROPERTIES LINKER_LANGUAGE CXX)

#add forte_test to the list of test to be executed by ctest
add_test(forte_test ${EXECUTABLE_OUTPUT_PATH}/forte_test)

set_tests_properties(forte_test PROPERTIES TIMEOUT 300)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
  if(FORTE_TEST_CODE_COVERAGE_ANALYSIS)
      INCLUDE(GCov.cmake)    
      SETUP_GCOV(TestCoverage ctest coverage)
  endif() 
endif()

#######################################################################################
# add includes
#######################################################################################
forte_test_add_inc_system_directories(${FORTE_TESTS_INC_DIRS})
forte_test_add_inc_system_directories(./)

GET_PROPERTY(FORTE_TEST_INC GLOBAL PROPERTY FORTE_TEST_INC_DIRECTORIES)
target_include_directories(forte_test PUBLIC ${FORTE_TEST_INC})

GET_PROPERTY(INCLUDE_SYSTEM_DIRECTORIES GLOBAL PROPERTY FORTE_INCLUDE_SYSTEM_DIRECTORIES)
GET_PROPERTY(INCLUDE_TEST_SYSTEM_DIRECTORIES GLOBAL PROPERTY FORTE_TEST_INC_SYSTEM_DIRECTORIES)
LIST(APPEND INCLUDE_TEST_SYSTEM_DIRECTORIES ${INCLUDE_SYSTEM_DIRECTORIES})

LIST(LENGTH INCLUDE_TEST_SYSTEM_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES INCLUDE_TEST_SYSTEM_DIRECTORIES)
  LIST(REVERSE INCLUDE_TEST_SYSTEM_DIRECTORIES) # bugfix, for replaced include files
ENDIF(len GREATER 0)

INCLUDE_DIRECTORIES(SYSTEM ${INCLUDE_TEST_SYSTEM_DIRECTORIES})

#######################################################################################
# Link Libraries to the Executable
#######################################################################################

#Link flags
get_property(LINK_TEST_LIBRARY GLOBAL PROPERTY FORTE_TEST_LIBRARIES)
target_link_libraries(forte_test ${LINK_TEST_LIBRARY})
