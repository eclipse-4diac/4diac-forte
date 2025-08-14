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

forte_register_module(PiFace OFF
                      "PiFace and PiFace like Raspberry PI expansion boards"
)

forte_set_process_interface("PiFace" IX QX)
add_library(forte-modules-piface OBJECT
            processinterface.h processinterface.cpp
            ../conmeleon_c1/spi/spidevice.h ../conmeleon_c1/spi/spidevice.cpp
)
target_link_libraries(forte PRIVATE forte-modules-piface)
