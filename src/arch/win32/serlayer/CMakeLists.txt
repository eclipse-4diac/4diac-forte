# *******************************************************************************
# Copyright (c) 2013 ACIN This program and the accompanying materials are made
# available under the terms of the Eclipse Public License 2.0 which is
# available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Martin Melik-Merkumians - initial API and implementation
# and/or initial documentation
# *******************************************************************************/
forte_register_com_layer(SER OFF "ser" CWin32SerComLayer cwin32sercomlayer.h
                         "Enable Forte serial line communication")
if (NOT FORTE_COM_SER)
    return()
endif ()

add_library(forte-arch-win32-serlayer OBJECT
            cwin32sercomlayer.cpp
            cwin32sercomlayer.h
            cwin32sercomhandler.cpp
            cwin32sercomhandler.h
)
target_link_libraries(forte PRIVATE forte-arch-win32-serlayer)
