#*******************************************************************************
# * Copyright (c) 2010 - 2014 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Micheal Hofmann, Alois Zoitl, Stanislav Meduna, Gerhard Ebenhofer, 
# *   Matthias Plasch
# *     - initial API and implementation and/or initial documentation
# *******************************************************************************/

set(FORTE_EXTERNAL_MODULES_DIRECTORY "" CACHE PATH "Path to a directory with FORTE modules located outside of the FORTE source code")

forte_create_modules_file(${CMAKE_CURRENT_SOURCE_DIR}/modules/ "${FORTE_EXTERNAL_MODULES_DIRECTORY}")
forte_add_subdirectory(arch)
forte_add_subdirectory(core)
forte_add_subdirectory(modules)
forte_add_subdirectory(stdfblib)

SET(SOURCE_GROUP core)	
forte_add_sourcefile_with_path_hcpp(${FORTE_BINARY_DIR}/stringlist)
set_source_files_properties(${FORTE_BINARY_DIR}/stringlist.h PROPERTIES GENERATED TRUE)
set_source_files_properties(${FORTE_BINARY_DIR}/stringlist.cpp PROPERTIES GENERATED TRUE)


if(${FORTE_BUILD_TYPE} STREQUAL STATIC_LIBRARY OR ${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)
  forte_add_sourcefile_with_path_hcpp(${FORTE_BINARY_DIR}/forteinit)
  set_source_files_properties(${FORTE_BINARY_DIR}/forteinit.h PROPERTIES GENERATED TRUE)
  set_source_files_properties(${FORTE_BINARY_DIR}/forteinit.cpp PROPERTIES GENERATED TRUE)
ENDIF(${FORTE_BUILD_TYPE} STREQUAL STATIC_LIBRARY OR ${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)

#######################################################################################
# Define global Include-directorys
#######################################################################################
GET_PROPERTY(INCLUDE_DIRECTORIES GLOBAL PROPERTY FORTE_INCLUDE_DIRECTORIES)
LIST(LENGTH INCLUDE_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES INCLUDE_DIRECTORIES)
  LIST(REVERSE INCLUDE_DIRECTORIES) # bugfix, for replaced include files
ENDIF(len GREATER 0)

GET_PROPERTY(INCLUDE_SYSTEM_DIRECTORIES GLOBAL PROPERTY FORTE_INCLUDE_SYSTEM_DIRECTORIES)
LIST(LENGTH INCLUDE_SYSTEM_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES INCLUDE_SYSTEM_DIRECTORIES)
  LIST(REVERSE INCLUDE_SYSTEM_DIRECTORIES) # bugfix, for replaced include files
ENDIF(len GREATER 0)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES})
INCLUDE_DIRECTORIES(SYSTEM ${INCLUDE_SYSTEM_DIRECTORIES})
INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(${FORTE_BINARY_DIR})
INCLUDE_DIRECTORIES(${FORTE_BINARY_DIR}/src_gen)
#######################################################################################
# Define directories for Libraries
#######################################################################################
GET_PROPERTY(LINK_DIRECTORIES GLOBAL PROPERTY FORTE_LINK_DIRECTORIES)
LIST(LENGTH LINK_DIRECTORIES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES LINK_DIRECTORIES)
ENDIF(len GREATER 0)
LINK_DIRECTORIES(${LINK_DIRECTORIES})

#######################################################################################
# Define directories for Libraries
#######################################################################################
GET_PROPERTY(DEFINITION GLOBAL PROPERTY FORTE_DEFINITION)
ADD_DEFINITIONS (${DEFINITION})

#######################################################################################
# Exclude FBs
#######################################################################################
set(FORTE_EXCLUDE_FBS "" CACHE STRING "Exclude Function blocks")
mark_as_advanced(FORTE_EXCLUDE_FBS)

IF(FORTE_EXCLUDE_FBS)
  STRING(REGEX MATCHALL "[^ ,;][^ ,;]*" FBLIST ${FORTE_EXCLUDE_FBS})

  FOREACH(FB ${FBLIST})
    forte_remove_sourcefile_h(${FB}.h)
    forte_remove_sourcefile_cpp(${FB}.cpp)
  ENDFOREACH(FB)
