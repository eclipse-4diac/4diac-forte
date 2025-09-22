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

#include "forte/util/string_utils.h"
#include "forte/iec61131_functions.h"

using namespace forte::literals;

namespace {
  static const forte::StringId eventInputNames[] = {"REQ"_STRID};
  static const forte::StringId eventOutputNames[] = {"CNF"_STRID};
  static const forte::StringId dataOutputNames[] = {"OUT"_STRID};

} // namespace

namespace forte::iec61131::arithmetic {
  DEFINE_GENERIC_FIRMWARE_FB(GEN_ADD, "iec61131::arithmetic::GEN_ADD"_STRID)

  GEN_ADD::GEN_ADD(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_OUT(*this, 0, var_OUT) {
  }

  void GEN_ADD::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == scmEventREQID) {
      var_OUT = mGenDIs[0];
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
            static_cast<CIEC_ANY_MAGNITUDE_VARIANT::variant &>(mGenDIs[i]));
      }
      sendOutputEvent(scmEventCNFID, paECET);
    }
  }

  void GEN_ADD::readInputData(TEventID) {
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumDIs(); ++i) {
      readData(i, mGenDIs[i], mGenDIConns[i]);
    }
  }

  void GEN_ADD::writeOutputData(TEventID) {
    writeData(getFBInterfaceSpec().getNumDIs() + 0, var_OUT, conn_OUT);
  }

  bool GEN_ADD::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *pcPos = strrchr(paConfigString, '_');
    if (pcPos == nullptr) {
      return false;
    }

    pcPos++;
    // we have an underscore and it is the first underscore after ADD
    unsigned int numDIs = static_cast<unsigned int>(util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("DIs: %d;\n", numDIs);

    if (numDIs < 2) {
      return false;
    }

    // now the number of needed eventInputs and dataOutputs are available in the integer array
    // create the eventInputs
    generateGenericInterfacePointNameArray("IN", mDINames, numDIs);

    paInterfaceSpec.mEINames = eventInputNames;
    paInterfaceSpec.mEONames = eventOutputNames;
    paInterfaceSpec.mDINames = mDINames;
    paInterfaceSpec.mDONames = dataOutputNames;

    return true;
  }

  CEventConnection *GEN_ADD::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_CNF : nullptr;
  }

  void GEN_ADD::createGenInputData() {
    mGenDIs = std::make_unique<CIEC_ANY_MAGNITUDE_VARIANT[]>(getFBInterfaceSpec().getNumDIs());
  }

  CIEC_ANY *GEN_ADD::getDI(size_t paDINum) {
    return &mGenDIs[paDINum];
  }

  CIEC_ANY *GEN_ADD::getDO(size_t paDONum) {
    return (paDONum == 0) ? &var_OUT : nullptr;
  }

  CDataConnection *GEN_ADD::getDOConUnchecked(const TPortId paDONum) {
    return (paDONum == 0) ? &conn_OUT : nullptr;
  }
} // namespace forte::iec61131::arithmetic
