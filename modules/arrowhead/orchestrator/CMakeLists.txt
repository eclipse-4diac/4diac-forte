# *******************************************************************************
# Copyright (c) 2018, fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral - initial implementation
# *******************************************************************************

# ############################################################################
# Arrowhead specific functional blocks for the orchestrator
# ############################################################################

add_subdirectory(HTTP)
add_subdirectory(OpcUa)

target_sources(forte-arrowhead PRIVATE
            # datatype
            forte_orchestrationform.h
            forte_orchestrationform.cpp
            forte_preferredprovider.h
            forte_preferredprovider.cpp
            forte_servicerequestform.h
            forte_servicerequestform.cpp
            # Orchestrator
            FieldsToPreferredProvider.h
            FieldsToPreferredProvider.cpp
            FieldsToServiceRequestForm.h
            FieldsToServiceRequestForm.cpp
            OrchestratorRequestAdp_adp.h
            OrchestratorRequestAdp_adp.cpp
            RequestOrchestrationForm.h
            RequestOrchestrationForm.cpp
            OrchestrationForm2OrchestrationForm_fbt.h
            OrchestrationForm2OrchestrationForm_fbt.cpp
)
