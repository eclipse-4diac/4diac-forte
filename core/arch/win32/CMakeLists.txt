#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

forte_register_architecture(Win32 Windows)

option(FORTE_WINDOWS_XP_COMPAT "Use only APIs that still work on Windows XP" OFF)

target_link_libraries(forte-core PUBLIC comctl32 rpcrt4 winmm ws2_32 version)

if (MINGW)
    #force MINGW to statically link libc and lib stdc++ so that no mingw dlls are needed for forte.exe
    target_link_options(forte-core PUBLIC -static-libgcc -static-libstdc++)
elseif (MSVC)
    target_compile_options(forte-core PUBLIC -wd4595)  #warning 4595: 'operator delete/delete[]': non-member operator new or delete functions may not be declared inline
    target_compile_options(forte-core PUBLIC -wd4290)  #warning 4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow). A function is declared using exception specification, which Visual C++ accepts but does not implement. (from new/delete warning)
    target_compile_definitions(forte-core PUBLIC _SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING) # silence C++20 codecvt facets deprecation warnings until a viable replacement is found to prevent repeated warnings during builds
    target_compile_definitions(forte-core PUBLIC NOMINMAX)  #needed that windows.h is not adding macros for MIN and MAX
    target_compile_definitions(forte-core PUBLIC _ITERATOR_DEBUG_LEVEL=0) # needed to keep MSVC from complaining about allocating heap memory in constinit containers
    target_compile_options(forte-core PUBLIC /Zc:preprocessor)  # needed for __VA_OPT__
endif ()

target_compile_definitions(forte-core PUBLIC WIN32_LEAN_AND_MEAN)  # needed for windows.h
target_compile_definitions(forte-core PUBLIC _CRT_SECURE_NO_DEPRECATE)
target_compile_definitions(forte-core PUBLIC _CRT_NONSTDC_NO_DEPRECATE)
target_compile_definitions(forte-core PUBLIC _SCL_SECURE_NO_WARNINGS)
target_compile_definitions(forte-core PUBLIC _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(forte-core PUBLIC _NO_W32_PSEUDO_MODIFIERS)   #takes care of IN, OUT and OPTIONAL in mingw
target_compile_definitions(forte-core PUBLIC FORTE_USE_DEFAULT_NEW_AND_DELETE)

if (FORTE_WINDOWS_XP_COMPAT)
    target_compile_definitions(forte-core PUBLIC _WIN32_WINNT=0x0501) # Windows XP
else ()
    target_compile_definitions(forte-core PUBLIC _WIN32_WINNT=0x0600) # Windows Vista
endif ()

if (FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)
    target_compile_definitions(forte-core PUBLIC BOOST_TEST_NO_LIB)
endif (FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)

add_subdirectory(include)
add_subdirectory(src)
