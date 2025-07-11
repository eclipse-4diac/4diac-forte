# ############################################################################
# Copyright (c) 2023 KT Elektronik GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Dirk Kaar - initial API and implementation and/or initial
# documentation
# ############################################################################

if (NOT FORTE_ARCHITECTURE STREQUAL "Zephyr")
    return()
endif ()

# ############################################################################
# Zephyr FBs Base
# ############################################################################

add_subdirectory(handler)
add_subdirectory(handle)

# ############################################################################
# Zephyr template FBs
# ############################################################################

forte_register_io(ZEPHYR OFF "Support for the Zephyr template board")

# ############################################################################
# ESP32 Ethernet Kit FBs
# ############################################################################

forte_register_io(ESP32ETHERNETKIT OFF
                  "Support for the Esp32EthernetKit board"
)

# ############################################################################
add_library(forte-modules-zephyr OBJECT
            types/ZephyrIOBase.h types/ZephyrIOBase.cpp types/ZephyrIO_fbt.h
            types/ZephyrIO_fbt.cpp types/Esp32EthernetKitIO_fbt.h
            types/Esp32EthernetKitIO_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-zephyr)
