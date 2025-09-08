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
# Arrowhead specific functional blocks for event handler
# ############################################################################

add_subdirectory(HTTP)
add_subdirectory(OpcUa)

target_sources(forte-arrowhead PRIVATE
            forte_arrowheadevent.h
            forte_arrowheadevent.cpp
            forte_publishevent.h
            forte_publishevent.cpp
            forte_eventfilter.h
            forte_eventfilter.cpp
            FieldsToArrowheadEvent.h
            FieldsToArrowheadEvent.cpp
            FieldsToPublishEvent.h
            FieldsToPublishEvent.cpp
            FieldsToEventFilter.h
            FieldsToEventFilter.cpp
            ArrowheadPublish.h
            ArrowheadPublish.cpp
            ArrowheadPublishAdp_adp.h
            ArrowheadPublishAdp_adp.cpp
            SubscribeEvent.h
            SubscribeEvent.cpp
            SubscribeEventAdp_adp.h
            SubscribeEventAdp_adp.cpp
)
