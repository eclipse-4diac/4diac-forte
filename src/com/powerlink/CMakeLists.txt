#*******************************************************************************
# Copyright (c) 2014 - 2024 AIT, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
#    Michael Gafert - automatic download and linking of openPOWERLINK and tinyXML
# *******************************************************************************/

forte_add_module(POWERLINK OFF "openPOWERLINK Service Interface Function Blocks")

#############################################################################
# COM Service Interface Function Blocks
#############################################################################

if (NOT "${FORTE_ARCHITECTURE}" STREQUAL "Win32" AND NOT "${FORTE_ARCHITECTURE}" STREQUAL "Posix")
  MESSAGE(FATAL_ERROR "openPOWERLINK is only supported for FORTE on Windows and Linux platforms.")
endif ()

SET(FORTE_MODULE_POWERLINK_LIB_DIR "" CACHE PATH "Path to openPOWERLINK-V1.08 directory")
SET(FORTE_MODULE_POWERLINK_TINYXML_DIR "" CACHE PATH "Path to TinyXML directory")

if (FORTE_ARCHITECTURE STREQUAL "Win32")
  forte_add_definition(-DWPCAP)

  forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Target/X86/Windows/WpdPack/Include)
  forte_add_link_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Target/X86/Windows/WpdPack/Lib)
  forte_add_sourcefile_with_path_c(${FORTE_MODULE_POWERLINK_LIB_DIR}/EplStack/trace.c)

  forte_add_link_library(openPOWERLINK wpcap iphlpapi)
elseif (FORTE_ARCHITECTURE STREQUAL "Posix")
  forte_add_link_library(powerlink pcap rt)
endif ()

forte_add_sourcefile_cpp(EplWrapper.cpp)
forte_add_sourcefile_h(EplWrapper.h)

forte_add_sourcefile_hcpp(
        fb/PowerlinkFunctionBlockDI
        fb/PowerlinkFunctionBlockDO
        fb/PowerlinkFunctionBlockAI
        fb/PowerlinkFunctionBlockAO
        fb/PowerlinkFunctionBlockAT
        fb/POWERLINK_MN_fbt
        fb/X20AI2622_fbt
        fb/X20AI4622_fbt
        fb/X20AO4622_fbt
        fb/X20AT2402_fbt
        fb/X20AT4222_fbt
        fb/X20DI4653_fbt
        fb/X20DI9371_fbt
        fb/X20DI9372_fbt
        fb/X20DO4623_fbt
        fb/X20DO4649_fbt
        fb/X20DO9321_fbt
        fb/X20DO9322_fbt
        EplXmlReader
        ModuleList
        ProcessImageMatrix)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_startup_hook(powerlinkStartupHook)
forte_add_sourcefile_cpp(startup.cpp)

forte_add_link_directories(${FORTE_MODULE_POWERLINK_LIB_DIR})
forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Include)
forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/SharedBuff)
forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/ObjDicts/CiA302-4_MN)
forte_add_include_directories(${FORTE_MODULE_POWERLINK_LIB_DIR}/Examples/X86/Generic/powerlink_user_lib)
forte_add_sourcefile_with_path_c(${FORTE_MODULE_POWERLINK_LIB_DIR}/ObjDicts/CiA302-4_MN/EplApiProcessImageSetup.c)
forte_add_sourcefile_with_path_c(${FORTE_MODULE_POWERLINK_LIB_DIR}/EplStack/EplTgtConio.c)

forte_add_include_directories(${FORTE_MODULE_POWERLINK_TINYXML_DIR})
forte_add_sourcefile_with_path_hcpp(
        ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinystr
        ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxml
)

forte_add_sourcefile_with_path_cpp(
        ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxmlerror.cpp
        ${FORTE_MODULE_POWERLINK_TINYXML_DIR}/tinyxmlparser.cpp
)
