# *******************************************************************************
# Copyright (c) 2018 TU Wien/ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Martin Melik-Merkumians - inital contribution *    Martin
# Melik-Merkumians - adds IB, QB, IL, QL
# *******************************************************************************/

option(FORTE_MODULE_ADS "Interacting with ADS servers" OFF)

if (NOT FORTE_MODULE_ADS)
  return()
endif ()

message(FATAL_ERROR "This module has not yet been ported to 4diac FORTE 3.0")

set(FORTE_MODULE_ADS_PI_ADS_ROOT
    ""
    CACHE PATH "Path to ADS library source root directory"
)
find_library(ADS_LIBRARY libads.a)
target_link_libraries(forte PUBLIC ${ADS_LIBRARY})

# ############################################################################
# ADS interface
# ############################################################################

forte_set_process_interface("ADS"
                            IX
                            QX
                            IW
                            QW
                            ID
                            QD
                            IL
                            QL
)

add_library(forte-ads
            CAdsProcessInterface.h
            CAdsProcessInterface.cpp
            CAdsConnection.h
            CAdsConnection.cpp
            CAdsConnectionManager.h
            CAdsConnectionManager.cpp
            ADS_SERVER_CONFIG_fbt.h
            ADS_SERVER_CONFIG_fbt.cpp
            SET_LOCAL_ADS_ADDRESS_fbt.h
            SET_LOCAL_ADS_ADDRESS_fbt.cpp
            processinterface.h
)
target_link_libraries(forte-ads PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-ads,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-ads>>)
target_include_directories(forte-ads
                           PUBLIC ${FORTE_MODULE_ADS_PI_ADS_ROOT}/AdsLib
)

install(TARGETS forte-ads EXPORT forte-export FILE_SET HEADERS)
