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
#include "GEN_STRUCT_DEMUX_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_STRUCT_DEMUX_fbt_gen.cpp"
#endif
#include "GEN_STRUCT_MUX_fbt.h"
#include <stdio.h>
#include "GEN_STRUCT_DEMUX_fbt.h"

#include "resource.h"
#include "criticalregion.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_STRUCT_DEMUX, g_nStringIdGEN_STRUCT_DEMUX);

const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scmEventInputNames[] = { g_nStringIdREQ };
const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scmEventInputTypeIds[] = {g_nStringIdEvent};
const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scmEventOutputNames[] = { g_nStringIdCNF };
const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scmEventOutputTypeIds[] = {g_nStringIdEvent};

const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scmDataInputNames[] = { g_nStringIdIN };

void GEN_STRUCT_DEMUX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if(scmEventREQID == paEIID) {
    copyStructValuesToOutputs();
    sendOutputEvent(scmEventCNFID, paECET);
  }
}

GEN_STRUCT_DEMUX::GEN_STRUCT_DEMUX(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId){
}

void GEN_STRUCT_DEMUX::readInputData(TEventID) {
  readData(0, *mDIs[0], mDIConns[0]);
}

void GEN_STRUCT_DEMUX::writeOutputData(TEventID) {
  for(TPortId i = 0; i < getFBInterfaceSpec().mNumDOs; ++i) {
    writeData(i, *mDOs[i], mDOConns[i]);
  }
}

bool GEN_STRUCT_DEMUX::initialize() {
  if(CGenFunctionBlock::initialize()) {
    setConfiguredDOPorts();
    copyStructValuesToOutputs();
    return true;
  }
  return false;
}

void GEN_STRUCT_DEMUX::setInitialValues() {
  CFunctionBlock::setInitialValues();
  copyStructValuesToOutputs();
}

void GEN_STRUCT_DEMUX::copyStructValuesToOutputs() {
  for (TPortId i = 0; i < mConfiguredDOPorts.size(); i++){
      getDO(i)->setValue(mConfiguredDOPorts[i]->unwrap());
    }
}

bool GEN_STRUCT_DEMUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  std::string_view configString {paConfigString};
  size_t endIndex = configString.find(STRUCT_NAME_SEPARATOR);
  bool isConfigured = false;
  if(endIndex != std::string::npos) {
    configString = configString.substr(0, endIndex);
    isConfigured = true;
  }
  const auto structTypeNameId = GEN_STRUCT_MUX::getStructNameId(configString);

  if(structTypeNameId == CStringDictionary::scmInvalidStringId){
    DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: Structure name for %s does not exist\n", paConfigString);
    return false;
  }

  std::unique_ptr<CIEC_ANY> data(CTypeLib::createDataTypeInstance(structTypeNameId, nullptr));

  if(nullptr == data) {
    DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: Couldn't create structure of type: %s\n", CStringDictionary::get(structTypeNameId));
    return false;
  }

  if(data->getDataTypeID() != CIEC_ANY::e_STRUCT) {
    DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: data type is not a structure: %s\n", CStringDictionary::get(structTypeNameId));
    return false;
  }

  // we could find the struct
  auto structInstance = static_cast<CIEC_STRUCT*>(data.get());
  size_t structSize;
  if(isConfigured) {
    std::string_view configuredMemberNamesStr {paConfigString};
    configuredMemberNamesStr = configuredMemberNamesStr.substr(endIndex + STRUCT_NAME_SEPARATOR.size()); 
    std::vector<std::string_view> configuredMemberNames = getConfiguredMemberNames(configuredMemberNamesStr);
    structSize = configuredMemberNames.size();
    mDoDataTypeNames = std::make_unique<CStringDictionary::TStringId[]>(calcConfiguredStructTypeNameSize(structInstance, configuredMemberNames));
    mDoNames = std::make_unique<CStringDictionary::TStringId[]>(structSize);
    fillConfiguredInterfaceSpec(structInstance, configuredMemberNames);
  } else {
    structSize = structInstance->getStructSize();
    mDoDataTypeNames = std::make_unique<CStringDictionary::TStringId[]>(GEN_STRUCT_MUX::calcStructTypeNameSize(*structInstance));
    mDoNames = std::make_unique<CStringDictionary::TStringId[]>(structSize);
    fillInterfaceSpec(structInstance);
  }
  if(structSize == 0 || structSize >= cgInvalidPortId) { //the structure size must be non zero and less than cgInvalidPortId (maximum number of data input)
    DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: The structure %s has a size that is not within range > 0 and < %u\n",
                CStringDictionary::get(structTypeNameId), cgInvalidPortId);
    return false;
  }

  mDiDataTypeNames[0] = structTypeNameId;
  paInterfaceSpec.mNumEIs = 1;
  paInterfaceSpec.mEINames = scmEventInputNames;
  paInterfaceSpec.mEITypeNames = scmEventInputTypeIds;
  paInterfaceSpec.mNumEOs = 1;
  paInterfaceSpec.mEONames = scmEventOutputNames;
  paInterfaceSpec.mEOTypeNames = scmEventOutputTypeIds;
  paInterfaceSpec.mNumDIs = 1;
  paInterfaceSpec.mDINames = scmDataInputNames;
  paInterfaceSpec.mDIDataTypeNames = mDiDataTypeNames.data();
  paInterfaceSpec.mNumDOs = structSize;
  paInterfaceSpec.mDONames = mDoNames.get();
  paInterfaceSpec.mDODataTypeNames = mDoDataTypeNames.get();

  return true;
}

