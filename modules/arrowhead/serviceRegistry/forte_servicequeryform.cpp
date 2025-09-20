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

#include "forte_servicequeryform.h"

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(ServiceQueryForm, "ServiceQueryForm"_STRID);

CIEC_ServiceQueryForm::CIEC_ServiceQueryForm() :
    CIEC_STRUCT("ServiceQueryForm"_STRID, 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_ServiceQueryForm::scmElementTypes[] = {"ArrowheadService"_STRID, "BOOL"_STRID, "BOOL"_STRID,
                                                                  "DINT"_STRID};
const forte::StringId CIEC_ServiceQueryForm::scmElementNames[] = {"service"_STRID, "metadataSearch"_STRID,
                                                                  "pingProviders"_STRID, "version"_STRID};
