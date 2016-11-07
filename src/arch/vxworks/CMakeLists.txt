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

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  
  forte_add_sourcefile_hcpp(thread vxworkstimeha sync forte_sem)
  
  forte_add_definition("-march=atom -mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X -fasm -Wall -MD -MP -D_VX_CPU=_VX_ATOM -D_VX_TOOL_FAMILY=gnu -D_VX_TOOL=gnu -LC:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/lib/usr/lib/pentium/ATOM/common -IC:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/usr/h/wrn/coreip")
  set(CMAKE_SYSTEM_INCLUDE_PATH C:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/usr/h C:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/usr/h/wrn/coreip) 
  set(CMAKE_SYSTEM_LIBRARY_PATH C:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/lib/usr/lib/pentium/ATOM/common)
  set(CMAKE_SYSTEM_PROGRAM_PATH C:/WBOEM69_Rexroth/gnu/4.3.3-vxworks-6.9/x86-win32/bin )
  
  
  
  #SET(CMAKE_C_FLAGS -march=atom -mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X -std=c99 -fasm -Wall -MD -MP -D_VX_CPU=_VX_ATOM -D_VX_TOOL_FAMILY=gnu -D_VX_TOOL=gnu -LC:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/lib/usr/lib/pentium/ATOM/common)  
  #SET(CMAKE_CXX_FLAGS -march=atom	-mrtp -fno-strict-aliasing -D_C99 -D_HAS_C9X -LC:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/lib/usr/lib/pentium/ATOM/common -fasm -Wall -MD -MP -mrtp -lstdc++ -Wl,--start-group -Wl,--end-group)  

  #SET(CMAKE_C_LINK_FLAGS )  
  #SET(LIBS -lstdc++)
  #SET(ENV{WIND_USR} "C:/WBOEM69_Rexroth/customBosch/vxworks-6.9/target/usr")
  #SET(ENV{WIND_HOME} "C:/WBOEM69_Rexroth")
  #SET(ENV{WIND_BASE} "C:/WBOEM69_Rexroth/customBosch/vxworks-6.9")
  
  if(FORTE_COM_ETH)
   forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
   forte_add_sourcefile_h(../gensockhand.h)
   forte_add_sourcefile_h(sockhand.h)
  endif(FORTE_COM_ETH)

  #forte_add_link_library(pthread)

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
  
  #forte_add_link_library("-lpthread") 

endif()