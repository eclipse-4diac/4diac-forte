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

#include "forte_preferredprovider.h"

USE_STRING_ID(ArrowheadCloud);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(PreferredProvider);
USE_STRING_ID(providerCloud);
USE_STRING_ID(providerSystem);

DEFINE_FIRMWARE_DATATYPE(PreferredProvider, STRID(PreferredProvider));

CIEC_PreferredProvider::CIEC_PreferredProvider() :
    CIEC_STRUCT(STRID(PreferredProvider), 2, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_PreferredProvider::scmElementTypes[] = {STRID(ArrowheadSystem),
                                                                                STRID(ArrowheadCloud)};
const CStringDictionary::TStringId CIEC_PreferredProvider::scmElementNames[] = {STRID(providerSystem),
                                                                                STRID(providerCloud)};
