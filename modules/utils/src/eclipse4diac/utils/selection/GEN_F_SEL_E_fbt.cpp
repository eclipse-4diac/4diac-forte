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
 *** Name: GEN_F_SEL_E
 *** Description: Generic event-based selector
 *** Version:
 *** 1.0: 2025-10-16/Coding-Assistent - Google - initial creation
 *************************************************************************/

#include "forte/eclipse4diac/utils/selection/GEN_F_SEL_E_fbt.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::eclipse4diac::utils::selection {
  namespace {
    // Define the names for the fixed outputs.
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
  } // namespace

  // Registers the function block in FORTE.
  DEFINE_GENERIC_FIRMWARE_FB(GEN_F_SEL_E, "eclipse4diac::utils::selection::GEN_F_SEL_E"_STRID)

  GEN_F_SEL_E::GEN_F_SEL_E(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      var_OUT(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_OUT(*this, 0, var_OUT) {
  }

  /**
   * @brief This method is called when an event arrives at the FB.
   *
   * The event ID (paEIID) directly corresponds to the index of the input to be selected.
   */
  void GEN_F_SEL_E::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (static_cast<size_t>(paEIID) < getGenDINums()) {
      // Copy the value of the corresponding generic input to the output.
      var_OUT = mGenDIs[paEIID];
      // Send the 'CNF' event to activate subsequent FBs.
      sendOutputEvent(scmEventCNFID, paECET);
    } else {
      DEVLOG_ERROR("GEN_F_SEL_E: Event ID %d is out of valid range.\n", paEIID);
    }
  }

  /**
   * @brief Reads the data from the corresponding data input.
   *
   * Is only called for the input associated with the triggered event.
   */
  void GEN_F_SEL_E::readInputData(const TEventID paEIID) {
    if (static_cast<size_t>(paEIID) < getGenDINums()) {
      readData(paEIID, mGenDIs[paEIID], mGenDIConns[paEIID]);
    }
  }

  /**
   * @brief Writes the data to the data output.
   */
  void GEN_F_SEL_E::writeOutputData(const TEventID paEIID) {
    if (paEIID == scmEventCNFID) {
      // '0' is the index of the single data output 'OUT'.
      writeData(getFBInterfaceSpec().getNumDIs() + 0, var_OUT, conn_OUT);
    }
  }

  /**
   * @brief Dynamically builds the FB's interface.
   *
   * Reads the number at the end of the FB name (e.g., 'GEN_F_SEL_E_4' -> 4)
   * and creates the corresponding number of inputs.
   */
  bool GEN_F_SEL_E::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *pcPos = strrchr(paConfigString, '_'); // Find the last '_'
    if (pcPos == nullptr) {
      return false;
    }

    pcPos++; // Move to the position after the '_'
    unsigned int numPorts = static_cast<unsigned int>(util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("Ports: %d;\n", numPorts);

    if (numPorts < 2) {
      DEVLOG_ERROR("GEN_F_SEL_E must have at least 2 IN/REQ ports.\n");
      return false;
    }

    // Generate the names for the event and data inputs.
    generateGenericInterfacePointNameArray("REQ", mEINames, numPorts);
    generateGenericInterfacePointNameArray("IN", mDINames, numPorts);

    // Assign the names to the interface specification.
    paInterfaceSpec.mEINames = mEINames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = mDINames;
    paInterfaceSpec.mDONames = cDataOutputNames;

    return true;
  }

  CIEC_ANY *GEN_F_SEL_E::getDI(const size_t paIndex) {
    // As there is no offset, the index is the direct index into the array.
    return &mGenDIs[paIndex];
  }

  CIEC_ANY *GEN_F_SEL_E::getDO(const size_t paIndex) {
    if (paIndex == 0) {
      return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *GEN_F_SEL_E::getEOConUnchecked(const TPortId paIndex) {
    if (paIndex == 0) {
      return &conn_CNF;
    }
    return nullptr;
  }

  void GEN_F_SEL_E::createGenInputData() {
    if (getGenDINums() > 0) {
      mGenDIs = std::make_unique<CIEC_ANY_VARIANT[]>(getGenDINums());
    }
  }

  CDataConnection *GEN_F_SEL_E::getDOConUnchecked(const TPortId paIndex) {
    if (paIndex == 0) {
      return &conn_OUT;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::utils::selection
