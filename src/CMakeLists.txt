#*******************************************************************************
# Copyright (c) 2010 - 2014 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH, OFFIS e.V.
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Micheal Hofmann, Alois Zoitl, Stanislav Meduna, Gerhard Ebenhofer,
#     Matthias Plasch, OFFIS e.V.
# *******************************************************************************/

target_include_directories(forte-global INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

###############################################################################
# Core infrastructure

add_subdirectory(modules)


###############################################################################
# Modules

forte_add_modules(modules)

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
