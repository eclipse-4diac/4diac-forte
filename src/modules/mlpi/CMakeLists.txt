#*******************************************************************************
# * Copyright (c) 2016 fortiss GmbH 
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

###############################################################
# PiFace Raspberry Pi expansion board
###############################################################
forte_add_module(MLPI "Process interface using the MLPI functions for Bosch Rexroth PLCs")

SET(FORTE_COM_MLPI_DIR "" CACHE PATH "Path to MLPI src directory")


if(FORTE_MODULE_MLPI)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
  forte_add_sourcefile_hcpp(../../stdfblib/io/QX)
  forte_add_sourcefile_hcpp(processinterface)
  
  forte_add_include_system_directories( ${FORTE_COM_MLPI_DIR}/mlpiCore/include )
          
endif()
