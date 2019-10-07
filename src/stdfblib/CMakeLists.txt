#*******************************************************************************
# Copyright (c) 2010, 2011 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl - initial API and implementation and/or initial documentation
# *   Johannes Messmer - add IO directory
# *******************************************************************************/

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_GROUP ${SOURCE_GROUP}\\fblib)

forte_add_sourcefile_hcpp(timedfb)

add_subdirectory(events)
add_subdirectory(ita)
add_subdirectory(net)
