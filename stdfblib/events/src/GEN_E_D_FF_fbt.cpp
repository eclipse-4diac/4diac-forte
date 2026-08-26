/*******************************************************************************
 * Copyright (c) 2026 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - implement Generic GEN_E_D_FF_fbt
 *******************************************************************************/

#include "forte/iec61499/events/GEN_E_D_FF_fbt.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cEventInputNames = std::array{"CLK"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_E_D_FF, "iec61499::events::GEN_E_D_FF"_STRID)

  GEN_E_D_FF::GEN_E_D_FF(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_EO(*this, 0) {
  }

  void GEN_E_D_FF::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (scmEventCLKID == paEIID) {
      const size_t numD = getFBInterfaceSpec().getNumDIs();

      bool changed = false;
      for (size_t i = 0; i < numD; ++i) {
        if (static_cast<bool>(mGenDs[i]) != static_cast<bool>(mGenQs[i])) {
          changed = true;
          break;
        }
      }

      if (changed) {
        for (size_t i = 0; i < numD; ++i) {
          mGenQs[i] = mGenDs[i];
        }
        sendOutputEvent(scmEventEOID, paECET);
      }
    }
  }

  void GEN_E_D_FF::readInputData(TEventID) {
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumDIs(); ++i) {
      readData(i, mGenDs[i], mGenDIConns[i]);
    }
  }

  void GEN_E_D_FF::writeOutputData(TEventID) {
    const TPortId numD = static_cast<TPortId>(getFBInterfaceSpec().getNumDIs());
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumDOs(); ++i) {
      writeData(numD + i, mGenQs[i], mGenDOConns[i]);
    }
  }

  bool GEN_E_D_FF::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    // Find the last underscore in the name, e.g., "E_D_FF_3".
    const char *acPos = strrchr(paConfigString, '_');

    if (nullptr != acPos) {
      ++acPos; // Move pointer to the character after the underscore.

      char *acEnd = nullptr;
      const size_t numD = static_cast<size_t>(util::strtoul(acPos, &acEnd, 10));

      if (('\0' == *acEnd) && numD >= 1 && numD < cgInvalidPortId) {
        generateGenericInterfacePointNameArray("D", mDINames, numD);
        generateGenericInterfacePointNameArray("Q", mDONames, numD);

        paInterfaceSpec.mEINames = cEventInputNames;
        paInterfaceSpec.mEONames = cEventOutputNames;
        paInterfaceSpec.mDINames = mDINames;
        paInterfaceSpec.mDONames = mDONames;
        return true;
      }

      DEVLOG_ERROR("Cannot configure FB-Instance E_D_FF_%zu. Number of data channels must be within 1 and %u.\n", numD,
                   cgInvalidPortId);
    }
    return false;
  }

  CEventConnection *GEN_E_D_FF::getEOConUnchecked(const TPortId paIndex) {
    return (paIndex == 0) ? &conn_EO : nullptr;
  }

  void GEN_E_D_FF::createGenInputData() {
    mGenDs = std::make_unique<CIEC_BOOL[]>(getFBInterfaceSpec().getNumDIs());
  }

  void GEN_E_D_FF::createGenOutputData() {
    mGenQs = std::make_unique<CIEC_BOOL[]>(getFBInterfaceSpec().getNumDOs());
  }

  CIEC_ANY *GEN_E_D_FF::getDI(const size_t paIndex) {
    return &mGenDs[paIndex];
  }

  CIEC_ANY *GEN_E_D_FF::getDO(const size_t paIndex) {
    return &mGenQs[paIndex];
  }
} // namespace forte::iec61499::events
