# *******************************************************************************
# Copyright (c) 2017 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral - initial API and implementation and/or initial
# documentation
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

forte_register_module(uMIC
                      OFF
                      "Interacting with GPIOs using the umic library for MicroControl"
)

# ############################################################################
# uMIC
# ############################################################################

forte_set_process_interface("uMIC" IX QX)
target_link_libraries(forte PUBLIC umic.so)
add_library(forte-modules-umic OBJECT processinterface.h processinterface.cpp)
target_link_libraries(forte PRIVATE forte-modules-umic)
