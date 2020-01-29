#*******************************************************************************
# Copyright (c) 2010 - 2018 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl, Patrick Smejkal, Martin Melik-Merkumians
# *    - initial API and implementation and/or initial documentation
# *    - added utils directory
# *******************************************************************************/
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(SOURCE_GROUP ${SOURCE_GROUP}\\arch)

add_subdirectory(be_m1)
add_subdirectory(ecos)
add_subdirectory(freeRTOS)
add_subdirectory(macos)
add_subdirectory(netos)
add_subdirectory(pikeos_posix)
add_subdirectory(plcnext)
add_subdirectory(posix)
add_subdirectory(rcX)
add_subdirectory(vxworks)
add_subdirectory(win32)

add_subdirectory(utils)

forte_add_sourcefile_hcpp(timerha devlog)

SET(FORTE_LOGGER_BUFFER_SIZE "300" CACHE STRING "Buffer's length of the logger")
mark_as_advanced(FORTE_LOGGER_BUFFER_SIZE)
forte_add_custom_configuration("#define FORTE_LOGGER_BUFFER_SIZE ${FORTE_LOGGER_BUFFER_SIZE}")

