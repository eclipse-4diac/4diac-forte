#*******************************************************************************
# Copyright (c) 2017 fortiss GmbH
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
#############################################################################
# core/fmi/comm
#############################################################################

if(FORTE_ENABLE_FMU)

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}) #to be included in the normal forte for comlayersmanager.cpp

  forte_add_network_layer(fmu ON "fmu" fmuComLayer fmuComLayer "Enable Forte Com Fmu") 
    
  forte_add_handler(fmuHandler fmuHandler)
  
  forte_add_sourcefile_hcpp(fmuHandler) 
    
endif(FORTE_ENABLE_FMU)





