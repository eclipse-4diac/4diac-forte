/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, Profactor GmbH, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Matthias Plasch
 *     - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *....Franz Höpfinger - Update it to represent latest Generation Format from 4diac IDE. no Functional Changes.
 *******************************************************************************/
#include "forte/iec61499/events/GEN_E_MUX_fbt.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cDataOutputNames = std::array{"K"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_E_MUX, "iec61499::events::GEN_E_MUX"_STRID)

  GEN_E_MUX::GEN_E_MUX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      var_K(0_UINT),
      conn_EO(*this, 0),
      conn_K(*this, 0, var_K) {};

  void GEN_E_MUX::setInitialValues() {
    var_K = 0_UINT;
  }

  void GEN_E_MUX::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID < getFBInterfaceSpec().getNumEIs()) {
      var_K = CIEC_UINT(static_cast<TForteUInt16>(paEIID));
      sendOutputEvent(scmEventEOID, paECET);
    }
  }

  void GEN_E_MUX::readInputData(TEventID) {
    // nothing to do
  }

  void GEN_E_MUX::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(0, var_K, conn_K);
        break;
      }
      default: break;
    }
  }

  bool GEN_E_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *acPos = strrchr(paConfigString, '_');

    if (nullptr != acPos) {
      ++acPos;
      if ('M' != *acPos) {
        // we have an underscore and it is not the first underscore after E
        size_t numEIs = static_cast<TEventID>(util::strtoul(acPos, nullptr, 10));

        if (numEIs < scmMaxInterfaceEvents && numEIs >= 2) {
          generateGenericInterfacePointNameArray("EI", mEventInputNames, numEIs);

          paInterfaceSpec.mEINames = mEventInputNames;
          paInterfaceSpec.mEONames = cEventOutputNames;
          paInterfaceSpec.mDONames = cDataOutputNames;
          return true;
        } else {
          if (numEIs >= scmMaxInterfaceEvents) {
            DEVLOG_ERROR("Cannot configure FB-Instance E_MUX_%d. Number of event inputs exceeds maximum of %d.\n",
                         numEIs, CFunctionBlock::scmMaxInterfaceEvents);
          } else {
            DEVLOG_ERROR("Cannot configure FB-Instance E_MUX_%d. Number of event inputs smaller than minimum of 2.\n",
                         numEIs);
          }
        }
      }
    }
    return false;
  }

  CIEC_ANY *GEN_E_MUX::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *GEN_E_MUX::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_K;
    }
    return nullptr;
  }

  CEventConnection *GEN_E_MUX::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **GEN_E_MUX::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *GEN_E_MUX::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_K;
    }
    return nullptr;
  }
} // namespace forte::iec61499::events
