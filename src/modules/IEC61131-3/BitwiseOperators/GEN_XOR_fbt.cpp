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

#include "GEN_XOR_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_XOR_fbt_gen.cpp"
#endif

DEFINE_GENERIC_FIRMWARE_FB(GEN_XOR, g_nStringIdGEN_XOR)

GEN_XOR::GEN_XOR(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CGenBitBase(paInstanceNameId, paContainer){
}

void GEN_XOR::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      if(getFBInterfaceSpec().mNumDIs) {
        var_OUT() = var_IN(0);
        for (size_t i = 1; i < getFBInterfaceSpec().mNumDIs; ++i) {
          var_OUT() = std::visit([](auto &&paOUT, auto &&paIN) -> CIEC_ANY_BIT_VARIANT {
              return func_XOR(paOUT, paIN);
          }, static_cast<CIEC_ANY_BIT_VARIANT::variant&>(var_OUT()),
             static_cast<CIEC_ANY_BIT_VARIANT::variant&>(var_IN(i)));
        }
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

