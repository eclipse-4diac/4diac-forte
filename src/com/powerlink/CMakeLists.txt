#*******************************************************************************
# Copyright (c) 2012 - 2014 AIT, fortiss GmbH 
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_add_module(POWERLINK OFF "openPOWERLINK Service Interface Function Blocks")

#############################################################################
# COM Service Interface Function Blocks
#############################################################################

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
    forte_add_sourcefile_cpp(EplWrapper.cpp)  
  forte_add_definition( -D_CONSOLE -DWPCAP )
  
  forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Target/X86/Windows/WpdPack/Include)

  forte_add_link_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Target/X86/Windows/WpdPack/Lib)
  
  forte_add_sourcefile_with_path_cpp(${FORTE_MODULE_POWERLINK_LIB_DIR}/EplStack/trace.c)
  
  forte_add_link_library(openPOWERLINK.lib)
  forte_add_link_library(wpcap.lib)
  forte_add_link_library(iphlpapi.lib)

elseif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
  forte_add_sourcefile_cpp(EplWrapper.cpp)
  forte_add_link_library(-lpowerlink -lpcap -lrt)
else()
  MESSAGE("openPOWERLINK is only supported for FORTE on Windows and Linux platforms. Disabled Module!")
    set(FORTE_MODULE_POWERLINK OFF)
endif()

if(FORTE_MODULE_POWERLINK)  
  SET(FORTE_MODULE_POWERLINK_LIB_DIR "" CACHE PATH "Path to openPOWERLINK-V1.08.1 directory")
  SET(FORTE_MODULE_POWERLINK_TINYXML_DIR "" CACHE PATH "Path to TinyXML directory")
    
    forte_add_sourcefile_hcpp( POWERLINK_MN 
                   X20DO9321
                   X20DO9322
                 X20DI4653                 
                   X20DI9371
                   X20DI9372
                   X20AI4622
                   X20AO4622
                   X20AT2402
                 X20AT4222
                 X20DO4649
                 X20DO4623
                   EplXmlReader 
                   ModuleList 
                   ProcessImageMatrix )
    
    forte_add_sourcefile_h( EplWrapper.h )
    
    forte_add_link_directories(${FORTE_MODULE_POWERLINK_LIB_DIR})
    
    forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Include)
    forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/SharedBuff)
    forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/ObjDicts/CiA302-4_MN)
    forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Examples/X86/Generic/powerlink_user_lib)
  forte_add_sourcefile_with_path_cpp(${FORTE_MODULE_POWERLINK_LIB_DIR}/ObjDicts/CiA302-4_MN/EplApiProcessImageSetup.c)
  forte_add_sourcefile_with_path_cpp(${FORTE_MODULE_POWERLINK_LIB_DIR}/EplStack/EplTgtConio.c)
  
  forte_add_include_directories(${FORTE_MODULE_POWERLINK_TINYXML_DIR})
  forte_add_sourcefile_with_path_hcpp( ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinystr 
                     ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxml )

  forte_add_sourcefile_with_path_cpp( ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxmlerror.cpp 
                    ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxmlparser.cpp )
    
    forte_add_definition( -DCONFIG_POWERLINK_USERSTACK )
endif(FORTE_MODULE_POWERLINK)
