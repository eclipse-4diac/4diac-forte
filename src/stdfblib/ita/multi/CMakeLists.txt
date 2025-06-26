#*******************************************************************************
# Copyright (c) 2025 Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Jose Cabral
# *    - initial API and implementation and/or initial documentation
# *******************************************************************************/

if (NOT FORTE_COM_OPC_UA)
    return()
endif()

set(FORTE_MULTI_DEVICE FALSE CACHE BOOL "Compile the multi Device")

if (FORTE_MULTI_DEVICE)
    forte_add_device(MultiDevice "stdfblib/ita/multi/MultiDevice" "MultiDevice")
    forte_add_sourcefile_hcpp(MultiMGR MultiDevice utils)
endif()
