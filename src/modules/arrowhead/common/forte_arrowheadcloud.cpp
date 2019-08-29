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

#include "forte_arrowheadcloud.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_arrowheadcloud_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(ArrowheadCloud, g_nStringIdArrowheadCloud);

CIEC_ArrowheadCloud::CIEC_ArrowheadCloud() :
    CIEC_STRUCT(g_nStringIdArrowheadCloud, 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ArrowheadCloud::scmElementTypes[] = { g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdDINT,
  g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdBOOL };
const CStringDictionary::TStringId CIEC_ArrowheadCloud::scmElementNames[] =
  { g_nStringIdoperator, g_nStringIdcloudName, g_nStringIdaddress, g_nStringIdport,
  g_nStringIdgatekeeperServiceURI, g_nStringIdauthenticationInfo, g_nStringIdsecure };
