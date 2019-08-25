#/*******************************************************************************
# Copyright (c) 2016 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Jose Cabral - initial API and implementation and/or initial documentation
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\rcX)

forte_add_architecture(rcX)

if("${FORTE_ARCHITECTURE}" STREQUAL "rcX")

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

  set(FORTE_BUILD_STATIC_LIBRARY ON CACHE BOOL "Build FORTE as static library" FORCE)
  set(FORTE_BUILD_EXECUTABLE OFF CACHE BOOL "Build FORTE as an executable" FORCE)
  set(FORTE_LOGLEVEL "NOLOG" CACHE STRING "Loglevel to use" FORCE)

  set(FORTE_RCX_NETX_CPU NETX51 CACHE STRING "CPU where the netx is deployed")
  set_property(CACHE FORTE_RCX_NETX_CPU PROPERTY STRINGS NETX10 NETX50 NETX51 NETX100)
  string(TOLOWER ${FORTE_RCX_NETX_CPU} FORTE_RCX_NETX_CPU_low)
  string(REPLACE "x" "X" FORTE_RCX_NETX_CPU_FOLDER ${FORTE_RCX_NETX_CPU_low} )
  
  if ("${FORTE_RCX_NETX_CPU}" STREQUAL "NETX100")
    set(ARCH_NETX arm926ej-s)
  else("${FORTE_RCX_NETX_CPU}" STREQUAL "NETX10")
    set(ARCH_NETX arm966e-s)#$set(ARCH_NETX arm9e)
  endif()
  
  SET(FORTE_RCX_QUEUE_MAX "32" CACHE STRING "Max number of elements for the queue of the forte task")
  SET(FORTE_RCX_WAIT_QUEUE_MAX "32" CACHE STRING "Max number of elements for the incoming buffer queue of the forte task")
  SET(FORTE_RCX_POOL_MAX "32" CACHE STRING "Max number of elements for the pool for originated messages in the forte task")
  SET(FORTE_RCX_INCLUDES "" CACHE STRING "Folders to include from the rcX OS. It must have inside of it the following folders: API, Configuration, TLR, TCPIP, CPUs/netX{10,51,100,..}")
  
  forte_set_timer(rcXTimerHandler)
  forte_add_sourcefile_hcpp(forte_thread rcXUtilities forte_sync forte_sem forte_instance)
  forte_add_sourcefile_h(fortealloc.h ../forte_architecture_time.h)
  forte_add_sourcefile_cpp(forte_architecture.cpp ../genforte_printer.cpp)
  forte_add_sourcefile_cpp(../genforte_realFunctions.cpp forte_architecture_time.cpp)
 
  if(FORTE_COM_ETH)
   forte_add_sourcefile_hcpp(rcXSocketInterface)
   forte_add_sourcefile_h(sockhand.h)
  endif(FORTE_COM_ETH)

  forte_add_definition("-D__RCX__ -D__${FORTE_NETX_CPU} -c -mlong-calls -mcpu=${ARCH_NETX} -fshort-enums -gdwarf-2 -Wall -O0 -mapcs -mthumb-interwork -fsigned-char -mlittle-endian -marm -mno-apcs-frame    -ffunction-sections -fdata-sections -Wl,-gc-sections -msoft-float -mfpu=vfp -mfloat-abi=soft -mthumb-interwork -nostdlib -mcpu=arm966e-s -mthumb -T")# -static-libstdc++ -static-libgcc")# -mabi=apcs-gnu")# -xc")# 
  forte_add_definition("-W -Wall -Wextra -Woverloaded-virtual -Wconversion -DFORTE_LITTLE_ENDIAN -ggdb")
  forte_add_definition("-fno-threadsafe-statics")
  
  set(FORTE_RTTI_AND_EXCEPTIONS FALSE CACHE BOOL "Enable RTTI and Exceptions")
  mark_as_advanced(FORTE_RTTI_AND_EXCEPTIONS)
  IF(NOT FORTE_RTTI_AND_EXCEPTIONS)
    forte_add_definition("-fno-rtti -fno-exceptions")
  ENDIF(NOT FORTE_RTTI_AND_EXCEPTIONS)
  
  set(FORTE_RCX_GENERATE_MAP_FILE FALSE CACHE BOOL "Enable the generation of map files")
  mark_as_advanced(FORTE_RCX_GENERATE_MAP_FILE)
  IF(FORTE_RCX_GENERATE_MAP_FILE)
    forte_add_link_flags("-Wl,-M -Wl,-Map -Wl,forte.map -Wl,-cref")
  ENDIF(FORTE_RCX_GENERATE_MAP_FILE)  
  
  forte_add_include_system_directories(${FORTE_RCX_INCLUDES}/API)
  forte_add_include_system_directories(${FORTE_RCX_INCLUDES}/Configuration)
  forte_add_include_system_directories(${FORTE_RCX_INCLUDES}/TLR)
  forte_add_include_system_directories(${FORTE_RCX_INCLUDES}/CPUs/${FORTE_RCX_NETX_CPU_FOLDER})
  forte_add_include_system_directories(${FORTE_RCX_INCLUDES}/TCPIP)
  
  forte_add_custom_configuration("const unsigned int cg_forteQueueMax = ${FORTE_RCX_QUEUE_MAX};")
  forte_add_custom_configuration("const unsigned int cg_forteWaitingQueueMax = ${FORTE_RCX_WAIT_QUEUE_MAX}; ")
  forte_add_custom_configuration("const unsigned int cg_fortePoolMax = ${FORTE_RCX_POOL_MAX}; ")
  
endif()