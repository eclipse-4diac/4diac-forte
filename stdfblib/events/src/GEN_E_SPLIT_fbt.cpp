/*******************************************************************************
 * Copyright (c) 2024, 2025 HR Agartechnik GmbH
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
 *   Coding-Assistent
 *    - added detailed English comments for consistency
 *******************************************************************************/

#include "forte/iec61499/events/GEN_E_SPLIT_fbt.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  // Registers the generic FB with the name "GEN_E_SPLIT" in the FORTE type library.
  DEFINE_GENERIC_FIRMWARE_FB(GEN_E_SPLIT, "iec61499::events::GEN_E_SPLIT"_STRID)

  namespace {
    // Defines the constant name for the single event input.
    const auto cEventInputNames = std::array{"EI"_STRID};
  } // namespace

  GEN_E_SPLIT::GEN_E_SPLIT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId) {};

  void GEN_E_SPLIT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    // Check if the received event is the expected input event 'EI'.
    switch (paEIID) {
      case scmEventEIID:
        // If it is, iterate through all configured event outputs and send an event.
        for (TEventID i = 0; i < getFBInterfaceSpec().getNumEOs(); ++i) {
          sendOutputEvent(i, paECET);
        }
        break;
    }
  }

  void GEN_E_SPLIT::readInputData(TEventID) {
    // Nothing to do, as there are no data inputs.
  }

  void GEN_E_SPLIT::writeOutputData(TEventID) {
    // Nothing to do, as there are no data outputs.
  }

  bool GEN_E_SPLIT::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    // Find the last underscore in the name, e.g., "E_SPLIT_5".
    const char *acPos = strrchr(paConfigString, '_');

    if (nullptr != acPos) {
      ++acPos; // Move pointer to the character after the underscore.

      // Convert the numeric part to an integer. This is the number of event outputs.
      size_t numEOs = static_cast<TEventID>(util::strtoul(acPos, nullptr, 10));

      // Check if the number of outputs is within the valid range.
      if (numEOs < scmMaxInterfaceEvents && numEOs >= 2) {
        // Use the utility function to generate names like "EO1", "EO2", etc.
        generateGenericInterfacePointNameArray("EO", mEventOutputNames, numEOs);

        // Assign the constant input name and the generated output names to the interface.
        paInterfaceSpec.mEINames = cEventInputNames;
        paInterfaceSpec.mEONames = mEventOutputNames;
        return true; // Success
      } else {
        // Log errors if the number is out of bounds.
        if (numEOs >= scmMaxInterfaceEvents) {
          DEVLOG_ERROR("Cannot configure FB-Instance E_SPLIT_%d. Number of event outputs exceeds maximum of %d.\n",
                       numEOs, CFunctionBlock::scmMaxInterfaceEvents);
        } else {
          DEVLOG_ERROR("Cannot configure FB-Instance E_SPLIT_%d. Number of event outputs smaller than minimum of 2.\n",
                       numEOs);
        }
      }
    }
    return false; // Failure
  }

  // The following functions return nullptr because this FB does not handle data.
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
