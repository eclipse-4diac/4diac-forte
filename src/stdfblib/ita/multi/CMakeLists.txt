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

forte_register_device(MultiDevice)

add_library(forte-stdfblib-ita-multi OBJECT
            MultiMGR.h MultiMGR.cpp MultiDevice.h MultiDevice.cpp utils.h
            utils.cpp
)
target_link_libraries(forte PRIVATE forte-stdfblib-ita-multi)
