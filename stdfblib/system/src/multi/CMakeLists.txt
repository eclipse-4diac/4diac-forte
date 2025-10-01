# *******************************************************************************
# Copyright (c) 2025 Jose Cabral This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral *    - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

if (NOT FORTE_COM_OPC_UA)
    return()
endif ()

option(FORTE_MULTI_DEVICE "Compile the multi Device" FALSE)

if (NOT FORTE_MULTI_DEVICE)
    return()
endif ()

target_sources(forte-system PRIVATE
        MultiMGR.h MultiMGR.cpp MultiDevice.h MultiDevice.cpp utils.h
        utils.cpp
)
