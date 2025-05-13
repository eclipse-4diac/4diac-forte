#*******************************************************************************
# Copyright (c) 2010 - 2014 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Micheal Hofmann, Alois Zoitl, Stanislav Meduna, Gerhard Ebenhofer, 
# *   Matthias Plasch
# *     - initial API and implementation and/or initial documentation
# *******************************************************************************/

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# FIXME: this is way too many
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/arch)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/cominfra)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/datatypes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/datatypes/convert)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/arch/posix)

target_compile_definitions(forte-global INTERFACE FORTE_${CMAKE_CXX_BYTE_ORDER})

###############################################################################
# Core infrastructure

add_subdirectory(generated)

if (FORTE_BUILD_EXECUTABLE)
  add_executable(forte ${FORTE_GENERATED_FILES_DIR}/startuphook.cpp)
  install(TARGETS forte RUNTIME DESTINATION bin)
  message(STATUS "Building executable")
else()
  add_library(forte ${FORTE_GENERATED_FILES_DIR}/startuphook.cpp)
  install(TARGETS forte RUNTIME DESTINATION lib)
  message(STATUS "Building library")
endif()

add_subdirectory(core)
add_subdirectory(arch)
add_subdirectory(stdfblib)

###############################################################################
# Modules

function(forte_add_modules dir)
    file(REAL_PATH "${dir}" absdir BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    if (EXISTS ${absdir}/CMakeLists.txt)
        if (IS_ABSOLUTE ${dir})
            message(STATUS "Adding External Module: ${absdir}")
            string(MD5 bindir_hash "${absdir}")
            get_property(bindir_base TARGET fortelib PROPERTY BINARY_DIR)
            add_subdirectory(${absdir} ${bindir_base}/module-${bindir_hash})
        else()
            message(STATUS "Adding Internal Module: ${dir}")
            add_subdirectory(${dir})
        endif()
    else()
        message(STATUS "Searching Modules: ${dir}")
        file(GLOB dirs CONFIGURE_DEPENDS "${dir}/*")
        foreach(dir IN LISTS dirs)
            forte_add_modules(${dir})
        endforeach()
    endif()
endfunction()

macro(forte_register_module name default description)
  option(FORTE_MODULE_${name} "${description}" ${default})

  foreach (dependency IN LISTS ARGN)
    if (NOT ${dependency})
      return()
    endif()
  endforeach()
  if (NOT FORTE_MODULE_${name})
    return()
  endif()
endmacro()

forte_add_modules(com/HTTP)
forte_add_modules(modules/utils)
#forte_add_modules(modules)
#forte_add_modules(com)

set(FORTE_EXTERNAL_MODULES_DIRECTORY "" CACHE PATH "Path to a directory with FORTE modules located outside of the FORTE source code")
set(FORTE_EXTERNAL_MODULES_DIRECTORIES "" CACHE STRING "List of paths to directories with FORTE modules located outside of the FORTE source code")

if (FORTE_EXTERNAL_MODULES_DIRECTORY)
    forte_add_modules(${FORTE_EXTERNAL_MODULES_DIRECTORY})
endif()

if (FORTE_EXTERNAL_MODULES_DIRECTORIES)
    foreach(dir IN LISTS FORTE_EXTERNAL_MODULES_DIRECTORIES)
        file(REAL_PATH "${dir}" dir BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        forte_add_modules(${dir})
    endforeach()
endif()


###############################################################################
# Finalisation

# TODO: FORTE_EXTENSION_FILES

generate_typeinit(forte)
generate_stringlist(forte)

# TODO: FORTE_POST_BUILD

#[[

# function for configuring common things in both static and shared c libraries
FUNCTION(configure_c_library LIBRARY_NAME)
  target_sources(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/arch/c_interface/forte_c.cpp)
  set_target_properties(${TARGET_NAME}
    PROPERTIES PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/arch/c_interface/forte_c.h)

  install(TARGETS ${TARGET_NAME} 
      LIBRARY DESTINATION bin
      PUBLIC_HEADER DESTINATION include)
ENDFUNCTION()

if(FORTE_BUILD_STATIC_LIBRARY)
  message("Building static library")

  ADD_LIBRARY (${FORTE_STATIC_LIBRARY_NAME} STATIC)
  TARGET_LINK_LIBRARIES (${FORTE_STATIC_LIBRARY_NAME} PUBLIC ${FORTE_BASE_TARGET_NAME})

  if(FORTE_C_INTERFACE)
   configure_c_library(${FORTE_STATIC_LIBRARY_NAME})
  else()
    install(TARGETS ${FORTE_STATIC_LIBRARY_NAME} ARCHIVE DESTINATION bin)
  endif(FORTE_C_INTERFACE)

  endif(FORTE_BUILD_STATIC_LIBRARY)

if(FORTE_BUILD_SHARED_LIBRARY)
  message("Building shared library")

  if(WIN32)
    message(WARNING "Shared library in windows might not be linkable.")
  endif(WIN32)
  
  ADD_LIBRARY (${FORTE_SHARED_LIBRARY_NAME} SHARED)
  TARGET_LINK_LIBRARIES (${FORTE_SHARED_LIBRARY_NAME} PUBLIC ${FORTE_BASE_TARGET_NAME})
  set_target_properties(${FORTE_BASE_TARGET_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  set_target_properties(${FORTE_SHARED_LIBRARY_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)


  if(FORTE_C_INTERFACE)
    configure_c_library(${FORTE_SHARED_LIBRARY_NAME})

    # hide all symbols except the ones from the C interface
    # NOTE: This is still not working. The dynamic symbols are still being exported
    # --exclude-libs is an option for gcc but it does not work with OBJECT libraries
    # Exporting the dynamic symbols makes the loading time of the library longer
    # but it still can be used as usual
    set_target_properties(${FORTE_SHARED_LIBRARY_NAME} PROPERTIES 
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON)
    set_target_properties(${FORTE_SHARED_LIBRARY_NAME} PROPERTIES 
        C_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON)

    IF (WIN32 OR CYGWIN)
      SET(FORTE_SHARED_PREFIX "__declspec(dllexport)")
    else()
      SET(FORTE_SHARED_PREFIX "__attribute__((visibility(\"default\")))")
    endif()
    target_compile_definitions(${FORTE_SHARED_LIBRARY_NAME} PRIVATE FORTE_SHARED_PREFIX=${FORTE_SHARED_PREFIX})
  else()
    install(TARGETS ${FORTE_SHARED_LIBRARY_NAME} ARCHIVE DESTINATION bin)
  endif(FORTE_C_INTERFACE)
endif(FORTE_BUILD_SHARED_LIBRARY)


]]