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
 *** Name: LOG_16
 *** Description: Ring Logger for ANY Datattype.
 *** Version:
 ***     1.0: 2025-10-18/Franz Höpfinger -  - - initial implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/logging/GEN_LOG_fbt.h"

#include "forte/util/string_utils.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::logging {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_LOG, "eclipse4diac::utils::logging::GEN_LOG"_STRID)

  GEN_LOG::GEN_LOG(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      var_IN(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_IN(nullptr) {
  }

  bool GEN_LOG::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *pcPos = strrchr(paConfigString, '_'); // Find the last '_'
    if (pcPos == nullptr) {
      return false;
    }

    pcPos++; // Move to the position after the '_'
    unsigned int numDOs = static_cast<unsigned int>(util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("Ports: %d;\n", numPorts);

    if (numDOs < 1) {
      DEVLOG_ERROR("GEN_LOG must have at least 1 OUT ports.\n");
      return false;
    }

    generateGenericInterfacePointNameArray("OUT", mDONames, numDOs);

    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = cDataInputNames;
    paInterfaceSpec.mDONames = mDONames;

    return true;
  }

  void GEN_LOG::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == scmEventREQID) {

      mGenDOs[mCurrentIndex] = var_IN;

      mCurrentIndex++;

      if (mCurrentIndex >= getFBInterfaceSpec().getNumDOs()) {
        mCurrentIndex = 0;
      }

      sendOutputEvent(scmEventCNFID, paECET);
    }
  }

  void GEN_LOG::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default:
        break;
    }
  }

  void GEN_LOG::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        for (TPortId i = 0; i < getFBInterfaceSpec().getNumDOs(); ++i) {
          writeData(getFBInterfaceSpec().getNumDIs() + i, mGenDOs[i], mGenDOConns[i]);
        }
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *GEN_LOG::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *GEN_LOG::getDO(const size_t paIndex) {
    if (paIndex < getFBInterfaceSpec().getNumDOs()) {
      return &mGenDOs[paIndex];
    }
    return nullptr;
  }


  void GEN_LOG::createGenOutputData() {
    if (getGenDONums() > 0) {
      mGenDOs = std::make_unique<CIEC_ANY_VARIANT[]>(getGenDONums());
    }
  }


  CEventConnection *GEN_LOG::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **GEN_LOG::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *GEN_LOG::getDOConUnchecked(const TPortId paIndex) {
    if (paIndex < getFBInterfaceSpec().getNumDOs()) {
      return &mGenDOConns[paIndex];
    }
    return nullptr;
  }
} // namespace forte::eclipse4diac::utils::logging
