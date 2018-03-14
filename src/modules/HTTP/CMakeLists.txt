#*******************************************************************************
# *******************************************************************************
# * Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Marc Jakobi
# *******************************************************************************/
# *******************************************************************************/
#############################################################################
# HTTP Com Layer (currently implements an HTTP client)
#############################################################################
forte_add_network_layer(HTTP OFF "http" CHttpComLayer httplayer "Enable an HTTP Com Layer")
if(FORTE_COM_HTTP)
  forte_add_sourcefile_hcpp(httpparser httpiplayer)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif(FORTE_COM_HTTP)
