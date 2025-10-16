/*******************************************************************************
 * Copyright (c) 2025 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Coding-Assistent Gemini and Franz Höpfinger
 *    - implement Generic GEN_E_MERGE_fbt based on GEN_E_SPLIT
 *******************************************************************************/

#include "forte/iec61499/events/GEN_E_MERGE_fbt.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  // Registers the generic FB with the name "GEN_E_MERGE"
  DEFINE_GENERIC_FIRMWARE_FB(GEN_E_MERGE, "iec61499::events::GEN_E_MERGE"_STRID)

  namespace {
    // Defines the constant name for the single event output.
    const auto cEventOutputNames = std::array{"EO"_STRID};
  } // namespace

  GEN_E_MERGE::GEN_E_MERGE(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId) {};

  void GEN_E_MERGE::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    // Any event input triggers the single event output.
    // We don't need to check which input it was (paEIID is unused).
    sendOutputEvent(scmEventEOID, paECET);
  }

  void GEN_E_MERGE::readInputData(TEventID) {
    // Nothing to do, as there are no data inputs.
  }

  void GEN_E_MERGE::writeOutputData(TEventID) {
    // Nothing to do, as there are no data outputs.
  }

  bool GEN_E_MERGE::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    // Find the last underscore in the name, e.g., "E_MERGE_5".
    const char *acPos = strrchr(paConfigString, '_');

    if (nullptr != acPos) {
      ++acPos; // Move pointer to the character after the underscore.

      // Convert the numeric part to an integer. This is the number of event inputs.
      size_t numEIs = static_cast<TEventID>(util::strtoul(acPos, nullptr, 10));

      // Check if the number of inputs is within the valid range.
      if (numEIs < scmMaxInterfaceEvents && numEIs >= 2) {
        // Use the utility function to generate names like "EI1", "EI2", etc.
        generateGenericInterfacePointNameArray("EI", mEventInputNames, numEIs);

        // Assign the generated input names and the constant output name to the interface.
        paInterfaceSpec.mEINames = mEventInputNames;
        paInterfaceSpec.mEONames = cEventOutputNames;
        return true; // Success
      } else {
        // Log errors if the number is out of bounds.
        if (numEIs >= scmMaxInterfaceEvents) {
          DEVLOG_ERROR("Cannot configure FB-Instance E_MERGE_%d. Number of event inputs exceeds maximum of %d.\n",
                       numEIs, CFunctionBlock::scmMaxInterfaceEvents);
        } else {
          DEVLOG_ERROR("Cannot configure FB-Instance E_MERGE_%d. Number of event inputs smaller than minimum of 2.\n",
                       numEIs);
        }
      }
    }
    return false; // Failure
  }

  // The following functions return nullptr because this FB does not handle data.
  CIEC_ANY *GEN_E_MERGE::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *GEN_E_MERGE::getDO(size_t) {
    return nullptr;
  }

  CDataConnection **GEN_E_MERGE::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *GEN_E_MERGE::getDOConUnchecked(TPortId) {
    return nullptr;
  }
} // namespace forte::iec61499::events
