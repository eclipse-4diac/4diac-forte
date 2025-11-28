# ############################################################################
# Copyright (c) 2018 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Ben Schneider - Initial implementation
# ############################################################################

if (NOT LINUX)
    return()
endif ()

option(FORTE_COM_TSN "Enable Forte Com Ethernet Time-Sensitive Networking for Pub-Sub" OFF)
if (NOT FORTE_COM_TSN)
    return()
endif ()

add_library(forte-com-tsn
        $<$<BOOL:${FORTE_COM_TSN}>:tsn_layer.cpp>
        $<$<BOOL:${FORTE_COM_TSN}>:tsn_layer.h> tsn_layer.h tsn_layer.cpp
)
target_link_libraries(forte-com-tsn PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-tsn,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-tsn>>)

install(TARGETS forte-com-tsn EXPORT forte-export FILE_SET HEADERS)
