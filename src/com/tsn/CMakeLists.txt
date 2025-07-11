# ############################################################################
# Copyright (c) 2018 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Ben Schneider - Initial implementation
# ############################################################################

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

forte_register_com_layer(TSN
                         OFF
                         "tsn"
                         CTSNLayer
                         tsn_layer.h
                         "Enable Forte Com Ethernet Time-Sensitive Networking for Pub-Sub"
)
if (NOT FORTE_COM_TSN)
    return()
endif ()

add_library(forte-com-tsn OBJECT
            $<$<BOOL:${FORTE_COM_TSN}>:tsn_layer.cpp>
            $<$<BOOL:${FORTE_COM_TSN}>:tsn_layer.h> tsn_layer.h tsn_layer.cpp
)
target_link_libraries(forte PRIVATE forte-com-tsn)
