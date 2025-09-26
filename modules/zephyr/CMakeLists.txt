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

# ############################################################################
# Zephyr template FBs
# ############################################################################

option(FORTE_MODULE_ZEPHYR "Support for the Zephyr template board" OFF)

if (NOT FORTE_MODULE_ZEPHYR)
    return()
endif ()

# ############################################################################
# ESP32 Ethernet Kit FBs
# ############################################################################

option(FORTE_MODULE_ESP32ETHERNETKIT "Support for the Esp32EthernetKit board" OFF)

if (NOT FORTE_MODULE_ESP32ETHERNETKIT)
    return()
endif ()

# ############################################################################
add_library(forte-zephyr
            types/ZephyrIOBase.h types/ZephyrIOBase.cpp types/ZephyrIO_fbt.h
            types/ZephyrIO_fbt.cpp types/Esp32EthernetKitIO_fbt.h
            types/Esp32EthernetKitIO_fbt.cpp
)
target_link_libraries(forte-zephyr PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-zephyr,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-zephyr>>)

add_subdirectory(handler)
add_subdirectory(handle)

install(TARGETS forte-zephyr EXPORT forte-export FILE_SET HEADERS)
