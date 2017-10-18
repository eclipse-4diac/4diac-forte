/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luacfbtypeentry.h"

#include "luaengine.h"
#include "luacfb.h"
#include "luatype.h"

namespace luatype {
  bool getFBInstanceData(SCFB_FBInstanceData& def, CLuaEngine* luaEngine, int index) {
    def.m_nFBInstanceNameId = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "fbNameID");
    def.m_nFBTypeNameId = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "fbTypeID");
    if (def.m_nFBInstanceNameId == CStringDictionary::scm_nInvalidStringId || def.m_nFBTypeNameId == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }

  bool getFBConnectionData(SCFB_FBConnectionData& def, CLuaEngine* luaEngine, int index) {
    def.m_nDstFBNum = luaEngine->getField<int, &CLuaEngine::getInteger<int> >(index, "dstFBNum");
    def.m_nDstId = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "dstID");
    def.m_nSrcFBNum = luaEngine->getField<int, &CLuaEngine::getInteger<int> >(index, "srcFBNum");
    def.m_nSrcId = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "srcID");
    if (def.m_nDstId == CStringDictionary::scm_nInvalidStringId || def.m_nSrcId == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }

  bool getFBFannedOutConnectionData(SCFB_FBFannedOutConnectionData& def, CLuaEngine* luaEngine, int index) {
    def.m_nConnectionNum = luaEngine->getField<int, &CLuaEngine::getInteger<int> >(index, "connectionNum");
    def.m_nDstFBNum = luaEngine->getField<int, &CLuaEngine::getInteger<int> >(index, "dstFBNum");
    def.m_nDstId = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "dstID");
    if (def.m_nDstId == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }

  bool getFBParameter(SCFB_FBParameter& def, CLuaEngine* luaEngine, int index) {
    def.m_acParamValue = luaEngine->getField<char *, &CLuaEngine::getString>(index, "paramValue");
    def.m_nDINameID = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "diNameID");
    def.m_nFBNum = luaEngine->getField<int, &CLuaEngine::getInteger<int> >(index, "fbNum");
    if (def.m_nDINameID == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }
}

CLuaCFBTypeEntry::CLuaCFBTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING paLuaScriptAsString, SFBInterfaceSpec& interfaceSpec, SCFB_FBNData& fbnSpec)
    : CTypeLib::CFBTypeEntry(typeNameId, 0), cm_sLuaScriptAsString(paLuaScriptAsString), m_interfaceSpec(interfaceSpec), m_fbnSpec(fbnSpec) {
}

CLuaCFBTypeEntry::~CLuaCFBTypeEntry() {
  deleteInterfaceSpec(m_interfaceSpec);
  deleteFbnSpec(m_fbnSpec);
}

CLuaCFBTypeEntry* CLuaCFBTypeEntry::createLuaFBTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING& paLuaScriptAsString) {
  CLuaEngine luaEngine;
  if (!luaEngine.loadString(std::string(paLuaScriptAsString.getValue()))) {
    return NULL;
  }
  //interfaceSpec
  SFBInterfaceSpec interfaceSpec = { };
  if (!luaEngine.pushField(-1, "interfaceSpec", LUA_TTABLE)) {
    return NULL;
  }
  if (!initInterfaceSpec(interfaceSpec, &luaEngine, -1)) {
    deleteInterfaceSpec(interfaceSpec);
    return NULL;
  }
  luaEngine.pop(); //pop interfaceSpec
  //fbnSpec
  SCFB_FBNData fbnSpec = { };
  if (!luaEngine.pushField(-1, "fbnSpec", LUA_TTABLE)) {
    deleteInterfaceSpec(interfaceSpec);
    return NULL;
  }
  if (!initFbnSpec(fbnSpec, &luaEngine, -1)) {
    deleteInterfaceSpec(interfaceSpec);
    deleteFbnSpec(fbnSpec);
    return NULL;
  }
  luaEngine.pop(); //pop fbnSpec
  luaEngine.pop(); //pop loaded defs
  return new CLuaCFBTypeEntry(typeNameId, paLuaScriptAsString, interfaceSpec, fbnSpec);
}

