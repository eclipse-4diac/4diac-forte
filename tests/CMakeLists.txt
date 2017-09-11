#*******************************************************************************
# * Copyright (c) 2011- 201 ACIN, nxtControl, Profactor GmbH, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Ingo Hegny, Alois Zoitl, Stanislav Meduna, Micheal Hofmann, 
# *   Martin Melik-Merkumians  - initial API and implementation and/or initial documentation
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
# set include directories
#######################################################################################
GET_PROPERTY(INCLUDE_DIRECTORIES GLOBAL PROPERTY FORTE_INCLUDE_DIRECTORIES)

forte_test_add_inc_system_directories(${FORTE_TESTS_INC_DIRS})

#######################################################################################
# set library path
#######################################################################################
  GET_PROPERTY(FORTE_TEST_LINK_DIRECTORIES GLOBAL PROPERTY FORTE_LINK_DIRECTORIES)
  forte_test_add_link_directories(${FORTE_TESTS_LINK_DIRS})

  forte_test_add_link_directories(${CMAKE_BINARY_DIR}/src)

  GET_PROPERTY(FORTE_TEST_SOURCE_H GLOBAL PROPERTY FORTE_SOURCE_H)
  
  SET_PROPERTY(GLOBAL PROPERTY FORTE_TEST_SOURCE_CPP "")

#######################################################################################
# add subdirectories
#######################################################################################

forte_test_add_sourcefile_cpp(forte_boost_tester.cpp)

forte_test_add_subdirectory(arch)
forte_test_add_subdirectory(core)
forte_test_add_subdirectory(stdfblib)


#######################################################################################
# include generated stringlist (initial values for stringdict)
#######################################################################################
SET_SOURCE_FILES_PROPERTIES(${FORTE_BINARY_DIR}/stringlist.cpp PROPERTIES GENERATED TRUE)
forte_test_add_sourcefile_with_path_cpp(${FORTE_BINARY_DIR}/stringlist.cpp)

#######################################################################################
# update includes
#######################################################################################

LIST(APPEND INCLUDE_DIRECTORIES ${FORTE_TEST_INC_DIRECTORIES})

LIST(LENGTH INCLUDE_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES INCLUDE_DIRECTORIES)
  LIST(REVERSE INCLUDE_DIRECTORIES) # bugfix, for replaced include files
ENDIF(len GREATER 0)

GET_PROPERTY(INCLUDE_TEST_SYSTEM_DIRECTORIES GLOBAL PROPERTY FORTE_TEST_INC_SYSTEM_DIRECTORIES)
LIST(LENGTH INCLUDE_TEST_SYSTEM_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES INCLUDE_TEST_SYSTEM_DIRECTORIES)
  LIST(REVERSE INCLUDE_TEST_SYSTEM_DIRECTORIES) # bugfix, for replaced include files
ENDIF(len GREATER 0)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES})
INCLUDE_DIRECTORIES(SYSTEM ${INCLUDE_TEST_SYSTEM_DIRECTORIES})
INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(${FORTE_BINARY_DIR})
INCLUDE_DIRECTORIES(${FORTE_BINARY_DIR}/src_gen)

#######################################################################################
# Setup Forte-Executeable with all Functionblocks
#######################################################################################

GET_PROPERTY(SOURCE_H              GLOBAL PROPERTY FORTE_TEST_SOURCE_H)
GET_PROPERTY(SOURCE_H_GROUP GLOBAL PROPERTY FORTE_TEST_SOURCE_H_GROUP)
#LIST(APPEND SOURCE_FILES           ${SOURCE_H})
#LIST(APPEND SOURCE_FILES_GROUP     ${SOURCE_H_GROUP})

GET_PROPERTY(SOURCE_CPP              GLOBAL PROPERTY FORTE_TEST_SOURCE_CPP)
GET_PROPERTY(SOURCE_CPP_GROUP_STRUCT GLOBAL PROPERTY FORTE_TEST_SOURCE_CPP_GROUP)
#LIST(APPEND SOURCE_FILES             ${SOURCE_CPP})
#LIST(APPEND SOURCE_FILES_GROUP       ${SOURCE_CPP_GROUP_STRUCT})


SET(WRITE_FILE "")
FOREACH(FILE ${SOURCE_CPP} ${SOURCE_H})
  SET(WRITE_FILE "${WRITE_FILE}${FILE}\n")
ENDFOREACH(FILE)
FILE(WRITE ${CMAKE_BINARY_DIR}/file_test_list.txt "${WRITE_FILE}")
  
#######################################################################################
# Create Exe File
#######################################################################################
GET_PROPERTY(DEFINITION GLOBAL PROPERTY FORTE_DEFINITION)

