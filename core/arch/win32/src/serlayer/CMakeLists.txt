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
option(FORTE_COM_SER "Enable Forte serial line communication" OFF)
if (NOT FORTE_COM_SER)
    return()
endif ()

target_sources(forte-core PRIVATE
            cwin32sercomlayer.cpp
            cwin32sercomlayer.h
            cwin32sercomhandler.cpp
            cwin32sercomhandler.h
)
