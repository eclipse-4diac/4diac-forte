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

forte_register_module(MLPI
                      OFF
                      "Process interface using the MLPI functions for Bosch Rexroth PLCs"
)

set(FORTE_COM_MLPI_DIR
    ""
    CACHE PATH "Path to MLPI src directory"
)
forte_set_process_interface("MLPI" IX QX)

forte_register_handler(CMLPIFaceProcessInterface::CIOHandler
                       processinterface.h
)

add_library(forte-modules-mlpi OBJECT processinterface.h processinterface.cpp)
target_link_libraries(forte PRIVATE forte-modules-mlpi)
target_include_directories(forte-modules-mlpi
                           PUBLIC ${FORTE_COM_MLPI_DIR}/mlpiCore/include
)
