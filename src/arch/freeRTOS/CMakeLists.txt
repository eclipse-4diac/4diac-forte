#/************************************************************************************
# Copyright (c) 2016 - 2018 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#  Guru Chandrasekhara - initial API and implementation and/or initial documentation
# * Jose Cabral - Cleaning
# ************************************************************************************/


SET(SOURCE_GROUP ${SOURCE_GROUP}\\freertos)

forte_add_architecture(FreeRTOSLwIP)

if("${FORTE_ARCHITECTURE}" STREQUAL "FreeRTOSLwIP")

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  

  set(FORTE_LOGLEVEL "NOLOG" CACHE STRING "Loglevel to use" FORCE)
  SET(FORTE_FreeRTOSLwIP_INCLUDES "" CACHE STRING "Folders to include from the FreeRTOS OS and the LwIP stack")
  
  SET(FORTE_FreeRTOSLwIP_LIB_DIR "" CACHE PATH "Only for executable: ABSOLUTE path to FreeRTOSLwIP folder with object library FORTE_FreeRTOSLwIP_LIB")
  SET(FORTE_FreeRTOSLwIP_LIB "" CACHE STRING "Only for executable: FreeRTOSLwIP Library file in FORTE_FreeRTOSLwIP_DIR to be linked to forte")
  
  forte_add_sourcefile_h(fortealloc.h)
  forte_add_sourcefile_hcpp(forte_thread forte_sync freertostiha forte_sem forte_Init)
  forte_add_sourcefile_cpp(forte_architecture.cpp ../genforte_printer.cpp)
  forte_add_sourcefile_cpp(../genforte_realFunctions.cpp ./forte_architecture_time.cpp)
  
  forte_add_to_executable_cpp(main)
  
  forte_add_definition("-DFORTE_LITTLE_ENDIAN")
  
  forte_add_include_system_directories(${FORTE_FreeRTOSLwIP_INCLUDES})
  
  if(FORTE_COM_ETH)
    forte_add_handler(CFDSelectHandler sockhand)
    forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
    forte_add_sourcefile_h(../gensockhand.h)
    forte_add_sourcefile_h(sockhand.h)

    message("freeRTOS with LwIP arch needs the following flags to be defined (please do it in lwipopts.h):")
    message("#define LWIP_COMPAT_SOCKETS 1")
    message("#define LWIP_IGMP 1 //to use IP_ADD_MEMBERSHIP")
    message("#define LWIP_TIMEVAL_PRIVATE 0 //This is optional. Set this flag if you get a compilation error about redefinition of struct timeval")
  endif(FORTE_COM_ETH)
  
endif()
