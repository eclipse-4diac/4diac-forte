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

ADD_EXECUTABLE(forte_test ${SOURCE_H} ${SOURCE_CPP})
target_link_libraries(forte_test FORTE_LITE)

#######################################################################################
# Add definitions
#######################################################################################

GET_PROPERTY(DEFINITION GLOBAL PROPERTY FORTE_DEFINITION)
add_definitions(${DEFINITION}) #using target_compile_definitions put the definitions at the beginning giving problems sometimes because cmake adds -O3.

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
 if(FORTE_LINK_STATIC)
  set_target_properties(forte_test PROPERTIES LINK_SEARCH_START_STATIC ON)
  set_target_properties(forte_test PROPERTIES LINK_SEARCH_END_STATIC ON)
  target_link_options(forte_test PRIVATE -static-libgcc -static-libstdc++ -static)
 endif()
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
 ADD_DEFINITIONS (-DBOOST_TEST_NO_LIB)
 if(MINGW)
  #force MINGW to statically link libc and lib stdc++ so that no mingw dlls are needed for forte_test.exe
  TARGET_LINK_OPTIONS(forte_test PRIVATE -static-libgcc -static-libstdc++)
 endif()
endif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")

add_dependencies(forte_test forte_stringlist_generator)

SET_TARGET_PROPERTIES(forte_test PROPERTIES LINKER_LANGUAGE CXX)

#add forte_test to the list of test to be executed by ctest
add_test(forte_test ${EXECUTABLE_OUTPUT_PATH}/forte_test)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
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
TARGET_LINK_LIBRARIES(forte_test ${LINK_TEST_LIBRARY})

#######################################################################################
# Link Libraries to the C Interface Executable
#######################################################################################

if(FORTE_C_INTERFACE)
  if(FORTE_BUILD_STATIC_LIBRARY)
    SET(FORTE_C_STATIC_TEST_NAME "forte_c_static_test")

    ADD_EXECUTABLE(${FORTE_C_STATIC_TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/arch/forte_c_test.cpp)
    TARGET_LINK_LIBRARIES(${FORTE_C_STATIC_TEST_NAME} forte-c-static ${LINK_TEST_LIBRARY})
    target_include_directories(${FORTE_C_STATIC_TEST_NAME} PRIVATE ${FORTE_TEST_INC})
    SET_TARGET_PROPERTIES(${FORTE_C_STATIC_TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)

    add_test(${FORTE_C_STATIC_TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${FORTE_C_STATIC_TEST_NAME})
  endif(FORTE_BUILD_STATIC_LIBRARY)

  # Currently not working in windows
  if(FORTE_BUILD_SHARED_LIBRARY AND NOT WIN32)
    SET(FORTE_C_SHARED_TEST_NAME "forte_c_shared_test")
  
    ADD_EXECUTABLE(${FORTE_C_SHARED_TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/arch/forte_c_test.cpp)
    TARGET_LINK_LIBRARIES(${FORTE_C_SHARED_TEST_NAME} forte-c-shared ${LINK_TEST_LIBRARY})
    target_include_directories(${FORTE_C_SHARED_TEST_NAME} PRIVATE ${FORTE_TEST_INC})
    SET_TARGET_PROPERTIES(${FORTE_C_STATIC_TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)

    add_test(${FORTE_C_SHARED_TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${FORTE_C_SHARED_TEST_NAME})
  endif(FORTE_BUILD_SHARED_LIBRARY AND NOT WIN32) 

endif(FORTE_C_INTERFACE)

#######################################################################################
# Test if main can link to forte-static
#######################################################################################
if(FORTE_BUILD_STATIC_LIBRARY)
  SET(TEST_FORTE_STATIC "forte_test_static")
  GET_PROPERTY(SOURCE_EXECUTABLE_CPP GLOBAL PROPERTY FORTE_EXECUTABLE_CPP)
  add_executable (${TEST_FORTE_STATIC} ${SOURCE_EXECUTABLE_CPP})
  target_include_directories(${TEST_FORTE_STATIC} PRIVATE ${INCLUDE_DIRECTORIES})
  target_link_libraries (${TEST_FORTE_STATIC} forte-static)
  add_dependencies (${TEST_FORTE_STATIC} forte-static)
endif(FORTE_BUILD_STATIC_LIBRARY)

#######################################################################################
# Test if main can link to forte-shared
#######################################################################################
# Currently not working in windows
if(FORTE_BUILD_SHARED_LIBRARY AND NOT WIN32)
  SET(TEST_FORTE_SHARED "forte_test_shared")
  GET_PROPERTY(SOURCE_EXECUTABLE_CPP GLOBAL PROPERTY FORTE_EXECUTABLE_CPP)
  add_executable (${TEST_FORTE_SHARED} ${SOURCE_EXECUTABLE_CPP})
  target_include_directories(${TEST_FORTE_SHARED} PRIVATE ${INCLUDE_DIRECTORIES})
  target_link_libraries (${TEST_FORTE_SHARED} forte-shared)
  add_dependencies (${TEST_FORTE_SHARED} forte-shared)
endif(FORTE_BUILD_SHARED_LIBRARY AND NOT WIN32)