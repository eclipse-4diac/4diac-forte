/*******************************************************************************
 * Copyright (c) 2024 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - implement Generic GEN_E_SPLIT_fbt
 *******************************************************************************/

#include "forte/iec61499/events/GEN_E_SPLIT_fbt.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  DEFINE_GENERIC_FIRMWARE_FB(GEN_E_SPLIT, "iec61499::events::GEN_E_SPLIT"_STRID)

  namespace {
    const auto cEventInputNames = std::array{"EI"_STRID};
  }

  GEN_E_SPLIT::GEN_E_SPLIT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId) {};

  void GEN_E_SPLIT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventEIID:
        for (TEventID i = 0; i < getFBInterfaceSpec().getNumEOs(); ++i) {
          sendOutputEvent(i, paECET);
        }
        break;
    }
  }

  void GEN_E_SPLIT::readInputData(TEventID) {
    // nothing to do
  }

  void GEN_E_SPLIT::writeOutputData(TEventID) {
    // nothing to do
  }

  bool GEN_E_SPLIT::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *acPos = strrchr(paConfigString, '_');

    if (nullptr != acPos) {
      ++acPos; // move after underscore
      // we have an underscore
      size_t numEOs = static_cast<TEventID>(util::strtoul(acPos, nullptr, 10));

      if (numEOs < scmMaxInterfaceEvents && numEOs >= 2) {
        generateGenericInterfacePointNameArray("EO", mEventOutputNames, numEOs);

        paInterfaceSpec.mEINames = cEventInputNames;
        paInterfaceSpec.mEONames = mEventOutputNames;
        return true;
      } else {
        if (numEOs >= scmMaxInterfaceEvents) {
          DEVLOG_ERROR("Cannot configure FB-Instance E_SPLIT_%d. Number of event outputs exceeds maximum of %d.\n",
                       numEOs, CFunctionBlock::scmMaxInterfaceEvents);
        } else {
          DEVLOG_ERROR("Cannot configure FB-Instance E_SPLIT_%d. Number of event outputs smaller than minimum of 2.\n",
                       numEOs);
        }
      }
    }
    return false;
  }

  CIEC_ANY *GEN_E_SPLIT::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *GEN_E_SPLIT::getDO(size_t) {
    return nullptr;
  }

  CDataConnection **GEN_E_SPLIT::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *GEN_E_SPLIT::getDOConUnchecked(TPortId) {
    return nullptr;
  }
} // namespace forte::iec61499::events
