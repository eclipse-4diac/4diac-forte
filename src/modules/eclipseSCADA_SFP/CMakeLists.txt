#*******************************************************************************
# Copyright (c) 2014 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

#############################################################################
# Interface to the EclipseSCADA SFP 
#############################################################################

forte_add_network_layer(EclipseSCADA_SFP OFF "sfp" CES_SFP_Layer essfplayer "Layer for providing and receiving data to EclipseSCADA, DEPRECATED: Layer will be removed in 4diac FORTE 1.12.0")

SET(FORTE_ECLIPSE_SCADA_SFP_SERVER_LIB_DIR "" CACHE PATH "Path to EclipseSCADA SFP Server library directory (leave empty for installed source code)")

if(FORTE_COM_EclipseSCADA_SFP)

  message(WARNING, "EclipseSCADA_SFP communication layer is deprecated! It will be removed in the 4diac FORTE 1.12.0 release!")

  forte_add_sourcefile_hcpp(essfphandler) 

  forte_add_handler(CEclipseSCADASFPHandler essfphandler)

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