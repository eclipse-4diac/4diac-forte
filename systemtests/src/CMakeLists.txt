#*******************************************************************************
# Copyright (c) 2019 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Jose Cabral  - initial API and implementation and/or initial documentation
# *******************************************************************************/

target_sources(forte_systemtest PRIVATE
        GatheringTestStruct_dtp.h
        GatheringTestStruct_dtp.cpp
        MemberAccessTestStruct_dtp.h
        MemberAccessTestStruct_dtp.cpp
        $<$<AND:$<BOOL:${FORTE_COM_ETH}>,$<BOOL:${LINUX}>>:${CMAKE_CURRENT_SOURCE_DIR}/tcp_blackhole.cpp>
)

add_subdirectory(modules)
