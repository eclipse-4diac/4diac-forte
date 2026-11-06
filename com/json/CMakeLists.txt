# ############################################################################
# Copyright (c) 2026 Johannes Kepler University Linz
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Thomas Mayr - initial implementation and
# ############################################################################

option(FORTE_COM_JSON "Enable Forte JSON communication layer" OFF)
if(FORTE_COM_JSON)
    find_package(nlohmann_json CONFIG REQUIRED)
    target_link_libraries(forte-core PRIVATE nlohmann_json::nlohmann_json)
endif()

target_sources(forte-core PRIVATE
        $<$<BOOL:${FORTE_COM_JSON}>:${CMAKE_CURRENT_SOURCE_DIR}/jsonlayer.cpp>
		$<$<BOOL:${FORTE_COM_JSON}>:${CMAKE_CURRENT_SOURCE_DIR}/jsonlayer.h>
)