ENDIF(FORTE_EXCLUDE_FBS)

GET_PROPERTY(PLATFORM_REMOVES              GLOBAL PROPERTY FORTE_PLATFORM_REMOVES)
FOREACH(FB ${PLATFORM_REMOVES})
  forte_remove_sourcefile_h(${FB}.h)
  forte_remove_sourcefile_cpp(${FB}.cpp)
ENDFOREACH(FB)

#######################################################################################
# Setup Forte-Executeable with all Functionblocks
#######################################################################################

GET_PROPERTY(SOURCE_H              GLOBAL PROPERTY FORTE_SOURCE_H)
GET_PROPERTY(SOURCE_H_GROUP GLOBAL PROPERTY FORTE_SOURCE_H_GROUP)
LIST(APPEND SOURCE_FILES           ${SOURCE_H})
LIST(APPEND SOURCE_FILES_GROUP     ${SOURCE_H_GROUP})

GET_PROPERTY(SOURCE_CPP              GLOBAL PROPERTY FORTE_SOURCE_CPP)
GET_PROPERTY(SOURCE_CPP_GROUP_STRUCT GLOBAL PROPERTY FORTE_SOURCE_CPP_GROUP)
LIST(APPEND SOURCE_FILES             ${SOURCE_CPP})
LIST(APPEND SOURCE_FILES_GROUP       ${SOURCE_CPP_GROUP_STRUCT})

GET_PROPERTY(SOURCE_C                GLOBAL PROPERTY FORTE_SOURCE_C)
GET_PROPERTY(SOURCE_C_GROUP_STRUCT   GLOBAL PROPERTY FORTE_SOURCE_C_GROUP)
LIST(APPEND SOURCE_FILES             ${SOURCE_C})
LIST(APPEND SOURCE_FILES_GROUP       ${SOURCE_C_GROUP_STRUCT})

#FOREACH(FILE ${SOURCE_H})
  # Do not pars stringlist, as these files will be generated
#  STRING(REGEX MATCH  ".*stringlist\\.(cpp|h).*" REGEX_STRINGS ${FBLIB_FILE})
#  IF(NOT REGEX_STRINGS)
#  ENDIF(NOT REGEX_STRINGS)
#ENDFOREACH(FILE)

#############################################################################
# Configure Network Layers
#############################################################################

GET_PROPERTY(LAYER_CLASS GLOBAL PROPERTY FORTE_LAYER_CLASS)
GET_PROPERTY(LAYER_CONFIGNAME GLOBAL PROPERTY FORTE_LAYER_CONFIGNAME)
GET_PROPERTY(LAYER_FILENAME GLOBAL PROPERTY FORTE_LAYER_FILENAME)
LIST(LENGTH LAYER_CLASS       LAYER_CLASS_LEN)
math(EXPR LAYER_CLASS_LEN           ${LAYER_CLASS_LEN}-1)

SET(LAYERMANAGER_INCLUDE "")
SET(LAYERMANAGER_INSTANCE "if(false){\n  }\n")

FOREACH(POS RANGE ${LAYER_CLASS_LEN}-1)
  LIST(GET LAYER_CLASS      ${POS} CLASS) 
  LIST(GET LAYER_CONFIGNAME ${POS} CONFIGNAME)
  LIST(GET LAYER_FILENAME   ${POS} FILENAME)
  SET(LAYERMANAGER_INCLUDE  "${LAYERMANAGER_INCLUDE}#include<${FILENAME}>\n")
  SET(LAYERMANAGER_INSTANCE "${LAYERMANAGER_INSTANCE}  else if(0 == strcmp(\"${CONFIGNAME}\", pa_acLayerIdentifier)){\n    m_poNewLayer = new ${CLASS}(pa_poUpperLayer, pa_poComFB);\n  }\n")
ENDFOREACH(POS)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/core/cominfra/comlayersmanager.cpp.in ${CMAKE_BINARY_DIR}/core/cominfra/comlayersmanager_new.cpp)
forte_replacefile_if_changed(${CMAKE_BINARY_DIR}/core/cominfra/comlayersmanager_new.cpp ${CMAKE_BINARY_DIR}/core/cominfra/comlayersmanager.cpp)
file(REMOVE ${CMAKE_BINARY_DIR}/core/cominfra/comlayersmanager_new.cpp)

