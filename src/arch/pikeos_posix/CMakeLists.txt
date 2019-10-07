#*******************************************************************************
# Copyright (c) 2018 SYSGO AG
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Agostino Mascitti - adaption to PikeOS 4.2
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\pikeos)

forte_add_architecture(PikeOS_Posix)

if("${FORTE_ARCHITECTURE}" STREQUAL "PikeOS_Posix")

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

  forte_add_sourcefile_hcpp(../posix/forte_sync ../posix/forte_sem)
  forte_add_sourcefile_cpp(main.cpp ../genforte_printer.cpp ../posix/forte_thread.cpp pctimeha.cpp)
  forte_add_sourcefile_h(../forte_architecture_time.h forte_thread.h pctimeha.h)
  forte_add_sourcefile_hcpp(../utils/timespec_utils)
  forte_add_sourcefile_cpp(../genforte_realFunctions.cpp ../posix/forte_architecture_time.cpp)
  

  if(FORTE_COM_ETH)
   forte_add_handler(CFDSelectHandler sockhand)
   forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
   forte_add_sourcefile_h(../gensockhand.h)
   forte_add_sourcefile_h(sockhand.h)
  endif(FORTE_COM_ETH)


set(PIKEOS_LWIP_LIB_PATH "" CACHE PATH "Path to LWIP build that supports SO_REUSEADDR")

  if(PIKEOS_LWIP_LIB_PATH STREQUAL "")
    message(WARNING "PIKEOS_LWIP_LIB_PATH is unset. The lwIP library is required by the PikeOS_Posix architecture.")
  endif(PIKEOS_LWIP_LIB_PATH STREQUAL "")


    forte_add_include_directories(
        "${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/posix/include"
        "${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/include"
        "${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/include/kernel"
        "${CMAKE_FIND_ROOT_PATH}/cdk/arm/v7hf/lib/gcc/arm-unknown-eabihf/5.4.0/include"
        "${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/posix/include/c++98"

        #LwIP includes
        "${PIKEOS_LWIP_LIB_PATH}"   #lwipopts.h is here

        "${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/posix/lwip/include"
    )

    forte_add_link_directories(
        "${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/posix/lib"
        "${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/posix/os/smp/debug"
        "${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/lib"
        "${CMAKE_FIND_ROOT_PATH}/cdk/arm/v7hf/lib/gcc/arm-unknown-eabihf/5.4.0"
        "${PIKEOS_LWIP_LIB_PATH}"

        #LwIP lib
        "${PIKEOS_LWIP_LIB_PATH}"
    )

    # PikeOS POSIX libs and statically linked objects
      forte_add_link_library("${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/posix/lib/cxx_init-c++98.o")
      forte_add_link_library("${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/posix/os/smp/crt0.o")

      forte_add_link_library ("-lstdcxx-c++98" "-llwip" "-lsbuf" "-lm" "-lc" "-lgdbstubsig" "-lpse51" "-lm" "-lc" "-lpse51" "-lvm" "-lstand" "-lp4" "-lgcc")

      # PikeOS linker script and custom link flags
      forte_add_link_flags( "-T${CMAKE_FIND_ROOT_PATH}/target/arm/v7hf/ldscript/app.ld")

  forte_add_definition("-nostdinc -W -Wall -Wextra -Woverloaded-virtual -Wconversion -ggdb -fno-builtin -DFORTE_LITTLE_ENDIAN -DARM_V7HF -DARM_CPU_V7 -DPOSIX_CXX -DPOSIX_LWIP -DPOSIX_SMP -DSMP -DPIKEOS_POSIX")
    
  set(FORTE_POSIX_GENERATE_MAP_FILE FALSE CACHE BOOL "Enable the generation of map files")
  mark_as_advanced(FORTE_POSIX_GENERATE_MAP_FILE)
  IF(FORTE_POSIX_GENERATE_MAP_FILE)
    forte_add_link_flags("-Wl,-M -Wl,-Map -Wl,forte.map -Wl,-cref")
  ENDIF(FORTE_POSIX_GENERATE_MAP_FILE)  
 
  forte_add_network_layer(SER OFF "ser" CPosixSerCommLayer ../posix/posixsercommlayer "Enable Forte serial line communication")
  
  set(FORTE_RTTI_AND_EXCEPTIONS FALSE CACHE BOOL "Enable RTTI and Exceptions")
  mark_as_advanced(FORTE_RTTI_AND_EXCEPTIONS)
  
  if(FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)
    forte_add_definition("-DBOOST_TEST_DYN_LINK -g -O0 --coverage -fno-inline -fno-elide-constructors -fexceptions -fsanitize=address")
    forte_add_link_library("-fsanitize=address")
    forte_add_link_library("--coverage")
  else()
    IF(NOT FORTE_RTTI_AND_EXCEPTIONS)
      forte_add_definition("-fno-rtti -fno-exceptions")
    ENDIF(NOT FORTE_RTTI_AND_EXCEPTIONS)
  endif(FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)
  


    
endif()