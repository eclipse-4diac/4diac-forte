/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Martin Erich Jobst, Johannes Kepler University Linz,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Gunnar Grabmair, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *   Martin Jobst - account for data type size in FB initialization
 *   Alois Zoitl  - exracted internal variable handling to new CBaseFB
 *                - migrated data type toString to std::string
 *******************************************************************************/

#include "forte/basefb.h"

#include "forte/resource.h"
#include "resource_internal.h"
#include "forte/util/string_utils.h"

namespace forte {
  CBaseFB::CBaseFB(CFBContainer &paContainer,
                   const SFBInterfaceSpec &paInterfaceSpec,
                   const StringId paInstanceNameId,
                   std::span<const StringId> paVarInternalNames) :
      CFunctionBlock(paContainer, paInterfaceSpec, paInstanceNameId),
      cmVarInternalNames(paVarInternalNames) {
  }

  void CBaseFB::setInitialValues() {
    CFunctionBlock::setInitialValues();
  }

  CIEC_ANY *CBaseFB::getVar(const std::span<const StringId> paNameList) {
    if (paNameList.empty()) {
      return nullptr;
    }
    const StringId name = paNameList.front();
    if (const auto var = getInternalVar(name)) {
      return var->getVar(paNameList.subspan(1));
    }
    return CFunctionBlock::getVar(paNameList);
  }

  CIEC_ANY *CBaseFB::getInternalVar(StringId paInternalName) {
    TPortId unVarId = getPortId(paInternalName, cmVarInternalNames);

    if (unVarId != cgInvalidPortId) {
      return getVarInternal(unVarId);
    }
    return nullptr;
  }

  void CBaseFB::toString(std::string &paTargetBuf) const {
    CFunctionBlock::toString(paTargetBuf);

    if (cmVarInternalNames.empty()) {
      return; // nothing to do
    }

    paTargetBuf.resize(paTargetBuf.size() - 1); // move the pointer to the position of the closing )

    if (paTargetBuf.back() != '(') { // not only ()
      paTargetBuf += csmToStringSeparator;
    }

    for (size_t i = 0; i < cmVarInternalNames.size(); ++i) {
      util::writeToStringNameValuePair(paTargetBuf, cmVarInternalNames[i], getVarInternal(i));
      if (i != cmVarInternalNames.size() - 1) {
        paTargetBuf += csmToStringSeparator;
      }
    }

    paTargetBuf += ')';
  };

#ifdef FORTE_TRACE_CTF
  void CBaseFB::traceInstanceData() {
    std::vector<std::string> inputs(getFBInterfaceSpec().getNumDIs());
    std::vector<std::string> outputs(getFBInterfaceSpec().getNumDOs());
    std::vector<std::string> internals(cmVarInternalNames.size());
    std::vector<std::string> internalFbs(getChildren().size());
    std::vector<const char *> inputs_c_str(inputs.size());
    std::vector<const char *> outputs_c_str(outputs.size());
    std::vector<const char *> internals_c_str(internals.size());
    std::vector<const char *> internalFbs_c_str(internalFbs.size());

    for (TPortId i = 0; i < inputs.size(); ++i) {
      CIEC_ANY *value = getDI(i);
      std::string &valueString = inputs[i];
      value->toString(valueString);
      inputs_c_str[i] = valueString.c_str();
    }

    for (TPortId i = 0; i < outputs.size(); ++i) {
      CIEC_ANY *value = getDO(i);
      std::string &valueString = outputs[i];
      value->toString(valueString);
      outputs_c_str[i] = valueString.c_str();
    }

    for (TPortId i = 0; i < internals.size(); ++i) {
      CIEC_ANY *value = getVarInternal(i);
      std::string &valueString = internals[i];
      value->toString(valueString);
      internals_c_str[i] = valueString.c_str();
    }

    TPortId i = 0;
    for (auto child : getChildren()) {
      CFunctionBlock &value = static_cast<CFunctionBlock &>(*child);
      std::string &valueString = internalFbs[i];
      value.toString(valueString);
      internalFbs_c_str[i] = valueString.c_str();
      ++i;
    }

    getResource()->getInternal().getTracer().traceInstanceData(
        getFBTypeName(), getFullQualifiedApplicationInstanceName('.').c_str(), static_cast<uint32_t>(inputs.size()),
        inputs_c_str.data(), static_cast<uint32_t>(outputs.size()), outputs_c_str.data(),
        static_cast<uint32_t>(internals.size()), internals_c_str.data(), static_cast<uint32_t>(internalFbs.size()),
        internalFbs_c_str.data());
  }
#endif
} // namespace forte