CFunctionBlock* CLuaCFBTypeEntry::createFBInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) {
  CLuaEngine* luaEngine = pa_poSrcRes->getLuaEngine();
  if (!luaEngine->load(this)) {
    if (!luaEngine->loadString(std::string(cm_sLuaScriptAsString.getValue()))) {
      return NULL;
    }
  }
  TForteByte* connData = new TForteByte[CFunctionBlock::genFBConnDataSize(m_interfaceSpec.m_nNumEOs, m_interfaceSpec.m_nNumDIs, m_interfaceSpec.m_nNumDOs)];
  TForteByte* varsData = new TForteByte[CCompositeFB::genFBVarsDataSize(m_interfaceSpec.m_nNumDIs, m_interfaceSpec.m_nNumDOs, m_interfaceSpec.m_nNumAdapters)];
  return new CLuaCFB(pa_nInstanceNameId, this, getFbnSpec(), connData, varsData, pa_poSrcRes);
}

bool CLuaCFBTypeEntry::initInterfaceSpec(SFBInterfaceSpec& interfaceSpec, CLuaEngine* luaEngine, int index) {
  //EI
  interfaceSpec.m_nNumEIs = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numEIs");
  size_t numEIs = interfaceSpec.m_nNumEIs;
  interfaceSpec.m_aunEINames = luaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "EINames", numEIs);
  size_t numEIWith = SIZE_MAX;
  interfaceSpec.m_anEIWith = luaEngine->getArrayField<TDataIOID, &CLuaEngine::getInteger<TDataIOID> >(index, "EIWith", numEIWith);
  interfaceSpec.m_anEIWithIndexes = luaEngine->getArrayField<TForteInt16, &CLuaEngine::getInteger<TForteInt16> >(index, "EIWithIndexes", numEIs);
  //EO
  interfaceSpec.m_nNumEOs = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numEOs");
  size_t numEOs = interfaceSpec.m_nNumEOs;
  interfaceSpec.m_aunEONames = luaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "EONames", numEOs);
  size_t numEOWith = SIZE_MAX;
  interfaceSpec.m_anEOWith = luaEngine->getArrayField<TDataIOID, &CLuaEngine::getInteger<TDataIOID> >(index, "EOWith", numEOWith);
  interfaceSpec.m_anEOWithIndexes = luaEngine->getArrayField<TForteInt16, &CLuaEngine::getInteger<TForteInt16> >(index, "EOWithIndexes", numEOs);
  //DI
  interfaceSpec.m_nNumDIs = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numDIs");
  size_t numDIs = interfaceSpec.m_nNumDIs;
  interfaceSpec.m_aunDINames = luaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "DINames", numDIs);
  size_t numDIDataTypeNames = SIZE_MAX;
  interfaceSpec.m_aunDIDataTypeNames = luaEngine->getCustomArrayField<CStringDictionary::TStringId, luatype::getTypeNameId>(index, "DIDataTypeNames",
      numDIDataTypeNames);
  //DO
  interfaceSpec.m_nNumDOs = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numDOs");
  size_t numDOs = interfaceSpec.m_nNumDOs;
  interfaceSpec.m_aunDONames = luaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "DONames", numDOs);
  size_t numDODataTypeNames = SIZE_MAX;
  interfaceSpec.m_aunDODataTypeNames = luaEngine->getCustomArrayField<CStringDictionary::TStringId, luatype::getTypeNameId>(index, "DODataTypeNames",
      numDODataTypeNames);
  //Adapters
  interfaceSpec.m_nNumAdapters = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numAdapters");
  size_t numAdapters = interfaceSpec.m_nNumAdapters;
  interfaceSpec.m_pstAdapterInstanceDefinition = luaEngine->getCustomArrayField<SAdapterInstanceDef, luatype::getAdapterInstanceDefinition>(index,
      "adapterInstanceDefinition", numAdapters);
  //checks
  if (interfaceSpec.m_aunEINames == NULL || interfaceSpec.m_anEIWith == NULL || interfaceSpec.m_anEIWithIndexes == NULL || interfaceSpec.m_aunEONames == NULL
      || interfaceSpec.m_anEOWith == NULL || interfaceSpec.m_anEOWithIndexes == NULL || interfaceSpec.m_aunDINames == NULL
      || interfaceSpec.m_aunDIDataTypeNames == NULL || interfaceSpec.m_aunDONames == NULL || interfaceSpec.m_aunDODataTypeNames == NULL
      || interfaceSpec.m_pstAdapterInstanceDefinition == NULL) {
    return false;
  }
  for (size_t i = 0; i < numEIs; i++) {
    if (interfaceSpec.m_anEIWithIndexes[i] >= (TForteInt16) numEIWith) {
      return false;
    }
  }
  for (size_t i = 0; i < numEOs; i++) {
    if (interfaceSpec.m_anEOWithIndexes[i] >= (TForteInt16) numEOWith) {
      return false;
    }
  }
  return true;
}