#############################################################################
# Configure Network Layers
#############################################################################

GET_PROPERTY(HANDLER_CLASS GLOBAL PROPERTY FORTE_HANDLER_CLASS)
GET_PROPERTY(HANDLER_FILENAME GLOBAL PROPERTY FORTE_HANDLER_FILENAME)
LIST(LENGTH HANDLER_CLASS       HANDLER_CLASS_LEN_ORIGINAL)

if(${HANDLER_CLASS_LEN_ORIGINAL} GREATER 0)

math(EXPR HANDLER_CLASS_LEN           ${HANDLER_CLASS_LEN_ORIGINAL}-1)

SET(HANDLERMANAGER_INCLUDE "")
SET(HANDLERMANAGER_INSTANCE "")
SET(HANDLERMANAGER_VARIABLES "")
  
SET(HANDLER_ID 1)
FOREACH(POS RANGE ${HANDLER_CLASS_LEN}-1)
  LIST(GET HANDLER_CLASS      ${POS} CLASS) 
  LIST(GET HANDLER_FILENAME   ${POS} FILENAME)
  SET(HANDLERMANAGER_INCLUDE  "${HANDLERMANAGER_INCLUDE}#include <${FILENAME}>\n")
  
  SET(HANDLERMANAGER_VARIABLES  "${HANDLERMANAGER_VARIABLES}const unsigned int ${CLASS}::handlerIdentifier = ${HANDLER_ID};\n")
  SET(HANDLERMANAGER_INSTANCE "${HANDLERMANAGER_INSTANCE}  pa_DeviceExecution.mRegisteredEventHandlers[${HANDLER_ID}].m_poHandler = new ${CLASS}(pa_DeviceExecution);\n")
  math(EXPR HANDLER_ID           ${HANDLER_ID}+1)
ENDFOREACH(POS)

endif(${HANDLER_CLASS_LEN_ORIGINAL} GREATER 0)

math(EXPR HANDLER_CLASS_LEN_ORIGINAL ${HANDLER_CLASS_LEN_ORIGINAL}+1)
SET(FORTE_NUMBER_OF_HANDLERS ${HANDLER_CLASS_LEN_ORIGINAL} PARENT_SCOPE)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/core/deviceExecutionHandlers.cpp.in ${CMAKE_BINARY_DIR}/core/deviceExecutionHandlers_new.cpp)
forte_replacefile_if_changed(${CMAKE_BINARY_DIR}/core/deviceExecutionHandlers_new.cpp ${CMAKE_BINARY_DIR}/core/deviceExecutionHandlers.cpp)
file(REMOVE ${CMAKE_BINARY_DIR}/core/deviceExecutionHandlers_new.cpp)


#############################################################################
# Load FORTE Extension Files
#############################################################################

GET_PROPERTY(EXTENSION_FILES GLOBAL PROPERTY FORTE_EXTENSION_FILES)
LIST(LENGTH EXTENSION_FILES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES EXTENSION_FILES)
  FOREACH(FILE ${EXTENSION_FILES})
	INCLUDE("${FILE}")
  ENDFOREACH(FILE)
ENDIF(len GREATER 0)

#############################################################################
# Add Files to source-group
#############################################################################
LIST(LENGTH SOURCE_FILES       SOURCE_LEN)
LIST(LENGTH SOURCE_FILES_GROUP SOURCE_GROUP_LEN)
math(EXPR SOURCE_LEN           ${SOURCE_LEN}-1)
math(EXPR SOURCE_GROUP_LEN     ${SOURCE_GROUP_LEN}-1)

FOREACH(POS RANGE ${SOURCE_LEN}-1)
  LIST(GET SOURCE_FILES        ${POS} FILE) 
  LIST(GET SOURCE_FILES_GROUP  ${POS} GROUP) 
  source_group(${GROUP} FILES  ${FILE})
ENDFOREACH(POS)

# remove duplicate entries
LIST(LENGTH SOURCE_FILES len)
IF(len GREATER 0)
  LIST(REMOVE_DUPLICATES SOURCE_FILES)
