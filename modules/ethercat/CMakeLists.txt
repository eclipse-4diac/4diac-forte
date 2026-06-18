# *******************************************************************************
# Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
# *******************************************************************************/

if (NOT LINUX)
    return()
endif ()

option(FORTE_MODULE_ETHERCAT "Support for the modular ethercat system" OFF)

if (NOT FORTE_MODULE_ETHERCAT)
    return()
endif ()

# ############################################################################
# EtherCAT core integration (IgH ecrt, bus handler, device layer)
# ############################################################################

add_library(forte-ethercat
            handler/bus.h
            handler/bus.cpp
            device/bus_device_handler.h
            device/bus_device_handler.cpp
            device/handle.h
            device/handle.cpp
            device/ec_device.h
            device/ec_device.cpp
            device/model/ec_model.h
            device/model/ec_model.cpp
)
target_link_libraries(forte-ethercat PUBLIC forte-core)
target_link_libraries(forte-ethercat PRIVATE ethercat)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-ethercat,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-ethercat>>)
install(TARGETS forte-ethercat EXPORT forte-export FILE_SET HEADERS)