void CLuaCFBTypeEntry::deleteInterfaceSpec(SFBInterfaceSpec& interfaceSpec) {
  delete[] interfaceSpec.m_aunEINames;
  delete[] interfaceSpec.m_anEIWith;
  delete[] interfaceSpec.m_anEIWithIndexes;
  delete[] interfaceSpec.m_aunEONames;
  delete[] interfaceSpec.m_anEOWith;
  delete[] interfaceSpec.m_anEOWithIndexes;
  delete[] interfaceSpec.m_aunDINames;
  delete[] interfaceSpec.m_aunDIDataTypeNames;
  delete[] interfaceSpec.m_aunDONames;
  delete[] interfaceSpec.m_aunDODataTypeNames;
  delete[] interfaceSpec.m_pstAdapterInstanceDefinition;
}

bool CLuaCFBTypeEntry::initFbnSpec(SCFB_FBNData& fbnSpec, CLuaEngine* luaEngine, int index) {
  //internalFBs
  fbnSpec.m_nNumFBs = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numFBs");
  size_t numFBs = fbnSpec.m_nNumFBs;
  fbnSpec.m_pstFBInstances =  luaEngine->getCustomArrayField<SCFB_FBInstanceData, luatype::getFBInstanceData>(index, "internalFBs", numFBs);
  //eventConnections
  fbnSpec.m_nNumEventConnections = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numECons");
  size_t numECons = fbnSpec.m_nNumEventConnections;
  fbnSpec.m_pstEventConnections = luaEngine->getCustomArrayField<SCFB_FBConnectionData, luatype::getFBConnectionData>(index, "eventConnections", numECons);
  //fannedOutEventConnections
  fbnSpec.m_nNumFannedOutEventConnections = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numFECons");
  size_t numFECons = fbnSpec.m_nNumFannedOutEventConnections;
  fbnSpec.m_pstFannedOutEventConnections = luaEngine->getCustomArrayField<SCFB_FBFannedOutConnectionData, luatype::getFBFannedOutConnectionData>(index, "fannedOutEventConnections", numFECons);
  //dataConnections
  fbnSpec.m_nNumDataConnections = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numDCons");
  size_t numDCons = fbnSpec.m_nNumDataConnections;
  fbnSpec.m_pstDataConnections = luaEngine->getCustomArrayField<SCFB_FBConnectionData, luatype::getFBConnectionData>(index, "dataConnections", numDCons);
  //fannedOutDataConnections
  fbnSpec.m_nNumFannedOutDataConnections = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numFDCons");
  size_t numFDCons = fbnSpec.m_nNumFannedOutDataConnections;
  fbnSpec.m_pstFannedOutDataConnections = luaEngine->getCustomArrayField<SCFB_FBFannedOutConnectionData, luatype::getFBFannedOutConnectionData>(index, "fannedOutDataConnections", numFDCons);
  //parameters
  fbnSpec.m_nNumParams = luaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(index, "numParams");
  size_t numParams = fbnSpec.m_nNumParams;
  fbnSpec.m_pstParams = luaEngine->getCustomArrayField<SCFB_FBParameter, luatype::getFBParameter>(index, "parameters", numParams);
  //checks
  if (fbnSpec.m_pstFBInstances == NULL) {
    return false;
  }
  return true;
}

void CLuaCFBTypeEntry::deleteFbnSpec(SCFB_FBNData& fbnSpec) {
  delete[] fbnSpec.m_pstFBInstances;
  delete[] fbnSpec.m_pstEventConnections;
  delete[] fbnSpec.m_pstFannedOutEventConnections;
  delete[] fbnSpec.m_pstDataConnections;
  delete[] fbnSpec.m_pstFannedOutDataConnections;
  delete[] fbnSpec.m_pstParams;
}
