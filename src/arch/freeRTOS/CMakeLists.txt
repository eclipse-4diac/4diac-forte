#/************************************************************************************
# * Copyright (c) 2016 - 2018 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# * Guru Chandrasekhara - initial API and implementation and/or initial documentation
# * Jose Cabral - Cleaning
# ************************************************************************************/


SET(SOURCE_GROUP ${SOURCE_GROUP}\\freertos)

forte_add_architecture(FreeRTOSLwIP)

if("${FORTE_ARCHITECTURE}" STREQUAL "FreeRTOSLwIP")

  set(FORTE_LOGLEVEL "NOLOG" CACHE STRING "Loglevel to use" FORCE)
  SET(FORTE_FreeRTOSLwIP_INCLUDES "" CACHE STRING "Folders to include from the FreeRTOS OS and the LwIP stack")
  
  SET(FORTE_FreeRTOSLwIP_LIB_DIR "" CACHE PATH "Only for executable: ABSOLUTE path to FreeRTOSLwIP folder with object library FORTE_FreeRTOSLwIP_LIB")
  SET(FORTE_FreeRTOSLwIP_LIB "" CACHE STRING "Only for executable: FreeRTOSLwIP Library file in FORTE_FreeRTOSLwIP_DIR to be linked to forte")
  
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
  
  forte_add_sourcefile_h(fortealloc.h)
  forte_add_sourcefile_hcpp(forte_thread forte_sync freertostiha forte_sem forte_Init)
  forte_add_sourcefile_cpp(forte_architecture.cpp ../genforte_printer.cpp)
  
  if(FORTE_COM_ETH)
    forte_add_handler(CFDSelectHandler sockhand)
    forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
    forte_add_sourcefile_h(../gensockhand.h)
    forte_add_sourcefile_h(sockhand.h)
  endif(FORTE_COM_ETH)
  
  forte_add_to_executable_cpp(main)

  forte_add_definition("-DFORTE_LITTLE_ENDIAN")
  forte_add_definition("-D__USE_W32_SOCKETS") #to hide the FD functions

  forte_add_include_directories(../${CMAKE_CURRENT_SOURCE_DIR})
  
  forte_add_include_system_directories(${FORTE_FreeRTOSLwIP_INCLUDES})

  forte_add_definition(-mcpu=cortex-m3 -mthumb) #CPU dependant
  forte_add_definition("-ffunction-sections -fdata-sections -fstack-usage -Wall -specs=nano.specs -DLWIP_TIMEVAL_PRIVATE=0 -fno-threadsafe-statics -fno-rtti -fno-exceptions")
  

endif()
