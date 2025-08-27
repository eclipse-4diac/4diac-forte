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
        # eIO adapter (connecting IO + configuration)
        eGenAdapter_adp.cpp
        # eIO FBs
        eIX_fbt.cpp
        eIXconfig_fbt.cpp
        eIW_fbt.cpp
        eIWconfig_fbt.cpp
)
