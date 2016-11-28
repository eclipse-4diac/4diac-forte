 #*******************************************************************************
# * Copyright (c) 2010 - 2015 ACIN, Profactor GmbH, AIT, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Michael Hofmann, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Thomas Strasser,
# *   Martin Melik Merkumians
# *     - initial API and implementation and/or initial documentation
# *******************************************************************************/

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(FORTE)

if(NOT (${CMAKE_HOST_SYSTEM_NAME} STREQUAL ${CMAKE_SYSTEM_NAME}))
    MESSAGE("Cross compiling")
    SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
    SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)
    SET(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")
    SET(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>")
endif(NOT (${CMAKE_HOST_SYSTEM_NAME} STREQUAL ${CMAKE_SYSTEM_NAME}))

SET(FORTE_BUILDSUPPORT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/buildsupport" CACHE PATH "forte build support directory.")
mark_as_advanced(FORTE_BUILDSUPPORT_DIRECTORY)
#http://www.varsanofiev.com/inside/eclipse_and_windows.htm

#include forte cmake-functions
INCLUDE(${FORTE_BUILDSUPPORT_DIRECTORY}/forte.cmake)

#######################################################################################
# Determine how to build FORTE
#######################################################################################
set(FORTE_BUILD_TYPE EXECUTABLE CACHE STRING "Build FORTE as an executable or as a library")
set_property(CACHE FORTE_BUILD_TYPE PROPERTY STRINGS EXECUTABLE STATIC_LIBRARY SHARED_LIBRARY)
mark_as_advanced(FORTE_BUILD_TYPE)

#######################################################################################
# Determine the loglevel
#######################################################################################
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWERCASE)
if(BUILD_TYPE_LOWERCASE STREQUAL "debug")
    if(FORTE_LOGLEVEL MATCHES "NOLOG")
        message("Log Level NOLOG not allowed if CMAKE_BUILD_TYPE is set to Debug. Log level was changed to LOGINFO.")
        set(FORTE_LOGLEVEL "LOGINFO" CACHE STRING "Loglevel to use" FORCE)
    endif(FORTE_LOGLEVEL MATCHES "NOLOG")
    if(NOT DEFINED FORTE_LOGLEVEL)
    	message("No Log Level was specified. Setting Log Level to Debug default LOGINFO.")
        set(FORTE_LOGLEVEL "LOGINFO" CACHE STRING "Loglevel to use" FORCE)
    endif(NOT DEFINED FORTE_LOGLEVEL)
else(BUILD_TYPE_LOWERCASE STREQUAL "debug")
    if(NOT (FORTE_LOGLEVEL MATCHES "NOLOG"))
        message("Log Level was changed to NOLOG as CMAKE_BUILD_TYPE is not set to Debug.")
    endif(NOT (FORTE_LOGLEVEL MATCHES "NOLOG"))
    set(FORTE_LOGLEVEL "NOLOG" CACHE STRING "Loglevel to use" FORCE)
endif(BUILD_TYPE_LOWERCASE STREQUAL "debug")
set_property(CACHE FORTE_LOGLEVEL PROPERTY STRINGS LOGDEBUG LOGERROR LOGWARNING LOGINFO NOLOG)
 
forte_add_definition("-D${FORTE_LOGLEVEL}")

SET(FORTE_TRACE_EVENTS OFF CACHE BOOL "FORTE will log the events received at and sent from function blocks")
mark_as_advanced(FORTE_TRACE_EVENTS)
if(FORTE_TRACE_EVENTS)
forte_add_definition("-DFORTE_TRACE_EVENTS")
endif(FORTE_TRACE_EVENTS)

set(FORTE_SUPPORT_QUERY_CMD ON CACHE BOOL "Enable support for the query management commands")
mark_as_advanced(FORTE_SUPPORT_QUERY_CMD)
if(FORTE_SUPPORT_QUERY_CMD)
  forte_add_definition("-DFORTE_SUPPORT_QUERY_CMD")
endif(FORTE_SUPPORT_QUERY_CMD)

######################################################################################
set(FORTE_SYSTEM_TESTS OFF CACHE BOOL "FORTE System Tests")
if(FORTE_SYSTEM_TESTS)
ENABLE_TESTING()
endif(FORTE_SYSTEM_TESTS)

#######################################################################################
SET(FORTE_STRINGDICTFIXEDMEMORY OFF CACHE BOOL "FORTE string dict will reallocate memory if necessary when this flag is turned off")
mark_as_advanced(FORTE_STRINGDICTFIXEDMEMORY)

if(FORTE_STRINGDICTFIXEDMEMORY)
  forte_add_definition("-DFORTE_STRING_DICT_FIXED_MEMORY")
endif(FORTE_STRINGDICTFIXEDMEMORY)

set(FORTE_SUPPORT_BOOT_FILE ON CACHE BOOL "Enable FORTE boot file loading on FORTE start-up")
mark_as_advanced(FORTE_SUPPORT_BOOT_FILE)
if(FORTE_SUPPORT_BOOT_FILE)
  forte_add_definition("-DFORTE_SUPPORT_BOOT_FILE")
  SET(FORTE_BootfileLocation "" CACHE STRING "Path to the bootfile, if same directory as forte executable leave empty, include trailing '/'!")
  mark_as_advanced(FORTE_BootfileLocation)
  SET(FORTE_BOOTFILELINEBUFSIZE "300" CACHE STRING "size of the line buffer for reading boot files")
endif(FORTE_SUPPORT_BOOT_FILE)

set(FORTE_SUPPORT_MONITORING ON CACHE BOOL "Enable FORTE monitoring functionalities")
mark_as_advanced(FORTE_SUPPORT_MONITORING)
if(FORTE_SUPPORT_MONITORING)
  forte_add_definition("-DFORTE_SUPPORT_MONITORING")
endif(FORTE_SUPPORT_MONITORING)

if (WIN32)
  if (MSVC)
    set(FORTE_ADDITIONAL_CXX_FLAGS "/MP " CACHE STRING "Additional compile flags appended to CMAKE_CXX_FLAGS.")
    mark_as_advanced(FORTE_ADDITIONAL_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FORTE_ADDITIONAL_CXX_FLAGS}")
  endif(MSVC)
endif(WIN32)


	
#######################################################################################
# Add subdirectories
#######################################################################################
ADD_SUBDIRECTORY(src)


#######################################################################################
# FORTE Tests
#######################################################################################
set(FORTE_TESTS OFF CACHE BOOL "Build Tests")
set(FORTE_TESTS_LINK_DIRS "" CACHE PATH "Test specific library directories")
set(FORTE_TESTS_INC_DIRS "" CACHE PATH "Test specific include directories")


IF(FORTE_TESTS)
    enable_testing()
    ADD_SUBDIRECTORY(tests)
ENDIF(FORTE_TESTS)


ADD_SUBDIRECTORY(systemtests)

#######################################################################################
# FORTE forte_config.h 
#######################################################################################

SET(FORTE_TicksPerSecond "1000" CACHE STRING "forte sticks per second")
mark_as_advanced(FORTE_TicksPerSecond)

SET(FORTE_TimeBaseUnitsPerSecond "1000000000" CACHE STRING "Defines the time base in units per second that will be used in the TIME data type, The default value 1000000000 means 1ns")
mark_as_advanced(FORTE_TimeBaseUnitsPerSecond)

SET(FORTE_EventChainEventListSize "256" CACHE STRING "forte eventchain event list size")
mark_as_advanced(FORTE_EventChainEventListSize)

SET(FORTE_EventChainExternalEventListSize "10"  CACHE STRING "forte eventchain external event list size")
mark_as_advanced(FORTE_EventChainExternalEventListSize)

SET(FORTE_CommunicationInterruptQueueSize "10" CACHE STRING "forte Communication interrupt queue size")
mark_as_advanced(FORTE_CommunicationInterruptQueueSize)

SET(FORTE_IPLayerRecvBufferSize "1500" CACHE STRING "FORTE ip layer recv buffer size")
mark_as_advanced(FORTE_IPLayerRecvBufferSize)

SET(FORTE_MGMCOMMANDPROTOCOL "DEV_MGR" CACHE STRING "forte management command protocol")
set_property(CACHE FORTE_MGMCOMMANDPROTOCOL PROPERTY STRINGS DEV_MGR)
mark_as_advanced(FORTE_MGMCOMMANDPROTOCOL)

SET(FORTE_MGM_MAX_SUPPORTED_NAME_HIERACHY "30" CACHE STRING "Max supported hierarchy that can be provided in a management commands")
mark_as_advanced(FORTE_MGM_MAX_SUPPORTED_NAME_HIERACHY)

SET(FORTE_STRINGDICTINITIALSTRINGBUFSIZE "8000" CACHE STRING "FORTE string dict's initial string buffer size")
mark_as_advanced(FORTE_STRINGDICTINITIALSTRINGBUFSIZE)

SET(FORTE_STRINGDICTINITIALMAXNROFSTRINGS "300" CACHE STRING "FORTE string dict's initial max nr of strings")
mark_as_advanced(FORTE_STRINGDICTINITIALMAXNROFSTRINGS)

if(FORTE_USE_64BIT_DATATYPES)
 SET(MAX_TimeBaseUnitsPerSecond 1000000000)
else(FORTE_USE_64BIT_DATATYPES)
    SET(MAX_TimeBaseUnitsPerSecond 1000000)
endif(FORTE_USE_64BIT_DATATYPES)

if(${FORTE_TimeBaseUnitsPerSecond} GREATER ${MAX_TimeBaseUnitsPerSecond})  
 MESSAGE("Forced FORTE_TimeBaseUnitsPerSecond to ${MAX_TimeBaseUnitsPerSecond}")
 SET(FORTE_TimeBaseUnitsPerSecond ${MAX_TimeBaseUnitsPerSecond})
endif(${FORTE_TimeBaseUnitsPerSecond} GREATER ${MAX_TimeBaseUnitsPerSecond})

forte_add_include_directories(${CMAKE_BINARY_DIR})
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/forte_config.h.in ${CMAKE_BINARY_DIR}/forte_config.new.h)
forte_replacefile_if_changed(${CMAKE_BINARY_DIR}/forte_config.new.h ${CMAKE_BINARY_DIR}/forte_config.h)
file(REMOVE ${CMAKE_BINARY_DIR}/forte_config.new.h)

#######################################################################################
# Check FORTE_ARCHITECTURE has a valid value
#######################################################################################
if("${FORTE_ARCHITECTURE}" STREQUAL "None")
  message(FATAL_ERROR "No valid architecture chosen! Please check FORTE_ARCHITECTURE.")
endif("${FORTE_ARCHITECTURE}" STREQUAL "None")


#######################################################################################
# Setup Forte-Executeable with all Functionblocks
#######################################################################################
GET_PROPERTY(SOURCE_CPP              GLOBAL PROPERTY FORTE_SOURCE_CPP)
LIST(APPEND SOURCE_FILES_TMP           ${SOURCE_CPP})
GET_PROPERTY(SOURCE_C                GLOBAL PROPERTY FORTE_SOURCE_C)
LIST(APPEND SOURCE_FILES_TMP           ${SOURCE_C})

#######################################################################################
# Setup Forte-Executeable with all Functionblocks
#######################################################################################
GET_PROPERTY(SOURCE_TEST_CPP              GLOBAL PROPERTY FORTE_TEST_SOURCE_CPP)
LIST(APPEND SOURCE_FILES_TMP           ${SOURCE_TEST_CPP})

# Resolve to absolute path, Remove duplicate files, 
FOREACH( FBLIB_FILE ${SOURCE_FILES_TMP})
	get_filename_component(mod_fblib_file ${FBLIB_FILE} ABSOLUTE)
  STRING(REGEX MATCH  ".*stringlist\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
  IF(NOT REGEX_STRINGS)
  STRING(REGEX MATCH  ".*forteinit\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
  ENDIF(NOT REGEX_STRINGS)
  IF(NOT REGEX_STRINGS)
    LIST(APPEND SOURCE_FILES ${mod_fblib_file})
  ENDIF()
ENDFOREACH(FBLIB_FILE)
LIST(REMOVE_DUPLICATES SOURCE_FILES)

#######################################################################################
# Generate stringlist for every source file
#######################################################################################
SET(FORTE_LINKED_STRINGDICT ON CACHE BOOL "FORTE will resolve references to the stringdict at link-stage and not compile-stage. This will reduce compiletime if the stringdict changes.")
mark_as_advanced(FORTE_LINKED_STRINGDICT)

SET(FORTE_MODIFY_SOURCES_ON_MISSING_GENERATED_INCLUDES ON CACHE BOOL "FORTE change the source-files if includes for the generated includes are missing.")
mark_as_advanced(FORTE_MODIFY_SOURCES_ON_MISSING_GENERATED_INCLUDES)

if(FORTE_LINKED_STRINGDICT)
  FOREACH( FBLIB_FILE ${SOURCE_FILES})
	  # Do not pars stringlist, as these files will be generated
	  STRING(REGEX MATCH  ".*stringlist\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
	  IF(NOT REGEX_STRINGS)
	    STRING(REGEX MATCH  ".*forteinit\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
	  ENDIF(NOT REGEX_STRINGS)
	  IF(NOT REGEX_STRINGS)
		# Just the File name
		STRING(REGEX REPLACE  ".*/" "" FBLIB_FILE_NAME ${FBLIB_FILE})
		STRING(REGEX REPLACE  "\\." "_gen." FBLIB_FILE_NAME ${FBLIB_FILE_NAME})		
    ADD_CUSTOM_COMMAND(OUTPUT ${FORTE_BINARY_DIR}/src_gen/${FBLIB_FILE_NAME} COMMAND ${CMAKE_COMMAND} -DFORTE_MODIFY_SOURCES_ON_MISSING_GENERATED_INCLUDES:STRING="${FORTE_MODIFY_SOURCES_ON_MISSING_GENERATED_INCLUDES}" -DFORTE_SOURCE_DIR:STRING="${FORTE_SOURCE_DIR}/src" -DFORTE_BINARY_DIR:STRING="${FORTE_BINARY_DIR}/src_gen" -DFBLIB_FILE:STRING="${FBLIB_FILE}" -P ${FORTE_BUILDSUPPORT_DIRECTORY}/generate_stringlist_include_files.cmake MAIN_DEPENDENCY ${FBLIB_FILE})
    set_source_files_properties(${FORTE_BINARY_DIR}/src_gen/${FBLIB_FILE_NAME} HEADER_FILE_ONLY true)
		#list(APPEND DEPENDENCY_FILES ${FORTE_BINARY_DIR}/${FBLIB_FILE_NAME})
    ENDIF(NOT REGEX_STRINGS)		
	ENDFOREACH(FBLIB_FILE)
  # this is a hack; add_custom_target is always run, but we only want to run builds on modified files, maybe only valid for visual studio projects
  add_library(forte_stringlist_externals STATIC ${SOURCE_FILES})
  ADD_DEPENDENCIES (forte forte_stringlist_externals)
	#ADD_CUSTOM_TARGET(forte_stringlist_externals SOURCES  ${SOURCE_FILES})	
endif(FORTE_LINKED_STRINGDICT)
