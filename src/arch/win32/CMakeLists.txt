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

forte_register_architecture(Win32)

# TODO: should be replaced by better arch-independent mechanism
target_include_directories(forte-global INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)

add_subdirectory(serlayer)

target_sources(forte PRIVATE ../main.cpp)

option(FORTE_WINDOWS_XP_COMPAT "Use only APIs that still work on Windows XP" OFF)
forte_generate_config_flag_include(FORTE_WINDOWS_XP_COMPAT)

add_library(forte-arch-win32 OBJECT
    fortealloc.h
    forte_thread.cpp forte_thread.h
    forte_sem.cpp forte_sem.h
    forte_sync.h
    $<$<NOT:$<BOOL:${FORTE_WINDOWS_XP_COMPAT}>>:forte_sync.cpp>
    $<$<BOOL:${FORTE_WINDOWS_XP_COMPAT}>:forte_sync_xp.cpp>
    $<$<BOOL:${FORTE_EXTERNAL_TIMEHA}>:externtimeha.cpp>
    $<$<BOOL:${FORTE_EXTERNAL_TIMEHA}>:externtimeha.h>
    pctimeha.cpp pctimeha.h
    forte_specific_architecture.cpp
    ../forte_architecture_time.h
    winforte_printer.cpp
    forte_architecture_time.cpp
    ../forte_standard_time.cpp
    ../genforte_fileio.cpp
   $<$<BOOL:${FORTE_COM_ETH}>:win32socketinterf.cpp>
   $<$<BOOL:${FORTE_COM_ETH}>:win32socketinterf.h>
   $<$<BOOL:${FORTE_COM_ETH}>:../fdselecthand.cpp>
   $<$<BOOL:${FORTE_COM_ETH}>:../fdselecthand.h>
   $<$<BOOL:${FORTE_COM_ETH}>:../gensockhand.h>
   $<$<BOOL:${FORTE_COM_ETH}>:sockhand.h>
)

target_link_libraries(forte PRIVATE forte-arch-win32)

if(FORTE_COM_ETH)
   forte_register_handler(CFDSelectHandler sockhand.h)
endif(FORTE_COM_ETH)

target_link_libraries(forte-arch-win32 PUBLIC comctl32 rpcrt4 winmm ws2_32
version)
  
if (MINGW)
    #force MINGW to statically link libc and lib stdc++ so that no mingw dlls are needed for forte.exe
    target_link_options(forte-global INTERFACE -static-libgcc -static-libstdc++)
elseif(MSVC)
    target_compile_options(forte-global INTERFACE -wd4595)  #warning 4595: 'operator delete/delete[]': non-member operator new or delete functions may not be declared inline
    target_compile_options(forte-global INTERFACE -wd4290)  #warning 4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow). A function is declared using exception specification, which Visual C++ accepts but does not implement. (from new/delete warning)
    target_compile_definitions(forte-global INTERFACE _SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING) # silence C++20 codecvt facets deprecation warnings until a viable replacement is found to prevent repeated warnings during builds
    target_compile_definitions(forte-global INTERFACE NOMINMAX)  #needed that windows.h is not adding macros for MIN and MAX
    target_compile_options(forte-global INTERFACE /Zc:preprocessor)  # needed for __VA_OPT__
endif()
  
target_compile_definitions(forte-global INTERFACE WIN32_LEAN_AND_MEAN)  # needed for windows.h
target_compile_definitions(forte-global INTERFACE _CRT_SECURE_NO_DEPRECATE)
target_compile_definitions(forte-global INTERFACE _CRT_NONSTDC_NO_DEPRECATE)
target_compile_definitions(forte-global INTERFACE _SCL_SECURE_NO_WARNINGS)
target_compile_definitions(forte-global INTERFACE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(forte-global INTERFACE _NO_W32_PSEUDO_MODIFIERS)   #takes care of IN, OUT and OPTIONAL in mingw
target_compile_definitions(forte-global INTERFACE FORTE_USE_DEFAULT_NEW_AND_DELETE)

  
if (FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)
    target_compile_definitions(forte-global INTERFACE BOOST_TEST_NO_LIB)
endif (FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)

add_subdirectory(com)
