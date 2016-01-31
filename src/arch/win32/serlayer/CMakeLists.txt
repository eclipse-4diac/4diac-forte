#*******************************************************************************
# * Copyright (c) 2013 ACIN
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Martin Melik-Merkumians - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_network_layer(SER OFF "ser" CWin32SerComLayer cwin32sercomlayer "Enable Forte serial line communication")
if(FORTE_COM_SER)
  forte_add_sourcefile_hcpp( cwin32sercomhandler )
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif(FORTE_COM_SER)