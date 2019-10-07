/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "FB_CTUD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_CTUD_gen.cpp"
#endif
DEFINE_FIRMWARE_FB(FB_CTUD, g_nStringIdFB_CTUD)

const CStringDictionary::TStringId FB_CTUD::scm_anDataInputNames[] = { g_nStringIdCU, g_nStringIdCD, g_nStringIdR, g_nStringIdLD, g_nStringIdPV };

const CStringDictionary::TStringId FB_CTUD::scm_anDataOutputNames[] = { g_nStringIdQU, g_nStringIdQD, g_nStringIdCV };
const CStringDictionary::TStringId FB_CTUD::scm_aunDIDataTypeIds[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdINT };
const CStringDictionary::TStringId FB_CTUD::scm_aunDODataTypeIds[] = { g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdINT };

const TForteInt16 FB_CTUD::scm_anEIWithIndexes[] = { 0 };
const TDataIOID FB_CTUD::scm_anEIWith[] = { 1, 4, 3, 0, 2, 255 };
const CStringDictionary::TStringId FB_CTUD::scm_anEventInputNames[] = { g_nStringIdREQ };

const TDataIOID FB_CTUD::scm_anEOWith[] = { 0, 2, 1, 255 };
const TForteInt16 FB_CTUD::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId FB_CTUD::scm_anEventOutputNames[] = { g_nStringIdCNF };

const SFBInterfaceSpec FB_CTUD::scm_stFBInterfaceSpec = { 1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 5, scm_anDataInputNames, scm_aunDIDataTypeIds, 3, scm_anDataOutputNames, scm_aunDODataTypeIds,
    0,
    0 };

void FB_CTUD::executeEvent(int pa_nEIID) {
  if (pa_nEIID == scm_nEventREQID) {
    if (true == R()) {
      CV() = 0;
    }
    else {
      if (true == LD()) {
        CV() = PV();
      }
      else {
        if (!(CU() && CD())) {
          if ((CU() && (CV() < CIEC_INT::scm_nMaxVal))) {
            CV() = static_cast<TForteInt16>(CV() + 1);
          }
          else {
            if ((CD() && (CV() > CIEC_INT::scm_nMinVal))) {
              CV() = static_cast<TForteInt16>(CV() - 1);
            }
          }
        }
      }
    }
    QU() = (CV() >= PV());
    QD() = (CV() <= 0);
    sendOutputEvent(scm_nEventCNFID);
  }
}