ENDIF(len GREATER 0)

SET(WRITE_FILE "")
FOREACH(FILE ${SOURCE_FILES})
  SET(WRITE_FILE "${WRITE_FILE}${FILE}\n")
ENDFOREACH(FILE)
FILE(WRITE ${CMAKE_BINARY_DIR}/file_list.txt "${WRITE_FILE}")

######################################################################################
# Architecutre to build forte on
######################################################################################
set(FORTE_ARCHITECTURE "None" CACHE STRING "Architecture to build FORTE on")
GET_PROPERTY(architectures GLOBAL PROPERTY FORTE_ARCHITECTURES)
list(SORT architectures)
set_property(CACHE FORTE_ARCHITECTURE PROPERTY STRINGS None ${architectures})


#######################################################################################
# Create Exe File
#######################################################################################
#ADD_EXECUTABLE (forte  ${SOURCE_CPP} ${SOURCE_H})

#######################################################################################
# Create Files
#######################################################################################
if(${FORTE_BUILD_TYPE} STREQUAL EXECUTABLE)
ADD_EXECUTABLE (forte  ${SOURCE_CPP} ${SOURCE_C} ${SOURCE_H})
ENDIF(${FORTE_BUILD_TYPE} STREQUAL EXECUTABLE)

if(${FORTE_BUILD_TYPE} STREQUAL STATIC_LIBRARY)
ADD_LIBRARY (forte STATIC ${SOURCE_CPP} ${SOURCE_C} ${SOURCE_H})
forte_add_definition("-DFORTE_STATIC_LIB")
ADD_DEFINITIONS(-DFORTE_STATIC_LIB)
ENDIF(${FORTE_BUILD_TYPE} STREQUAL STATIC_LIBRARY)

