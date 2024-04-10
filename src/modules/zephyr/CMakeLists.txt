#############################################################################
# Copyright (c) 2023 KT Elektronik GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#  Dirk Kaar - initial API and implementation and/or initial documentation
#############################################################################

if ("${FORTE_ARCHITECTURE}" STREQUAL "Zephyr")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#############################################################################
# Zephyr FBs Base
#############################################################################

add_subdirectory(handler)
add_subdirectory(handle)
forte_add_sourcefile_hcpp(types/ZephyrIOBase)

#############################################################################
# Zephyr template FBs
#############################################################################

forte_add_io(ZEPHYR "Support for the Zephyr template board")

if(FORTE_IO_ZEPHYR)

forte_add_sourcefile_hcpp(types/ZephyrIO_fbt)

endif(FORTE_IO_ZEPHYR)

#############################################################################
# ESP32 Ethernet Kit FBs
#############################################################################

forte_add_io(ESP32ETHERNETKIT "Support for the Esp32EthernetKit board")

if(FORTE_IO_ESP32ETHERNETKIT)

forte_add_sourcefile_hcpp(types/Esp32EthernetKitIO_fbt)

endif(FORTE_IO_ESP32ETHERNETKIT)

#############################################################################

endif("${FORTE_ARCHITECTURE}" STREQUAL "Zephyr")
