#*******************************************************************************
# Copyright (c) 2010 - 2024 ACIN, Profactor GmbH, fortiss GmbH, Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Alois Zoitl, Gerhard Ebenhofer, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
#   Christoph Binder - add possibility to configure simulated time
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\win32)

forte_add_architecture(Win32)

if("${FORTE_ARCHITECTURE}" STREQUAL "Win32")

  add_subdirectory(serlayer)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
  
  forte_add_sourcefile_h(fortealloc.h ../forte_architecture_time.h)
  forte_add_sourcefile_hcpp(forte_thread forte_sem)
  forte_add_sourcefile_h(forte_sync.h)
  if(FORTE_WINDOWS_XP_COMPAT)
    forte_add_sourcefile_cpp(forte_sync_xp.cpp)
  else()
    forte_add_sourcefile_cpp(forte_sync.cpp)
  endif()
  forte_add_sourcefile_cpp(forte_specific_architecture.cpp winforte_printer.cpp forte_architecture_time.cpp ../forte_standard_time.cpp)
  forte_add_sourcefile_cpp(../genforte_fileio.cpp)
  forte_add_sourcefile_cpp(../genforte_realFunctions.cpp)
  
  forte_add_to_executable_cpp(../main)

  if(FORTE_BUILD_SHARED_LIBRARY)
    set(FORTE_EXTERNAL_TIMEHA OFF CACHE BOOL "Use External Time Handler")
   
    if(FORTE_EXTERNAL_TIMEHA)
      SET(EXTERNAL_TIMER_HANDLER_NAME "externalTimer")
      forte_add_timerhandler(CExternTimerHandler externtimeha ${EXTERNAL_TIMER_HANDLER_NAME})
      forte_add_sourcefile_hcpp(externtimeha)
      forte_set_default_timerhandler(${EXTERNAL_TIMER_HANDLER_NAME})
    endif(FORTE_EXTERNAL_TIMEHA)
  
  else(FORTE_BUILD_SHARED_LIBRARY)
      unset(FORTE_EXTERNAL_TIMEHA CACHE)
  endif(FORTE_BUILD_SHARED_LIBRARY)

  forte_add_timerhandler(CPCTimerHandler pctimeha)
  forte_add_sourcefile_hcpp(pctimeha)

  if(FORTE_COM_ETH)
   forte_add_handler(CFDSelectHandler sockhand)
   forte_add_sourcefile_hcpp(  win32socketinterf ../fdselecthand)
   forte_add_sourcefile_h(../gensockhand.h)
   forte_add_sourcefile_h(sockhand.h)
  endif(FORTE_COM_ETH)

  forte_add_definition("-DFORTE_LITTLE_ENDIAN")

# TODO this flag was originally wrongly added to the definitions. May be necessary to be added as link flag. However currently working fine.
# forte_add_link_flags("/NODEFAULTLIB:MSVCRT.lib")

  forte_add_link_library(comctl32)
  forte_add_link_library(rpcrt4)
  forte_add_link_library(winmm)
  forte_add_link_library(ws2_32)
  forte_add_link_library(version)
  
  if(MINGW)
    #force MINGW to statically link libc and lib stdc++ so that no mingw dlls are needed for forte.exe
    forte_add_link_flags("-static-libgcc" "-static-libstdc++")
    forte_add_definition("-W -Wall -Wextra -Woverloaded-virtual -Wconversion -Wshadow")
  elseif(MSVC)
    forte_add_definition("-wd4595")  #warning 4595: 'operator delete/delete[]': non-member operator new or delete functions may not be declared inline
    forte_add_definition("-wd4290")  #warning 4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow). A function is declared using exception specification, which Visual C++ accepts but does not implement. (from new/delete warning)
    forte_add_definition("-DNOMINMAX")  #needed that windows.h is not adding macros for MIN and MAX
  endif()
  
  forte_add_definition("-DWIN32_LEAN_AND_MEAN")  # needed for windows.h
  forte_add_definition("-D_CRT_SECURE_NO_DEPRECATE")
  forte_add_definition("-D_CRT_NONSTDC_NO_DEPRECATE")
  forte_add_definition("-D_SCL_SECURE_NO_WARNINGS")
  forte_add_definition("-D_CRT_SECURE_NO_WARNINGS")
  forte_add_definition("-D_NO_W32_PSEUDO_MODIFIERS")   #takes care of IN, OUT and OPTIONAL in mingw
  forte_add_definition("-DFORTE_USE_DEFAULT_NEW_AND_DELETE")

  
  if(FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)
    forte_add_definition("-DBOOST_TEST_NO_LIB")
  endif(FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)
  
 
endif()