ADD_DEFINITIONS (${DEFINITION})
if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
ADD_DEFINITIONS (-DBOOST_TEST_DYN_LINK)
ADD_DEFINITIONS (-g -O0 --coverage -fno-inline -fno-elide-constructors)
ADD_DEFINITIONS (-fsanitize=address)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
ADD_DEFINITIONS (-DBOOST_TEST_NO_LIB)
ADD_DEFINITIONS (-D_CRT_SECURE_NO_DEPRECATE)
ADD_DEFINITIONS (-D_CRT_NONSTDC_NO_DEPRECATE)
ADD_DEFINITIONS (-D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")

ADD_DEFINITIONS (-DFORTE_USE_DEFAULT_NEW_AND_DELETE)

LINK_DIRECTORIES(${FORTE_TEST_LINK_DIRECTORIES})

ADD_EXECUTABLE (forte_test ${SOURCE_CPP} ${SOURCE_H})

add_dependencies(forte_test forte)

ADD_DEPENDENCIES (forte_test forte_stringlist_generator)

SET_TARGET_PROPERTIES(forte_test PROPERTIES LINKER_LANGUAGE CXX)

get_target_property(existing_link_flags_test forte_test LINK_FLAGS)
if(existing_link_flags_test)
  set_property(GLOBAL APPEND PROPERTY FORTE_TEST_LINK_FLAGS ${existing_link_flags_test})
endif()

GET_PROPERTY(link_flags_test GLOBAL PROPERTY FORTE_TEST_LINK_FLAGS)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
remove_definitions (-fno-rtti)
remove_definitions (-fno-exceptions)
add_definitions (-fexceptions)

set_target_properties(forte_test PROPERTIES LINK_FLAGS "${link_flags_test} --coverage -lpthread -fsanitize=address")
endif()

add_test(forte_test ${EXECUTABLE_OUTPUT_PATH}/forte_test)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    set(FORTE_TEST_CODE_COVERAGE_ANALYSIS OFF CACHE BOOL "Perform code coverage analyis with GCOV and presentation with LCOV")
    mark_as_advanced(FORTE_TEST_CODE_COVERAGE_ANALYSIS)
    if(FORTE_TEST_CODE_COVERAGE_ANALYSIS)
        INCLUDE(${FORTE_BUILDSUPPORT_DIRECTORY}/GCov.cmake)    
        SETUP_GCOV(TestCoverage forte_test coverage)
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
	ADD_DEFINITIONS ("-DFORTE_ENABLE_GENERATED_SOURCE_CPP")
endif(ENABLE_GENERATED_SOURCE_CPP)

#######################################################################################
# Link Libraries to the Executeable
#######################################################################################

get_property(LINK_TEST_LIBRARY GLOBAL PROPERTY FORTE_LINK_LIBRARY)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    SET_PROPERTY(GLOBAL APPEND PROPERTY LINK_TEST_LIBRARY "-lboost_unit_test_framework -lboost_prg_exec_monitor -lboost_test_exec_monitor")
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")


TARGET_LINK_LIBRARIES (forte_test ${LINK_TEST_LIBRARY} )


# check if correct...
GET_PROPERTY(LINK_DIRECTORIES GLOBAL PROPERTY FORTE_TEST_LINK_DIRECTORIES)
LIST(LENGTH LINK_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES LINK_DIRECTORIES)
ENDIF(len GREATER 0)
LINK_DIRECTORIES(${LINK_DIRECTORIES})

file(WRITE sources.log "${FORTE_TEST_SOURCE_CPP}" )

#else()
#
#  if(EXISTS ${FORTE_BINARY_DIR}/file_test_list.txt)
#    file(REMOVE ${FORTE_BINARY_DIR}/file_test_list.txt)
#  endif()
#
#endif(FORTE_TESTS)

#export FORTE_CPPFLAGS="-W -Wall -DLOGDEBUG -DFORTE_LITTLE_ENDIAN -DBOOST_TEST_DYN_LINK\
#   -I `pwd`/.. \
#   -I `pwd`/../../../src \
#   -I `pwd`/../../../src/arch \
#   -I `pwd`/../../../src/arch/posix \
#   -I `pwd`/../../../src/core \
#   -I `pwd`/../../../src/core/datatypes \
#   -I `pwd`/../../../src/fblib/events \
#   -I `pwd`/../../../src/fblib/ita \
#   -I `pwd`/../../../src/fblib/math \
#   -I `pwd`/../../../src/fblib/net \
#   -I `pwd`/../../../bin/posix \
#   -I /home/acon/Boost/include"
# export FORTE_CXXFLAGS="-O2 -ggdb"
# export FORTE_LIBS="-lpthread"
