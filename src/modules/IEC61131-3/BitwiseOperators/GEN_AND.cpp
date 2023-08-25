/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "GEN_AND.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_AND_gen.cpp"
#endif

DEFINE_GENERIC_FIRMWARE_FB(GEN_AND, g_nStringIdGEN_AND)

GEN_AND::GEN_AND(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenBitBase(paInstanceNameId, paSrcRes){
}

GEN_AND::~GEN_AND() = default;

void GEN_AND::executeEvent(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID:
      if(getFBInterfaceSpec()->mNumDIs) {
        var_OUT() = var_IN(0);
        for (size_t i = 1; i < getFBInterfaceSpec()->mNumDIs; ++i) {
          var_OUT() = std::visit([](auto &&paOUT, auto &&paIN) -> CIEC_ANY_BIT_VARIANT {
            return func_AND(paOUT, paIN);
          }, var_OUT(), var_IN(i));
        }
      }
      sendOutputEvent(scmEventCNFID);
      break;
  }
}
