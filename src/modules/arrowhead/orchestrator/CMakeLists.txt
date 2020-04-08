# *******************************************************************************
# Copyright (c) 2018, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Jose Cabral - initial implementation
# *******************************************************************************

#############################################################################
# Arrowhead specific functional blocks for the orchestrator
#############################################################################

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#datatype
forte_add_sourcefile_hcpp(forte_orchestrationform forte_preferredprovider forte_servicerequestform)

#Orchestrator
forte_add_sourcefile_hcpp(FieldsToPreferredProvider FieldsToServiceRequestForm OrchestratorRequestAdp RequestOrchestrationForm OrchestrationForm2OrchestrationForm)

if(FORTE_COM_HTTP)
  add_subdirectory(HTTP)
endif(FORTE_COM_HTTP)

if(FORTE_COM_OPC_UA)
  add_subdirectory(OpcUa)
endif(FORTE_COM_OPC_UA)