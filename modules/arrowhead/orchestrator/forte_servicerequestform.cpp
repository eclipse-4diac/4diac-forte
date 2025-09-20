/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "forte_servicerequestform.h"

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(ServiceRequestForm, "ServiceRequestForm"_STRID);

CIEC_ServiceRequestForm::CIEC_ServiceRequestForm() :
    CIEC_STRUCT("ServiceRequestForm"_STRID, 6, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_ServiceRequestForm::scmElementTypes[] = {"ArrowheadSystem"_STRID,
                                                                    "ArrowheadCloud"_STRID,
                                                                    "ArrowheadService"_STRID,
                                                                    "ARRAY"_STRID,
                                                                    10,
                                                                    "WSTRING"_STRID,
                                                                    "ARRAY"_STRID,
                                                                    10,
                                                                    "PreferredProvider"_STRID,
                                                                    "ARRAY"_STRID,
                                                                    10,
                                                                    "WSTRING"_STRID};
const forte::StringId CIEC_ServiceRequestForm::scmElementNames[] = {
    "requesterSystem"_STRID,    "requesterCloud"_STRID,     "requestedService"_STRID,
    "orchestrationFlags"_STRID, "preferredProviders"_STRID, "requestedQoS"_STRID};
