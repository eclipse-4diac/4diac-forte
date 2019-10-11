# *******************************************************************************
# * Copyright (c) 2019, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Jose Cabral - initial implementation
# *******************************************************************************

#############################################################################
# Arrowhead specific functional blocks for event handler
#############################################################################

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_hcpp(forte_arrowheadevent forte_publishevent forte_eventfilter)
forte_add_sourcefile_hcpp(FieldsToArrowheadEvent FieldsToPublishEvent FieldsToEventFilter)
forte_add_sourcefile_hcpp(ArrowheadPublish ArrowheadPublishAdp SubscribeEvent SubscribeEventAdp)

if(FORTE_COM_HTTP)
  add_subdirectory(HTTP)
endif(FORTE_COM_HTTP)
