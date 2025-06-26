#*******************************************************************************
# Copyright (c) 2025 Jörg Walter
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Jörg Walter
#     - full rewrite
# *******************************************************************************/

function(forte_add_modules dir)
    file(REAL_PATH "${dir}" absdir BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    file(RELATIVE_PATH reldir "${CMAKE_CURRENT_SOURCE_DIR}" "${absdir}")
    if (EXISTS ${absdir}/CMakeLists.txt AND NOT absdir STREQUAL ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
        if (reldir MATCHES "^\\.")
            message(STATUS "Adding External Module: ${dir}")
            string(MD5 bindir_hash "${absdir}")
            get_property(bindir_base TARGET forte PROPERTY BINARY_DIR)
            add_subdirectory(${absdir} ${bindir_base}/module-${bindir_hash})
        else()
            message(STATUS "Adding Internal Module: ${dir}")
            add_subdirectory(${dir})
        endif()
    else()
        file(GLOB dirs RELATIVE ${CMAKE_CURRENT_LIST_DIR} CONFIGURE_DEPENDS "${dir}/*")
        foreach(dir IN LISTS dirs)
            forte_add_modules(${dir})
        endforeach()
    endif()
endfunction()

macro(forte_register_module name default description)
    option(FORTE_MODULE_${name} "${description}" ${default})

    if (NOT FORTE_MODULE_${name})
        return()
    endif()

    foreach (dependency ${ARGN})
        if (NOT ${${dependency}})
            message(FATAL_ERROR "Module ${name} needs ${dependency} enabled")
        endif()
    endforeach()

    # FIXME: In the long run, we want to get rid of this
    file(GLOB_RECURSE subdirs CMakeLists.txt)
    foreach (dir IN LISTS subdirs)
        cmake_path(REMOVE_FILENAME dir)
        target_include_directories(forte-global INTERFACE ${dir})
    endforeach()
endmacro()

macro(forte_register_io name default description)
    option(FORTE_IO_${name} "${description}" ${default})

    if (NOT FORTE_IO_${name})
        return()
    endif()

    foreach (dependency FORTE_IO ${ARGN})
        if (NOT ${${dependency}})
            message(FATAL_ERROR "IO Module ${name} needs ${dependency} enabled")
        endif()
    endforeach()
endmacro()
