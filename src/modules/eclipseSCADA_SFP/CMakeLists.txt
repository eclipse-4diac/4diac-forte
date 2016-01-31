#*******************************************************************************
# * Copyright (c) 2014 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

#############################################################################
# Interface to the EclipseSCADA SFP 
#############################################################################

forte_add_network_layer(EclipseSCADA_SFP OFF "sfp" CES_SFP_Layer essfplayer "Layer for providing and receiving data to EclipseSCADA")

SET(FORTE_ECLIPSE_SCADA_SFP_SERVER_LIB_DIR "" CACHE PATH "Path to EclipseSCADA SFP Server library directory (leave empty for installed source code)")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(FORTE_COM_EclipseSCADA_SFP)
  forte_add_sourcefile_hcpp(essfphandler) 


  if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
        if(EXISTS ${FORTE_ECLIPSE_SCADA_SFP_SERVER_LIB_DIR})
          forte_add_include_directories( ${FORTE_ECLIPSE_SCADA_SFP_SERVER_LIB_DIR}/include )
          forte_add_link_directories( ${FORTE_ECLIPSE_SCADA_SFP_SERVER_LIB_DIR}/posix/src/.lib )      
        else(EXISTS ${FORTE_ECLIPSE_SCADA_SFP_SERVER_LIB_DIR})
            forte_add_include_directories( /usr/local/include/libeclipsescada-sfp-server-0.1 )
            forte_add_include_directories( /usr/include/libeclipsescada-sfp-server-0.1 )
            forte_add_include_directories( /usr/lib/libeclipsescada-sfp-server-0.1/include )
        endif(EXISTS ${FORTE_ECLIPSE_SCADA_SFP_SERVER_LIB_DIR})
        
        forte_add_link_library( -leclipsescada-sfp-server-0.1)
        forte_add_link_library( -levent)
        
    #elseif(FORTE_ARCHITECTURE_WIN32)
      #TODO implement this for the other architectures accordingly see modlayer for an example
       
  endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

endif(FORTE_COM_EclipseSCADA_SFP)