/*******************************************************************************
 * Copyright (c) 2014, 2023 Profactor GmbH, fortiss GmbH,
 *                          Johannes Kepler University, Martin Erich Jobst
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
 *     - add generic readInputData and writeOutputData
 *******************************************************************************/

#include "forte/iec61131/arithmetic/GEN_ADD_fbt.h"

#include "forte/iec61131_functions/func_ADD.h"

using namespace forte::literals;

namespace forte::iec61131::arithmetic {
  DEFINE_GENERIC_FIRMWARE_FB(GEN_ADD, "iec61131::arithmetic::GEN_ADD"_STRID)

  GEN_ADD::GEN_ADD(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenArithBase<CIEC_ANY_MAGNITUDE_VARIANT>(paInstanceNameId, paContainer) {
  }

  void GEN_ADD::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == scmEventREQID) {
      var_OUT = var_IN(0);
      for (size_t i = 1; i < getFBInterfaceSpec().getNumDIs(); ++i) {
        var_OUT = std::visit(
            [](auto &&paOUT, auto &&paIN) -> CIEC_ANY_MAGNITUDE_VARIANT {
              using T = std::decay_t<decltype(paOUT)>;
              using U = std::decay_t<decltype(paIN)>;
              using deductedType = typename mpl::get_add_operator_result_type<T, U>::type;
              if constexpr (!std::is_same<deductedType, mpl::NullType>::value) {
                return func_ADD(paOUT, paIN);
              }
              DEVLOG_ERROR("Adding incompatible types %s and %s\n", paOUT.getTypeNameID().data(),
                           paIN.getTypeNameID().data());
              return paOUT;
            },
            static_cast<CIEC_ANY_MAGNITUDE_VARIANT::variant &>(var_OUT),
            static_cast<CIEC_ANY_MAGNITUDE_VARIANT::variant &>(var_IN(i)));
      }
      sendOutputEvent(scmEventCNFID, paECET);
    }
  }
} // namespace forte::iec61131::arithmetic
