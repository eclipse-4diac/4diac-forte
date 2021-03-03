#*******************************************************************************
# Copyright (c) 2016 fortiss GmbH 
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

###############################################################
# PiFace Raspberry Pi expansion board
###############################################################
forte_add_module(PiFace OFF "PiFace and PiFace like Raspberry PI expansion boards")

if(FORTE_MODULE_PiFace)

  forte_add_handler(CPiFaceProcessInterface::CPiFaceIOHandler processinterface)

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
  forte_add_sourcefile_hcpp(../../stdfblib/io/QX)
  forte_add_sourcefile_hcpp(processinterface)
  forte_add_sourcefile_hcpp(../conmeleon_c1/spi/spidevice)
endif()
