#*******************************************************************************
# Copyright (c) 2019 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
#  Contributors:
#    Jose Cabral
#    - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_add_io(PLC01A1 "Support for X-NUCLEO-PLC01A")

if(FORTE_IO_PLC01A1)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_add_sourcefile_hcpp(plc01a1_controller plc01a1_config_fb)
endif(FORTE_IO_PLC01A1)
