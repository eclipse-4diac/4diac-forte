#*******************************************************************************
# * Copyright (c) 2010 - 2015 ACIN, Profactor GmbH, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *  Alois Zoitl, Gerhard Ebenhofer, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\win32)

forte_add_architecture(Win32)

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")

  add_subdirectory ( serlayer )
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
  
  forte_add_sourcefile_h(fortealloc.h)
  forte_add_sourcefile_hcpp(forte_thread forte_sync forte_sem)
  forte_add_sourcefile_cpp(main.cpp)

  if(${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)
    set(FORTE_EXTERNAL_TIMEHA ON CACHE BOOL "Use External Time Handler")
	if(FORTE_EXTERNAL_TIMEHA)
      forte_add_sourcefile_hcpp(externtimeha)
	else(FORTE_EXTERNAL_TIMEHA)
	  forte_add_sourcefile_hcpp(pctimeha)
	endif(FORTE_EXTERNAL_TIMEHA)
  else(${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)
    unset(FORTE_EXTERNAL_TIMEHA CACHE)
    forte_add_sourcefile_hcpp(pctimeha)
  endif(${FORTE_BUILD_TYPE} STREQUAL SHARED_LIBRARY)


  if(FORTE_COM_ETH)
   forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
   forte_add_sourcefile_h(../gensockhand.h)
   forte_add_sourcefile_h(sockhand.h)
  endif(FORTE_COM_ETH)

  forte_add_definition("-DFORTE_LITTLE_ENDIAN") # -DNONAMESPACE -DWITH_NONAMESPACES)  

# TODO this flag was originaly wrongly added to the definitions. May be necessary to be added as link flag. However currently working fine.
# forte_add_link_flags("/NODEFAULTLIB:MSVCRT.lib")

  forte_add_link_library(comctl32.lib)
  forte_add_link_library(rpcrt4.lib)
  forte_add_link_library(winmm.lib)
  forte_add_link_library(Ws2_32.lib)
  forte_add_link_library(version.lib)
  
 
endif()
