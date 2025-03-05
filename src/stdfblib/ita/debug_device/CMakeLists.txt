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


if (FORTE_COM_OPC_UA)
  set(FORTE_DEBUG_DEVICE FALSE CACHE BOOL "Compile the debug Device")
  if (FORTE_DEBUG_DEVICE)
    forte_add_device(DebugDevice "stdfblib/ita/debug_device/DebugDevice" "DebugDevice")
    forte_add_sourcefile_hcpp(DebugMGR DebugDevice)
  endif()
endif()
