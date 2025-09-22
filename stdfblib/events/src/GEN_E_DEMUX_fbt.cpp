/*******************************************************************************
 * Copyright (c) 2011, 2023 ACIN, Profactor GmbH, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
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
 *******************************************************************************/
#include "forte/iec61499/events/GEN_E_DEMUX_fbt.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"K"_STRID};
    const auto cEventInputNames = std::array{"EI"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_E_DEMUX, "iec61499::events::GEN_E_DEMUX"_STRID)

  GEN_E_DEMUX::GEN_E_DEMUX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_K(nullptr) {
  }

  void GEN_E_DEMUX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (scmEventEIID == paEIID && static_cast<CIEC_UINT::TValueType>(var_K) < getFBInterfaceSpec().getNumEOs()) {
      sendOutputEvent(static_cast<CIEC_UINT::TValueType>(var_K),
                      paECET); // the value of K corresponds to the output event ID;
    }
  }

  void GEN_E_DEMUX::readInputData(TEventID) {
    readData(0, var_K, conn_K);
  }

  void GEN_E_DEMUX::writeOutputData(TEventID) {
  }

  CIEC_ANY *GEN_E_DEMUX::getDI(const size_t paIndex) {
    return (paIndex == 0) ? &var_K : nullptr;
  }

  CDataConnection **GEN_E_DEMUX::getDIConUnchecked(const TPortId paIndex) {
    return (paIndex == 0) ? &conn_K : nullptr;
  }

  bool GEN_E_DEMUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *acPos = strrchr(paConfigString, '_');

    if (nullptr != acPos) {
      ++acPos;
      if ('D' != *acPos) {
        // we have an underscore and it is not the first underscore after E
        size_t numEOs = static_cast<TEventID>(util::strtoul(acPos, nullptr, 10));

        if (numEOs < scmMaxInterfaceEvents) {
          generateGenericInterfacePointNameArray("EO", mEventOutputNames, numEOs);

          paInterfaceSpec.mEINames = cEventInputNames;
          paInterfaceSpec.mEONames = mEventOutputNames;
          paInterfaceSpec.mDINames = cDataInputNames;
          return true;
        }
      }
    }
    return false;
  }
} // namespace forte::iec61499::events
