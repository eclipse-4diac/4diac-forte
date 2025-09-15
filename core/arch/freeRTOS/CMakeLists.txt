#/************************************************************************************
# Copyright (c) 2016, 2024 fortiss GmbH, HR Agrartechnik GmbH, Jose Cabral
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#  Guru Chandrasekhara - initial API and implementation and/or initial documentation
# * Jose Cabral - Cleaning
#   Christoph Binder - add possibility to configure simulated time
#   Franz Hoepfinger - add memory allocation config.
#                    - add stack size config.
# ************************************************************************************/


SET(SOURCE_GROUP ${SOURCE_GROUP}\\freertos)

forte_add_architecture(FreeRTOSLwIP)

if("${FORTE_ARCHITECTURE}" STREQUAL "FreeRTOSLwIP")

  option(FORTE_FREERTOS_PLUS_TCP "Use FreeRTOS-plus-TCP library instead of lwIP")
  if (FORTE_FREERTOS_PLUS_TCP)
    forte_add_definition("-DFORTE_FREERTOS_PLUS_TCP")
  endif()

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
  
  SET(FORTE_FREERTOS_MINIMAL_STACK_SIZE "15000" CACHE STRING "Allow to set a special stack size for forte threads")
  mark_as_advanced(FORTE_FREERTOS_MINIMAL_STACK_SIZE)
  forte_add_custom_configuration("#define configMINIMAL_STACK_SIZE_FORTE ${FORTE_FREERTOS_MINIMAL_STACK_SIZE}")
  
  SET(FORTE_FreeRTOSLwIP_INCLUDES "" CACHE STRING "Folders to include from the FreeRTOS OS and the LwIP stack")

  SET(FORTE_FreeRTOSLwIP_LIB_DIR "" CACHE PATH "Only for executable: ABSOLUTE path to FreeRTOSLwIP folder with object library FORTE_FreeRTOSLwIP_LIB")
  SET(FORTE_FreeRTOSLwIP_LIB "" CACHE STRING "Only for executable: FreeRTOSLwIP Library file in FORTE_FreeRTOSLwIP_DIR to be linked to forte")

  forte_add_sourcefile_hcpp(forte_thread forte_sync forte_sem)

  forte_add_timerhandler(CFreeRTOSTimerHandler freertostiha)
  forte_add_sourcefile_cpp(./forte_architecture_time.cpp freertostiha.cpp)

  forte_add_sourcefile_cpp(../common/forte_specific_architecture.cpp)
  forte_add_sourcefile_cpp(../genforte_printer.cpp)
  forte_add_sourcefile_cpp(../genforte_fileio.cpp)

  forte_add_to_executable_cpp(main)

  forte_add_include_system_directories(${FORTE_FreeRTOSLwIP_INCLUDES})

  if(FORTE_COM_ETH)
    forte_add_handler(CFDSelectHandler sockhand)
    forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
    forte_add_sourcefile_h(../gensockhand.h)
    forte_add_sourcefile_h(sockhand.h)

    if (FORTE_FREERTOS_PLUS_TCP)
      message(WARNING "The FreeRTOS-plus-TCP network stack does not (yet) support multicast communication")
    else()
      message("The lwIP network stack needs the following flags to be defined (please do it in lwipopts.h):")
      message("#define LWIP_COMPAT_SOCKETS 1")
      message("#define LWIP_IGMP 1 // multicast support")
      message("#define LWIP_TIMEVAL_PRIVATE 0 // This is optional. Set this flag if you get a compilation error about redefinition of struct timeval")
    endif()
  endif(FORTE_COM_ETH)

endif()
