/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Ingo Hegny,
 *   Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_PUBL.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_PUBL, g_nStringIdGEN_PUBL)

GEN_PUBL::GEN_PUBL(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes):
          GEN_PUBLISH( pa_nInstanceNameId, pa_poSrcRes) {
}

bool GEN_PUBL::configureFB(const char *pa_acConfigString){
  bool bRetVal = GEN_PUBLISH::configureFB(pa_acConfigString);
  //publs normally don't show the QI in the tool
  QI() = true;
  return bRetVal;
}


