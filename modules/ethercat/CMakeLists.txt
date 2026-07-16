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
#   Zijun Tang - initial API and implementation
# *******************************************************************************/

if (NOT LINUX)
    return()
endif ()

option(FORTE_MODULE_ETHERCAT "Support for the modular ethercat system" OFF)

if (NOT FORTE_MODULE_ETHERCAT)
    return()
endif ()

# ############################################################################
# EtherCAT IO function blocks (PR 2 of 3)
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
            device/ec_module.h
            device/ec_module.cpp
            device/model/ec_model.h
            device/model/ec_model.cpp
            utils/esi_catalog.h
            utils/esi_catalog.cpp
            utils/esi_io_configurator.h
            utils/esi_io_configurator.cpp
            utils/esi_xml_utils.h
            types/ECBusAdapter.h
            types/ECBusAdapter.cpp
            types/ECDevice.h
            types/ECDevice.cpp
            types/ECModule.h
            types/ECModule.cpp
            types/ECCoupler.h
            types/ECCoupler.cpp
            types/GEN_ECCoupler_fbt.h
            types/GEN_ECCoupler_fbt.cpp
            types/GEN_ECDevice_fbt.h
            types/GEN_ECDevice_fbt.cpp
            types/GEN_ECModule_fbt.h
            types/GEN_ECModule_fbt.cpp
            types/ECController.h
            types/ECController.cpp
            structs/ECDeviceConfig.h
            structs/ECDeviceConfig.cpp
            structs/ECModuleConfig.h
            structs/ECModuleConfig.cpp
)
target_link_libraries(forte-ethercat PUBLIC forte-core)
target_link_libraries(forte-ethercat PRIVATE ethercat tinyxml)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-ethercat,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-ethercat>>)
install(TARGETS forte-ethercat EXPORT forte-export FILE_SET HEADERS)
