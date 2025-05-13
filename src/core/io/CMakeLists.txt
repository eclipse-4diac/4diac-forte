#*******************************************************************************
# Copyright (c) 2017 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Johannes Messmer
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/

include(../../stdfblib/io/functions.cmake)

option(FORTE_IO "Enable IO Modules" OFF)
if (NOT FORTE_IO)
    return()
endif()

add_subdirectory(configFB)
add_subdirectory(device)
add_subdirectory(eIO)
add_subdirectory(mapper)

add_library(forte-core-io OBJECT
    processinterfacefb.cpp
)
target_link_libraries(forte PRIVATE forte-core-io)

forte_set_process_interface("Modular IO" IE IX QX IB QB IW QW ID QD IL QL)
