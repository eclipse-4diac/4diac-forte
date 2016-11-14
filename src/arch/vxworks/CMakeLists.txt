#/*******************************************************************************
# * Copyright (c) 2016 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *  Jose Cabral - initial API and implementation and/or initial documentation
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\vxworks)

forte_add_architecture(VxWorks)

if("${FORTE_ARCHITECTURE}" STREQUAL "VxWorks")
  message("Set an environment vairable WIND_BASE to any value")
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  
  forte_add_sourcefile_hcpp(thread vxworkstimeha sync forte_sem)
  forte_add_sourcefile_cpp(main.cpp)
  
  #SET(ENV{WIND_HOME} "C:/WBOEM69_Rexroth")
  #SET(ENV{WIND_BASE} "${WIND_HOME}/customBosch/vxworks-6.9")
  #SET(ENV{WIND_USR} "${WIND_BASE}/target/usr")
  
  SET(WIND_HOME "C:/WBOEM69_Rexroth")
  SET(WIND_BASE "${WIND_HOME}/customBosch/vxworks-6.9")
  SET(WIND_USR "${WIND_BASE}/target/usr")
  
  set(CPU ATOM)
  set(TOOL_FAMILY gnu)
  set(TOOL gnu)
  set(VSB_DIR ${WIND_BASE}/target/lib)
  
  #for RTP (not tested)
  #forte_add_definition("-march=atom -mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X -fasm -Wall -MD -MP -D_VX_CPU=_VX_ATOM -D_VX_TOOL_FAMILY=gnu -D_VX_TOOL=gnu -LC:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/lib/usr/lib/pentium/ATOM/common -IC:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/usr/h/wrn/coreip")  
  
  #for DKM
  forte_add_definition(-DVXWORKS -march=atom -mmovbe -nostdlib -fno-builtin -fno-defer-pop -fno-implicit-fp -ansi -fno-zero-initialized-in-bss -D_WRS_KERNEL -Wall -MD -MP -DCPU=_VX_${CPU} -DTOOL_FAMILY=${TOOL_FAMILY} -DTOOL=${TOOL} -DIP_PORT_VXWORKS=69 -D_VSB_CONFIG_FILE=\"${VSB_DIR}/h/config/vsbConfig.h\")
  
  #-O2 -march=atom -mmovbe -nostdlib -fno-builtin -fno-defer-pop -fno-implicit-fp -fno-zero-initialized-in-bss -D_WRS_KERNEL -Wall -MD -MP -DCPU=_VX_ATOM -DTOOL_FAMILY=gnu -DTOOL=gnu -DIP_PORT_VXWORKS=69 -D_VSB_CONFIG_FILE=\"C:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/lib/h/config/vsbConfig.h\" -IC:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/h -IC:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/h/wrn/coreip -W -Wall -Wextra -Wconversion -DFORTE_LITTLE_ENDIAN -ggdb    
  
	#-DVXWORKS  -ansi   -D_TYPE_fpos_t=fpos_t  -include "C:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/h/vxWorks.h"
  
  #Add commands for creating target image
  set(CMAKE_NM ${BIN_HOME}/nmpentium.exe)
  SET(FORTE_TCLSH "C:/WBOEM69_Rexroth/setup/x86-win32/bin/tclsh.exe" CACHE STRING "tclsh command")
  
  forte_add_post_build_command(COMMAND ${CMAKE_NM} forte.vxe > nmFile.txt
                               COMMAND ${FORTE_TCLSH} ${WIND_BASE}/host/resource/hutils/tcl/munch.tcl -c pentium -tags ${VSB_DIR}/tags/pentium/ATOM/common/dkm.tags < nmFile.txt 1> ctdt.c 
                               COMMAND ${CMAKE_C_COMPILER} -march=atom -mmovbe -nostdlib -fno-builtin -fno-defer-pop -fno-implicit-fp -fdollars-in-identifiers -Wall -I${WIND_BASE}/target/h -I${WIND_BASE}/target/h/wrn/coreip -DCPU=_VX_${CPU} -DTOOL_FAMILY=${TOOL_FAMILY} -DTOOL=${TOOL} -D_WRS_KERNEL -DIP_PORT_VXWORKS=69 -D_VSB_CONFIG_FILE=\"${VSB_DIR}/h/config/vsbConfig.h\" -o ctdt.o ctdt.c 
                               COMMAND ${CMAKE_C_COMPILER} -r -nostdlib -Wl,-X -T ${WIND_BASE}/target/h/tool/gnu/ldscripts/link.OUT -o forte.out ctdt.o forte.vxe)
  
  forte_add_include_directories(${WIND_BASE}/target/h ${WIND_BASE}/target/h/wrn/coreip)
  
  if(FORTE_COM_ETH)
   forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
   forte_add_sourcefile_h(../gensockhand.h)
   forte_add_sourcefile_h(sockhand.h)
  endif(FORTE_COM_ETH)

  forte_add_definition("-W -Wall -Wextra -Woverloaded-virtual -Wconversion -DFORTE_LITTLE_ENDIAN -ggdb")
  
  set(FORTE_RTTI_AND_EXCEPTIONS FALSE CACHE BOOL "Enable RTTI and Exceptions")
  mark_as_advanced(FORTE_RTTI_AND_EXCEPTIONS)
  IF(NOT FORTE_RTTI_AND_EXCEPTIONS)
    forte_add_definition("-fno-rtti -fno-exceptions")
  ENDIF(NOT FORTE_RTTI_AND_EXCEPTIONS)
  
  set(FORTE_POSIX_GENERATE_MAP_FILE FALSE CACHE BOOL "Enable the generation of map files")
  mark_as_advanced(FORTE_POSIX_GENERATE_MAP_FILE)
  IF(FORTE_POSIX_GENERATE_MAP_FILE)
  	forte_add_link_flags("-Wl,-M -Wl,-Map -Wl,forte.map -Wl,-cref")
  ENDIF(FORTE_POSIX_GENERATE_MAP_FILE)	

endif()