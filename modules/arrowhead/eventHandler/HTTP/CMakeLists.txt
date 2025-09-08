# *******************************************************************************
# Copyright (c) 2019, fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral - initial implementation
# *******************************************************************************

# ############################################################################
# Arrowhead specific functional blocks for HTTP for event handler
# ############################################################################
if (NOT FORTE_COM_HTTP)
    return()
endif ()

target_sources(forte-arrowhead PRIVATE
            PublishEventHTTP.h PublishEventHTTP.cpp SubscribeEventHTTP.h
            SubscribeEventHTTP.cpp
)
