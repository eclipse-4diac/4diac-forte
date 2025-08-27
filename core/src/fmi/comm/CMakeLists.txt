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

option(FORTE_COM_fmu "Enable Forte Com Fmu" ON)
target_sources(forte-core PRIVATE
            $<$<BOOL:${FORTE_COM_fmu}>:${CMAKE_CURRENT_SOURCE_DIR}/fmuComLayer.cpp>
            $<$<BOOL:${FORTE_COM_fmu}>:${CMAKE_CURRENT_SOURCE_DIR}/fmuComLayer.h> fmuHandler.h
            fmuHandler.cpp
)
