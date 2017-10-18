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
  message(STATUS "VXWORKS: Set an environment variable WIND_BASE to any value")
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  
  forte_add_sourcefile_hcpp(forte_thread vxworkstimeha forte_sync forte_sem)
  forte_add_sourcefile_cpp(main.cpp ../genforte_printer.cpp)
  forte_add_sourcefile_h(../forte_architecture_time.h)
  
  SET(FORTE_VXWORKS_BASE_DIR "C:/WBOEM69_Rexroth/customBosch/vxworks-6.9" CACHE STRING "VxWorks base directory.")
  
  SET(FORTE_VXWORKS_FIND_DIR_AUTOMATICALLY ON CACHE BOOL "Selects the rest of needed directories from the BASE_DIR")
  SET(FORTE_VXWORKS_VSB_CONFIG_FILE "" CACHE STRING "VSB Configuration file")
  SET(FORTE_VXWORKS_MUNCH_TCL "" CACHE STRING "Munch tcl utility")
  SET(FORTE_VXWORKS_INCLUDE "" CACHE STRING "Include directories")    
  
  SET(FORTE_VXWORKS_TAGS "C:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/lib/tags/pentium/ATOM/common/dkm.tags" CACHE STRING "Tags file")

  #Add commands for creating target image
  get_filename_component(COMPILER_FOLDER ${CMAKE_CXX_COMPILER} DIRECTORY)
  set(FORTE_VXWORKS_CMAKE_NM "${COMPILER_FOLDER}/nmpentium.exe" CACHE STRING "Munch tcl utility")  
  set(FORTE_VXWORKS_TCLSH "C:/WBOEM69_Rexroth/setup/x86-win32/bin/tclsh.exe" CACHE STRING "tclsh command")
  
  set(FORTE_VXWORKS_CPU "ATOM" CACHE STRING "CPU of the hardware")
  set(FORTE_VXWORKS_TOOL_FAMILY "gnu" CACHE STRING "tool family used to compile")
  set(FORTE_VXWORKS_TOOL "gnu" CACHE STRING "tool used to compile")
  
  if (FORTE_VXWORKS_FIND_DIR_AUTOMATICALLY)
    set(FORTE_VXWORKS_VSB_CONFIG_FILE "${FORTE_VXWORKS_BASE_DIR}/target/lib/h/config/vsbConfig.h" CACHE STRING "VSB Configuration file" FORCE)
    set(FORTE_VXWORKS_MUNCH_TCL "${FORTE_VXWORKS_BASE_DIR}/host/resource/hutils/tcl/munch.tcl" CACHE STRING "Munch tcl utility" FORCE)
    set(FORTE_VXWORKS_INCLUDE "${FORTE_VXWORKS_BASE_DIR}/target/h;${FORTE_VXWORKS_BASE_DIR}/target/h/wrn/coreip" CACHE STRING "Include directories separated by ;" FORCE)  
  endif(FORTE_VXWORKS_FIND_DIR_AUTOMATICALLY)
  
  foreach(ARG ${FORTE_VXWORKS_INCLUDE}) 
     forte_add_include_system_directories(${ARG})
  endforeach()
  
  #Add commands for creating target image
  get_filename_component(COMPILER_FOLDER ${CMAKE_CXX_COMPILER} DIRECTORY)
  set(CMAKE_NM ${COMPILER_FOLDER}/nmpentium.exe)
  SET(FORTE_TCLSH "C:/WBOEM69_Rexroth/setup/x86-win32/bin/tclsh.exe" CACHE STRING "tclsh command")
 
  #for DKM
  forte_add_definition(-DVXWORKS -march=atom -mmovbe -nostdlib -fno-builtin -fno-defer-pop 
  -fno-implicit-fp -ansi -fno-zero-initialized-in-bss -D_WRS_KERNEL -Wall -MD -MP -DCPU=_VX_${FORTE_VXWORKS_CPU} -D_VX_CPU=_VX_${FORTE_VXWORKS_CPU}
  -DTOOL_FAMILY=${FORTE_VXWORKS_TOOL_FAMILY} -DTOOL=${FORTE_VXWORKS_TOOL} -DIP_PORT_VXWORKS=69 -D_VSB_CONFIG_FILE=\"${FORTE_VXWORKS_VSB_CONFIG_FILE}\")
  
  forte_add_post_build_command(COMMAND ${FORTE_VXWORKS_CMAKE_NM} forte.vxe > nmFile.txt
                               COMMAND ${FORTE_VXWORKS_TCLSH} ${FORTE_VXWORKS_MUNCH_TCL} -c pentium -tags ${FORTE_VXWORKS_TAGS} < nmFile.txt 1> ctdt.c 
                               COMMAND ${CMAKE_C_COMPILER} -march=atom -mmovbe -nostdlib -fno-builtin -fno-defer-pop -fno-implicit-fp -fdollars-in-identifiers -Wall -DCPU=_VX_${FORTE_VXWORKS_CPU} -D_VX_CPU=_VX_${FORTE_VXWORKS_CPU} -DTOOL_FAMILY=${FORTE_VXWORKS_TOOL_FAMILY} -DTOOL=${FORTE_VXWORKS_TOOL} -D_WRS_KERNEL -DIP_PORT_VXWORKS=69 -D_VSB_CONFIG_FILE=\"${FORTE_VXWORKS_VSB_CONFIG_FILE}\" -o ctdt.o ctdt.c 
                               COMMAND ${CMAKE_C_COMPILER} -r -nostdlib -Wl,-X -T ${FORTE_VXWORKS_BASE_DIR}/target/h/tool/${FORTE_VXWORKS_TOOL}/ldscripts/link.OUT -o forte.out ctdt.o forte.vxe)
  
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