/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: GEN_F_MUX
 *** Description: multiplexer
 *** Version:
 ***     1.0: 2013-08-30/Monika Wenger - TU Wien ACIN -
 ***     1.1: 2024-07-15/Franz Höpfinger - HR Agrartechnik GmbH - F_MUX_2 copy and added 1 IN
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 *************************************************************************/

#include "forte/iec61131/selection/GEN_F_MUX_fbt.h"

#include "forte/util/string_utils.h"
#include "forte/iec61131_functions.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61131::selection {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_F_MUX, "iec61131::selection::GEN_F_MUX"_STRID)

  GEN_F_MUX::GEN_F_MUX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      var_K(CIEC_ANY_INT_VARIANT()),
      var_OUT(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_K(nullptr),
      conn_OUT(*this, 0, var_OUT) {};


  void GEN_F_MUX::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_OUT = std::visit(
            [this](auto &&paK) -> CIEC_ANY_VARIANT {
              using T = std::decay_t<decltype(paK)>;
              typename T::TValueType valueK = static_cast<typename T::TValueType>(paK);

              if (valueK >= 0 && valueK < getFBInterfaceSpec().getNumDIs() - 1) {
                return mGenDIs[valueK];
              }
              DEVLOG_ERROR("Value of input K is out of range. Expected between 0 and %zu\n",
                           getFBInterfaceSpec().getNumDIs() - 2);
              return CIEC_ANY_VARIANT();
            },
            static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_K));
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void GEN_F_MUX::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_K, conn_K);
        for (TPortId i = 0; i < getGenDINums(); ++i) {
          readData(i + getGenDIOffset(), mGenDIs[i], mGenDIConns[i]);
        }
        break;
      }
      default: break;
    }
  }

  void GEN_F_MUX::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(getFBInterfaceSpec().getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  bool GEN_F_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *pcPos = strrchr(paConfigString, '_'); // find last _
    if (pcPos == nullptr) {
      return false;
    }

    pcPos++; // move after underscore
    // we have an underscore and it is the first underscore after F_MUX
    unsigned int numDIs = static_cast<unsigned int>(util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("DIs: %d;\n", numDIs);

    if (numDIs < 2) {
      DEVLOG_ERROR("GEN_F_MUX must have at least 2 IN ports.\n");
      return false;
    }

    // now the number of needed eventInputs and dataOutputs are available in the integer array
    // create the dataInputs
    mDINames.emplace_back("K"_STRID);
    generateGenericInterfacePointNameArray("IN", mDINames, numDIs);

    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = mDINames;
    paInterfaceSpec.mDONames = cDataOutputNames;

    return true;
  }

  CIEC_ANY *GEN_F_MUX::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_K;
    }
    return &mGenDIs[paIndex - getGenDIOffset()];
  }

  CIEC_ANY *GEN_F_MUX::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *GEN_F_MUX::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  void GEN_F_MUX::createGenInputData() {
    if (getGenDINums()) {
      mGenDIs = std::make_unique<CIEC_ANY_VARIANT[]>(getGenDINums());
    }
  }

  CDataConnection **GEN_F_MUX::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_K;
      default: return CGenFunctionBlock::getDIConUnchecked(paIndex);
    }
    return nullptr;
  }

  CDataConnection *GEN_F_MUX::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

} // namespace forte::iec61131::selection
