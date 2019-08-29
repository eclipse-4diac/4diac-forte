/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Rene Smodic, Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer,
 *   Ingo Hegny, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_SUBL.h"
#include "comlayersmanager.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_SUBL, g_nStringIdGEN_SUBL)


const char * const GEN_SUBL::scmLocalIDPrefix = "loc[";
const char * const GEN_SUBL::scmLocalIDSuffix = "]";

GEN_SUBL::GEN_SUBL(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes):
          GEN_SUBSCRIBE( pa_nInstanceNameId, pa_poSrcRes){
}

bool GEN_SUBL::configureFB(const char *pa_acConfigString){
  bool bRetVal = GEN_SUBSCRIBE::configureFB(pa_acConfigString);
  //subls normally don't show the QI in the tool
  QI() = true;
  return bRetVal;
}

char * GEN_SUBL::getDefaultIDString(const char *paID){
  return buildIDString(scmLocalIDPrefix, paID, scmLocalIDSuffix);
}