void GEN_STRUCT_DEMUX::fillConfiguredInterfaceSpec(CIEC_STRUCT *paStructType, std::vector<std::string_view> &paConfiguredMemberNames) {
  auto doDataTypeNames = mDoDataTypeNames.get();
  for(size_t i = 0; i < paConfiguredMemberNames.size(); ++i) {
    CStringDictionary::TStringId memberNameId = CStringDictionary::insert(paConfiguredMemberNames[i].data(), paConfiguredMemberNames[i].length());
    mDoNames[i] = memberNameId;
    CIEC_ANY *member = getNestedMember(memberNameId, paStructType);
    fillDataPointSpec(*member, doDataTypeNames);
  }
}

void GEN_STRUCT_DEMUX::fillInterfaceSpec(CIEC_STRUCT *paStructType) {
  auto doDataTypeNames = mDoDataTypeNames.get();
  for(size_t i = 0; i < paStructType->getStructSize(); ++i) {
    const CIEC_ANY &member = *paStructType->getMember(i);
    mDoNames[i] = paStructType->elementNames()[i];
    fillDataPointSpec(member, doDataTypeNames);
  }
}

std::vector<std::string_view> GEN_STRUCT_DEMUX::getConfiguredMemberNames(std::string_view paMemberNameString) {
  std::vector<std::string_view> configuredMemberNames;
  size_t nextIndex = paMemberNameString.find(',');
  while(nextIndex != std::string::npos) {
    configuredMemberNames.emplace_back(paMemberNameString.substr(0, nextIndex));
    paMemberNameString = paMemberNameString.substr(nextIndex + 1);
    nextIndex = paMemberNameString.find(',');
  }
  if(!paMemberNameString.empty()) {
    configuredMemberNames.emplace_back(paMemberNameString.substr(0));
  }
  return configuredMemberNames;
}

void GEN_STRUCT_DEMUX::setConfiguredDOPorts() {
  const SFBInterfaceSpec interfaceSpec = getGenInterfaceSpec();
  const CStringDictionary::TStringId *names = interfaceSpec.mDONames;
  CIEC_STRUCT *structType = &st_IN();
  for(TPortId i = 0; i < interfaceSpec.mNumDOs; i++){
    CIEC_ANY *member = getNestedMember(names[i], structType);
    mConfiguredDOPorts.emplace_back(member);
  }
}

CIEC_ANY *GEN_STRUCT_DEMUX::getNestedMember(const CStringDictionary::TStringId paNameId, CIEC_STRUCT *paStructType) {
  std::string_view memberName {CStringDictionary::get(paNameId)};
  size_t index = memberName.find(NESTED_VAR_SEPARATOR);
  CIEC_STRUCT *structType = paStructType;
  while(index != std::string::npos) {
    std::string_view nestedStructName = memberName.substr(0, index);
    structType = static_cast<CIEC_STRUCT*>(structType->getMemberNamed(CStringDictionary::getId(nestedStructName.data(), nestedStructName.length())));
    memberName = memberName.substr(index + 1);
    index = memberName.find(NESTED_VAR_SEPARATOR);
  }
  return structType->getMemberNamed(CStringDictionary::getId(memberName.data(), memberName.length()));
}

size_t GEN_STRUCT_DEMUX::calcConfiguredStructTypeNameSize(CIEC_STRUCT *paStructType, std::vector<std::string_view> &paConfiguredMemberNames) {
  size_t result = 0;
  for(std::string_view memberName : paConfiguredMemberNames) {
    CStringDictionary::TStringId memberNameId = CStringDictionary::insert(memberName.data(), memberName.length());
    CIEC_ANY *member = getNestedMember(memberNameId, paStructType);
    result += getDataPointSpecSize(*member);
  }
  return result;
}