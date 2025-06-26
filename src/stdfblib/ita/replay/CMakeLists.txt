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

if (NOT FORTE_TRACE_CTF_REPLAY_DEBUGGING)
    return()
endif()

set(FORTE_REPLAY_DEVICE FALSE CACHE BOOL "Compile the replay Device")

if (FORTE_REPLAY_DEVICE)
    forte_add_definition("-DFORTE_REPLAY_DEVICE")
    forte_add_device(ReplayDevice "stdfblib/ita/replay/ReplayDevice" "ReplayDevice")
    forte_add_sourcefile_hcpp(deviceReplayer resourceReplayer ReplayDevice ReplayMGR)
    forte_add_link_library(${FORTE_TRACE_READER_LIB})
endif()
