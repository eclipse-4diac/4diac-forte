#*******************************************************************************
# Copyright (c) 2018, 2021, 2024 SYSGO GmbH, Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Agostino Mascitti - adaption to PikeOS 4.2 arm v7hf
#   Thomas Wagner -     adaption to PikeOS >=5.0 architecture independent
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\pikeos)

forte_add_architecture(PikeOS_Posix)

if("${FORTE_ARCHITECTURE}" STREQUAL "PikeOS_Posix")

   execute_process(COMMAND $ENV{CC} -dumpversion OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CCVERSION)
   execute_process(COMMAND $ENV{CC} -dumpmachine OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CCTARGET)
   string(TOUPPER $ENV{PIKEOS_ARCHITECTURE} ARCH_UPPERC)
   string(TOUPPER $ENV{PIKEOS_TARGET_ENDIAN} ENDIAN_UPPERC)
   set(PIKEOS_LWIP_LIB_PATH $ENV{PIKEOS_PROJECT})
   if("$ENV{POSIX_SMP}" STREQUAL "true")
      set(PROCCORE smp)
   else()
      set(PROCCORE up)
   endif()
   if("$ENV{POSIX_DEBUG}$ENV{POSIX_TRACE}" STREQUAL "truetrue")
      set(DEBUG "/debug-trace")
   elseif("$ENV{POSIX_DEBUG}" STREQUAL "true")
      set(DEBUG_VARIANT "/debug")
   elseif("$ENV{POSIX_TRACE}" STREQUAL "true")
      set(DEBUG_VARIANT "/trace")
   else()
      set(DEBUG_VARIANT "")
   endif()
  
  
   forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
   forte_add_sourcefile_hcpp(../posix/forte_sync ../utils/timespec_utils)
   forte_add_sourcefile_cpp(forte_thread.cpp pctimeha.cpp forte_sem.cpp forte_architecture_time.cpp
                            ../genforte_printer.cpp ../genforte_realFunctions.cpp)
   forte_add_sourcefile_h(../forte_architecture_time.h forte_thread.h pctimeha.h)
   forte_add_timerhandler(CPCTimerHandler pctimeha)
   forte_add_sourcefile_cpp(./forte_specific_architecture.cpp)

   if(FORTE_COM_ETH)
      forte_add_handler(CFDSelectHandler sockhand)
      forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
      forte_add_sourcefile_h(../gensockhand.h sockhand.h)
   endif(FORTE_COM_ETH)

   forte_add_include_directories(
        "$ENV{PIKEOS_POOL_DIR}/posix/include"
        "$ENV{PIKEOS_POOL_DIR}/posix/include/c++"
        "$ENV{PIKEOS_POOL_DIR}/include"
        "$ENV{PIKEOS_POOL_DIR}/include/kernel"
        "$ENV{PIKEOS_PREFIX}/cdk/$ENV{PIKEOS_ARCH}/$ENV{PIKEOS_PROC}/lib/gcc/${CCTARGET}/${CCVERSION}/include"
        "$ENV{PIKEOS_POOL_DIR}/posix/include/c++98"
        "${PIKEOS_LWIP_LIB_PATH}"
        "$ENV{PIKEOS_POOL_DIR}/posix/lwip/include"
   )

   forte_add_link_directories(
        "$ENV{PIKEOS_POOL_DIR}/posix/os/${PROCCORE}${DEBUG_VARIANT}"
        "$ENV{PIKEOS_POOL_DIR}/posix/lib"
        "$ENV{PIKEOS_POOL_DIR}/lib"
        "$ENV{PIKEOS_PREFIX}/cdk/$ENV{PIKEOS_ARCH}/$ENV{PIKEOS_PROC}/lib/gcc/${CCTARGET}/${CCVERSION}"
        "${PIKEOS_LWIP_LIB_PATH}"
   )

   # PikeOS POSIX libs and statically linked objects ("-lc" and "-lpse51" have to be included twice)
   forte_add_link_library (
        -lp4start -lstdcxx -lm -lc -lstand -lpse51
        -llwip -lsbuf -lgdbstubsig -lc -lpse51 -lvm -lp4
        -T$ENV{PIKEOS_POOL_DIR}/ldscript/app.ld
   )

   forte_add_definition("-nostdinc -W -Wall -Wextra -Woverloaded-virtual -Wconversion -ggdb -fno-builtin -DFORTE_${ENDIAN_UPPERC}_ENDIAN -D${ARCH_UPPERC} -DPOSIX_CXX -DPOSIX_LWIP -DPOSIX_${PROCCORE} -D${PROCCORE} -DPIKEOS_POSIX")
    
   set(FORTE_POSIX_GENERATE_MAP_FILE FALSE CACHE BOOL "Enable the generation of map files")
   mark_as_advanced(FORTE_POSIX_GENERATE_MAP_FILE)
   
   IF(FORTE_POSIX_GENERATE_MAP_FILE)
      forte_add_link_flags("-Wl,-M -Wl,-Map -Wl,forte.map -Wl,-cref")
   ENDIF(FORTE_POSIX_GENERATE_MAP_FILE)  
 
   forte_add_network_layer(SER OFF "ser" CPosixSerCommLayer ../posix/posixsercommlayer "Enable Forte serial line communication")
  
   set(FORTE_RTTI_AND_EXCEPTIONS FALSE CACHE BOOL "Enable RTTI and Exceptions")
   mark_as_advanced(FORTE_RTTI_AND_EXCEPTIONS)
  
   if("$ENV{POSIX_DEBUG}" STREQUAL "true")
      forte_add_definition("-g -O0 --coverage -fno-inline -fno-elide-constructors -fexceptions -fsanitize=address")
      forte_add_link_library("-fsanitize=address")
      forte_add_link_library("--coverage")
   else()
      IF(NOT FORTE_RTTI_AND_EXCEPTIONS)
        forte_add_definition("-fno-rtti -fno-exceptions")
      ENDIF(NOT FORTE_RTTI_AND_EXCEPTIONS)
   endif()
  
endif()
