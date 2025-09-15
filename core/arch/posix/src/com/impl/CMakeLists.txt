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

if (NOT FORTE_COM_ETH)
    return()
endif ()

target_sources(forte-core PRIVATE
        net.cpp
        net.h
        socket.cpp
        socket.h
        tcp.cpp
        tcp.h
        tcp_listen.cpp
        tcp_listen.h
        udp.cpp
        udp.h
        udp_listen.cpp
        udp_listen.h
)
