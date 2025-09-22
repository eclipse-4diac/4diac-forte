/*******************************************************************************
 * Copyright (c) 2020, 2025 Johannes Kepler University, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Martin Jobst - add generic readInputData and writeOutputData
 *   Markus Meingast - add support for configured struct demux instances
 *******************************************************************************/
#include "forte/eclipse4diac/convert/GEN_STRUCT_DEMUX_fbt.h"

using namespace forte::literals;

#include "forte/eclipse4diac/convert/GEN_STRUCT_MUX_fbt.h"
#include <stdio.h>
#include "forte/eclipse4diac/convert/GEN_STRUCT_DEMUX_fbt.h"

namespace forte::eclipse4diac::convert {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_STRUCT_DEMUX, "eclipse4diac::convert::GEN_STRUCT_DEMUX"_STRID)

  void GEN_STRUCT_DEMUX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (scmEventREQID == paEIID) {
      // we using struct members as outputs so no copying needed here
      sendOutputEvent(scmEventCNFID, paECET);
    }
  }

  GEN_STRUCT_DEMUX::GEN_STRUCT_DEMUX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_IN(nullptr) {
  }

  void GEN_STRUCT_DEMUX::readInputData(TEventID) {
    readData(0, *var_IN, conn_IN);
  }

  void GEN_STRUCT_DEMUX::writeOutputData(TEventID) {
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumDOs(); ++i) {
      writeData(1 + i, *mConfiguredDOPorts[i], mGenDOConns[i]);
    }
  }

  bool GEN_STRUCT_DEMUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    std::string_view configString{paConfigString};
    size_t endIndex = configString.find(STRUCT_NAME_SEPARATOR);
    bool isConfigured = false;
    if (endIndex != std::string::npos) {
      configString = configString.substr(0, endIndex);
      isConfigured = true;
    }
    const auto structTypeNameId = GEN_STRUCT_MUX::getStructNameId(configString);

    if (!structTypeNameId) {
      DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: Structure name for %s does not exist\n", paConfigString);
      return false;
    }

    std::unique_ptr<CIEC_ANY> data(createDataTypeInstance(structTypeNameId, nullptr));

    if (nullptr == data) {
      DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: Couldn't create structure of type: %s\n", structTypeNameId.data());
      return false;
    }

    if (data->getDataTypeID() != CIEC_ANY::e_STRUCT) {
      DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: data type is not a structure: %s\n", structTypeNameId.data());
      return false;
    }

    // we could find the struct
    var_IN = std::unique_ptr<CIEC_STRUCT>(static_cast<CIEC_STRUCT *>(data.release()));

    size_t structSize;
    if (isConfigured) {
      std::string_view configuredMemberNamesStr{paConfigString};
      configuredMemberNamesStr = configuredMemberNamesStr.substr(endIndex + STRUCT_NAME_SEPARATOR.size());
      std::vector<std::string_view> configuredMemberNames = getConfiguredMemberNames(configuredMemberNamesStr);
      structSize = configuredMemberNames.size();
      fillConfiguredInterfaceSpec(configuredMemberNames);
    } else {
      structSize = var_IN->getStructSize();
      fillInterfaceSpec();
    }
    if (structSize == 0 || structSize >= cgInvalidPortId) { // the structure size must be non zero and less than
                                                            // cgInvalidPortId (maximum number of data input)
      DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: The structure %s has a size that is not within range > 0 and < %u\n",
                   structTypeNameId.data(), cgInvalidPortId);
      return false;
    }

    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = cDataInputNames;
    paInterfaceSpec.mDONames = mDoNames;

    return true;
  }

  void GEN_STRUCT_DEMUX::fillConfiguredInterfaceSpec(std::vector<std::string_view> &paConfiguredMemberNames) {
    mConfiguredDOPorts.reserve(paConfiguredMemberNames.size());
    mDoNames.reserve(paConfiguredMemberNames.size());
    for (size_t i = 0; i < paConfiguredMemberNames.size(); ++i) {
      StringId memberNameId = StringId::insert(paConfiguredMemberNames[i]);
      mDoNames.emplace_back(memberNameId);
      CIEC_ANY *member = getNestedMember(memberNameId, var_IN.get());
      mConfiguredDOPorts.emplace_back(member);
    }
  }

  void GEN_STRUCT_DEMUX::fillInterfaceSpec() {
    size_t structSize = var_IN->getStructSize();
    mConfiguredDOPorts.reserve(structSize);
    mDoNames.reserve(structSize);
    for (size_t i = 0; i < structSize; ++i) {
      CIEC_ANY *member = var_IN->getMember(i);
      mDoNames.emplace_back(var_IN->elementNames()[i]);
      mConfiguredDOPorts.emplace_back(member);
    }
  }

  std::vector<std::string_view> GEN_STRUCT_DEMUX::getConfiguredMemberNames(std::string_view paMemberNameString) {
    std::vector<std::string_view> configuredMemberNames;
    size_t nextIndex = paMemberNameString.find(',');
    while (nextIndex != std::string::npos) {
      configuredMemberNames.emplace_back(paMemberNameString.substr(0, nextIndex));
      paMemberNameString = paMemberNameString.substr(nextIndex + 1);
      nextIndex = paMemberNameString.find(',');
    }
    if (!paMemberNameString.empty()) {
      configuredMemberNames.emplace_back(paMemberNameString.substr(0));
    }
    return configuredMemberNames;
  }

  CIEC_ANY *GEN_STRUCT_DEMUX::getNestedMember(const StringId paNameId, CIEC_STRUCT *paStructType) {
    std::string_view memberName = paNameId;
    size_t index = memberName.find(NESTED_VAR_SEPARATOR);
    CIEC_STRUCT *structType = paStructType;
    while (index != std::string::npos) {
      std::string_view nestedStructName = memberName.substr(0, index);
      structType = static_cast<CIEC_STRUCT *>(structType->getMemberNamed(StringId::lookup(nestedStructName)));
      memberName = memberName.substr(index + 1);
      index = memberName.find(NESTED_VAR_SEPARATOR);
    }
    return structType->getMemberNamed(StringId::lookup(memberName));
  }

  CEventConnection *GEN_STRUCT_DEMUX::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_CNF : nullptr;
  }

  CIEC_ANY *GEN_STRUCT_DEMUX::getDI(size_t paIndex) {
    return (paIndex == 0) ? var_IN.get() : nullptr;
  }

  CIEC_ANY *GEN_STRUCT_DEMUX::getDO(size_t paIndex) {
    return mConfiguredDOPorts[paIndex];
  }

  CDataConnection **GEN_STRUCT_DEMUX::getDIConUnchecked(const TPortId paIndex) {
    return (paIndex == 0) ? &conn_IN : nullptr;
  }
} // namespace forte::eclipse4diac::convert
