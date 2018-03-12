#*******************************************************************************
# * Copyright (c) 2016 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Milan Vathoopan, Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/ 
#############################################################################
# WAGO KBUS SIFBs
#############################################################################
forte_add_module(WagoKbus "Wago Kbus interface")
if(FORTE_MODULE_WagoKbus)

forte_add_handler(WagoPFCProcessInterface::CKBusHandler processinterface)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

##All Wago depedencies are added here
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
  forte_add_sourcefile_hcpp(../../stdfblib/io/IX)
  forte_add_sourcefile_hcpp( ../../stdfblib/io/QX) 
  forte_add_sourcefile_hcpp( ../../stdfblib/io/QW) 
  forte_add_sourcefile_hcpp( ../../stdfblib/io/IW) 
  forte_add_sourcefile_hcpp(processinterface) 
  forte_add_link_flags("-Wall") 
  forte_add_definition("-I../../../sysroot-target/usr/include")  
  forte_add_definition("-I../../../sysroot-target/usr/include/OsLinux")
  forte_add_definition("-I../../../sysroot-target/usr/include/dal") 
  
  forte_add_link_library(-ldal -llibloader -lpthread -lffi -ldbus-glib-1 -lglib-2.0 -lrt)
  forte_add_link_library(-ltypelabel -loslinux -ldbuskbuscommon)    


#TODO: Further dependencies are to be checked and added later

else("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
 #TODO add check if we are really building for wago
  MESSAGE("wAGO_KBUS is only supported for FORTE on Linux platforms. Disabled Module!")
    set(FORTE_MODULE_wAGO_KBUS OFF)
endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

endif(FORTE_MODULE_WagoKbus)
