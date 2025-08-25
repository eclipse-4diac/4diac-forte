# *******************************************************************************
# Copyright (c) 2025 Maximilian Scharf This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Maximilian Scharf - Initial implementation.
# ******************************************************************************/

target_sources(forte-core PRIVATE
            # eIO base class
            einputfb.h
            eConfigFB.h
            # eIO adapter (connecting IO + configuration)
            eGenAdapter_adp.h
            eGenAdapter_adp.cpp
            # eIO FBs
            eIX_fbt.h
            eIX_fbt.cpp
            eIXconfig_fbt.h
            eIXconfig_fbt.cpp
            eIW_fbt.h
            eIW_fbt.cpp
            eIWconfig_fbt.h
            eIWconfig_fbt.cpp
)
target_include_directories(forte-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
