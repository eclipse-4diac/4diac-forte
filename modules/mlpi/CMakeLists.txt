# *******************************************************************************
# Copyright (c) 2016 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl - initial API and implementation and/or initial
# documentation
# *******************************************************************************/

# ############################################################################
# PiFace Raspberry Pi expansion board
# ############################################################################
if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

option(FORTE_MODULE_MLPI "Process interface using the MLPI functions for Bosch Rexroth PLCs" OFF)

if (NOT FORTE_MODULE_MLPI)
    return()
endif ()

set(FORTE_COM_MLPI_DIR
    ""
    CACHE PATH "Path to MLPI src directory"
)
forte_set_process_interface("MLPI" IX QX)

add_library(forte-mlpi processinterface.h processinterface.cpp)
target_link_libraries(forte-mlpi PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-mlpi,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-mlpi>>)
target_include_directories(forte-mlpi
                           PUBLIC ${FORTE_COM_MLPI_DIR}/mlpiCore/include
)

install(TARGETS forte-mlpi EXPORT forte-export FILE_SET HEADERS)
