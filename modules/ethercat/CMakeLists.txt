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
# ethercat FBs
# ############################################################################

add_library(forte-ethercat         
            handler/bus.h
            handler/bus.cpp
            slave/slave.h
            slave/slave.cpp
            slave/handle.h
            slave/handle.cpp
            slave/ec_device.h
            slave/ec_device.cpp
            slave/ec_module.h
            slave/ec_module.cpp
            slave/model/ec_model.h
            slave/model/ec_model.cpp
            utils/EsiFileParser.h
            utils/EsiFileParser.cpp
            types/ECBusAdapter.h
            types/ECBusAdapter.cpp
            types/ECSlave.h
            types/ECSlave.cpp
            types/ECModule.h
            types/ECModule.cpp
            types/ECCoupler.h
            types/ECCoupler.cpp
            types/GEN_ECCoupler_fbt.h
            types/GEN_ECCoupler_fbt.cpp
            types/GEN_ECSlave_fbt.h
            types/GEN_ECSlave_fbt.cpp
            types/GEN_ECModule_fbt.h
            types/GEN_ECModule_fbt.cpp
            types/ECMaster.h
            types/ECMaster.cpp
            structs/ECSlaveConfig.h
            structs/ECSlaveConfig.cpp
            structs/ECModuleConfig.h
            structs/ECModuleConfig.cpp
)
target_link_libraries(forte-ethercat PUBLIC forte-core)
target_link_libraries(forte-ethercat PRIVATE ethercat tinyxml)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-ethercat,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-ethercat>>)
install(TARGETS forte-ethercat EXPORT forte-export FILE_SET HEADERS)
