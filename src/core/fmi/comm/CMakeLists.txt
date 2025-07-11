# *******************************************************************************
# Copyright (c) 2017 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral *    - initial API and implementation and/or
# initial documentation
# *******************************************************************************/
# ############################################################################
# core/fmi/comm
# ############################################################################

forte_register_com_layer(fmu ON "fmu" fmuComLayer fmuComLayer.h
                         "Enable Forte Com Fmu"
)

forte_register_handler(fmuHandler fmuHandler.h)

add_library(forte-core-fmi-comm OBJECT
            $<$<BOOL:${FORTE_COM_fmu}>:fmuComLayer.cpp>
            $<$<BOOL:${FORTE_COM_fmu}>:fmuComLayer.h> fmuHandler.h
            fmuHandler.cpp
)
target_link_libraries(forte PRIVATE forte-core-fmi-comm)
