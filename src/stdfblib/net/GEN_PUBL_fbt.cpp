/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Ingo Hegny,
 *   Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_PUBL_fbt.h"
#include "comlayersmanager.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_PUBL, g_nStringIdGEN_PUBL)


const char * const GEN_PUBL::scmLocalIDPrefix = "loc[";
const char * const GEN_PUBL::scmLocalIDSuffix = "]";

GEN_PUBL::GEN_PUBL(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer):
          GEN_PUBLISH( paInstanceNameId, paContainer) {
}

bool GEN_PUBL::configureFB(const char *paConfigString){
  bool bRetVal = GEN_PUBLISH::configureFB(paConfigString);
  //publs normally don't show the QI in the tool
  QI() = CIEC_BOOL(true);
  return bRetVal;
}

char * GEN_PUBL::getDefaultIDString(const char *paID){
  return buildIDString(scmLocalIDPrefix, paID, scmLocalIDSuffix);
}


