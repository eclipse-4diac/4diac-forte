#*******************************************************************************
# Copyright (c) 2013 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Martin Melik-Merkumians - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_network_layer(SER OFF "ser" CWin32SerComLayer cwin32sercomlayer "Enable Forte serial line communication")
if(FORTE_COM_SER)
  forte_add_handler(CWin32SerComHandler cwin32sercomhandler)
  forte_add_sourcefile_hcpp(cwin32sercomhandler)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif(FORTE_COM_SER)