if(${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)
ADD_LIBRARY (forte SHARED ${SOURCE_CPP} ${SOURCE_C} ${SOURCE_H} )
forte_add_definition("-DFORTE_STATIC_LIB")
forte_add_definition("-DFORTE_SHARED_LIB")
ADD_DEFINITIONS(-DFORTE_STATIC_LIB)
ADD_DEFINITIONS(-DFORTE_SHARED_LIB)
ENDIF(${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_DEPRECATE)
    ADD_DEFINITIONS (-D_CRT_NONSTDC_NO_DEPRECATE)
    ADD_DEFINITIONS (-D_SCL_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
endif()

get_target_property(existing_link_flags forte LINK_FLAGS)
if(existing_link_flags)
  set_property(GLOBAL APPEND PROPERTY FORTE_LINK_FLAGS ${existing_link_flags})
endif()

GET_PROPERTY(link_flags GLOBAL PROPERTY FORTE_LINK_FLAGS)

set_target_properties(forte PROPERTIES LINK_FLAGS "${link_flags}")
 
if(${FORTE_BUILD_TYPE} STREQUAL EXECUTABLE)
install(TARGETS forte RUNTIME DESTINATION bin)
ENDIF(${FORTE_BUILD_TYPE} STREQUAL EXECUTABLE)

if(${FORTE_BUILD_TYPE} STREQUAL STATIC_LIBRARY)
install(TARGETS forte ARCHIVE DESTINATION bin)
ENDIF(${FORTE_BUILD_TYPE} STREQUAL STATIC_LIBRARY)

if(${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)
install(TARGETS forte ARCHIVE DESTINATION bin LIBRARY DESTINATION bin)
ENDIF(${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)

#######################################################################################
# Link Libraries to the Executable
#######################################################################################
# get Linker-Direcotries from Modules
get_property(LINK_LIBRARY GLOBAL PROPERTY FORTE_LINK_LIBRARY)
TARGET_LINK_LIBRARIES (forte ${LINK_LIBRARY} )

#######################################################################################
# Generate stringlist and modules
#######################################################################################

if(${FORTE_BUILD_TYPE} STREQUAL STATIC_LIBRARY OR ${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)	#No reason to do this if we're building an executable...
  Message("lib")
  ADD_CUSTOM_TARGET(forte_init_generator COMMAND ${CMAKE_COMMAND} -DFORTE_STATIC_LIB:STRING="TRUE" -DFORTE_BINARY_DIR:STRING="${CMAKE_BINARY_DIR}" -DFORTE_SOURCE_DIR:STRING="${CMAKE_SOURCE_DIR}" -DFORTE_USE_64BIT_DATATYPES:STRING="${FORTE_USE_64BIT_DATATYPES}" -DFORTE_USE_REAL_DATATYPE:STRING="${FORTE_USE_REAL_DATATYPE}" -DFORTE_USE_LREAL_DATATYPE:STRING="${FORTE_USE_LREAL_DATATYPE}" -DFORTE_SUPPORT_ARRAYS:STRING="${FORTE_SUPPORT_ARRAYS}" -DFORTE_LITTLE_ENDIAN:STRING=${FORTE_LITTLE_ENDIAN} -DFORTE_BIG_ENDIAN:STRING=${FORTE_BIG_ENDIAN} -DFORTE_LOGLEVEL:STRING="${FORTE_LOGLEVEL}" -P ${FORTE_BUILDSUPPORT_DIRECTORY}/generate_init.cmake)	#
  ADD_DEPENDENCIES (forte forte_init_generator)
  ADD_DEPENDENCIES (forte_init_generator forte_generate_modules_cmake_files)
endif(${FORTE_BUILD_TYPE} STREQUAL STATIC_LIBRARY OR ${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)

ADD_DEPENDENCIES (forte forte_generate_modules_cmake_files)
ADD_CUSTOM_TARGET(forte_generate_modules_cmake_files COMMAND ${CMAKE_COMMAND} -DFORTE_MODULE_DIR:STRING=${CMAKE_CURRENT_SOURCE_DIR}/modules/ -DFORTE_EXTERNAL_MODULE_DIR:STRING=${FORTE_EXTERNAL_MODULES_DIRECTORY} -P ${FORTE_BUILDSUPPORT_DIRECTORY}/generate_modules_cmake_file.cmake)
ADD_CUSTOM_TARGET(forte_stringlist_generator COMMAND ${CMAKE_COMMAND} -DFORTE_LINKED_STRINGDICT:STRING="${FORTE_LINKED_STRINGDICT}" -DFORTE_BINARY_DIR:STRING="${CMAKE_BINARY_DIR}" -DFORTE_SOURCE_DIR:STRING="${CMAKE_CURRENT_SOURCE_DIR}" -P ${FORTE_BUILDSUPPORT_DIRECTORY}/generate_stringlist.cmake)
ADD_DEPENDENCIES (forte forte_stringlist_generator)
ADD_DEPENDENCIES (forte_stringlist_generator forte_generate_modules_cmake_files)

#######################################################################################
# Generate stringlist for every source file
#######################################################################################
if(FORTE_LINKED_STRINGDICT)
	set(ENABLE_GENERATED_SOURCE_CPP ON)
	ADD_DEPENDENCIES (forte forte_stringlist_externals)
endif(FORTE_LINKED_STRINGDICT)

#######################################################################################
# Enable autogenerated source files
#######################################################################################
if(ENABLE_GENERATED_SOURCE_CPP)
	ADD_DEFINITIONS ("-DFORTE_ENABLE_GENERATED_SOURCE_CPP")
endif(ENABLE_GENERATED_SOURCE_CPP)


#######################################################################################
# Link everything together
#######################################################################################
# link libraries to executeable
GET_PROPERTY(LINK_LIBRARY GLOBAL PROPERTY MODULE_LINK_LIBRARY)
TARGET_LINK_LIBRARIES (forte ${LINK_LIBRARY} )


#######################################################################################
# Add post build commands
#######################################################################################
GET_PROPERTY(POST_BUILD_COM GLOBAL PROPERTY FORTE_POST_BUILD_COMMAND)
STRING(LENGTH "${POST_BUILD_COM}" LEN_POST_BUILD_COM)
if(NOT LEN_POST_BUILD_COM EQUAL 0)
  ADD_CUSTOM_COMMAND(TARGET forte POST_BUILD ${POST_BUILD_COM})
endif(NOT LEN_POST_BUILD_COM EQUAL 0)


