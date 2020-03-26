/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luacfbtypeentry.h"

#include "resource.h"
#include "luaengine.h"
#include "luacfb.h"
#include "luatype.h"

namespace luatype {
  bool getFBInstanceData(SCFB_FBInstanceData& paDef, CLuaEngine* paLuaEngine, int paIndex) {
    paDef.m_nFBInstanceNameId = paLuaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "fbNameID");
    paDef.m_nFBTypeNameId = paLuaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "fbTypeID");
    if(paDef.m_nFBInstanceNameId == CStringDictionary::scm_nInvalidStringId || paDef.m_nFBTypeNameId == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }

  bool getFBConnectionData(SCFB_FBConnectionData& paDef, CLuaEngine* paLuaEngine, int paIndex) {
    paDef.m_nDstFBNum = paLuaEngine->getField<int, &CLuaEngine::getInteger<int> >(paIndex, "dstFBNum");
    paDef.m_nDstId = paLuaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "dstID");
    paDef.m_nSrcFBNum = paLuaEngine->getField<int, &CLuaEngine::getInteger<int> >(paIndex, "srcFBNum");
    paDef.m_nSrcId = paLuaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "srcID");
    if(paDef.m_nDstId == CStringDictionary::scm_nInvalidStringId || paDef.m_nSrcId == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }

  bool getFBFannedOutConnectionData(SCFB_FBFannedOutConnectionData& paDef, CLuaEngine* paLuaEngine, int paIndex) {
    paDef.m_nConnectionNum = paLuaEngine->getField<int, &CLuaEngine::getInteger<int> >(paIndex, "connectionNum");
    paDef.m_nDstFBNum = paLuaEngine->getField<int, &CLuaEngine::getInteger<int> >(paIndex, "dstFBNum");
    paDef.m_nDstId = paLuaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "dstID");
    if(paDef.m_nDstId == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }

  bool getFBParameter(SCFB_FBParameter& paDef, CLuaEngine* paLuaEngine, int paIndex) {
    paDef.m_acParamValue = paLuaEngine->getField<const char *, &CLuaEngine::getString>(paIndex, "paramValue");
    paDef.m_nDINameID = paLuaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "diNameID");
    paDef.m_nFBNum = paLuaEngine->getField<int, &CLuaEngine::getInteger<int> >(paIndex, "fbNum");
    if(paDef.m_nDINameID == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }
}

CLuaCFBTypeEntry::CLuaCFBTypeEntry(CStringDictionary::TStringId paTypeNameId, CIEC_STRING paLuaScriptAsString, SFBInterfaceSpec& paInterfaceSpec,
    SCFB_FBNData& paFbnSpec) :
    CTypeLib::CFBTypeEntry(paTypeNameId, 0, &m_interfaceSpec), cm_sLuaScriptAsString(paLuaScriptAsString), m_interfaceSpec(paInterfaceSpec), m_fbnSpec(paFbnSpec) {
}

CLuaCFBTypeEntry::~CLuaCFBTypeEntry() {
  deleteInterfaceSpec(m_interfaceSpec);
  deleteFbnSpec(m_fbnSpec);
}

CLuaCFBTypeEntry* CLuaCFBTypeEntry::createLuaFBTypeEntry(CStringDictionary::TStringId paTypeNameId, CIEC_STRING& paLuaScriptAsString) {
  CLuaEngine luaEngine;
  if(!luaEngine.loadString(std::string(paLuaScriptAsString.getValue()))) {
    return NULL;
  }
  //interfaceSpec
  SFBInterfaceSpec interfaceSpec = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  if(!luaEngine.pushField(-1, "interfaceSpec", LUA_TTABLE)) {
    return NULL;
  }
  if(!initInterfaceSpec(interfaceSpec, &luaEngine, -1)) {
    deleteInterfaceSpec(interfaceSpec);
    return NULL;
  }
  luaEngine.pop(); //pop interfaceSpec
  //fbnSpec
  SCFB_FBNData fbnSpec = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  if(!luaEngine.pushField(-1, "fbnSpec", LUA_TTABLE)) {
    deleteInterfaceSpec(interfaceSpec);
    return NULL;
  }
  if(!initFbnSpec(fbnSpec, &luaEngine, -1)) {
    deleteInterfaceSpec(interfaceSpec);
    deleteFbnSpec(fbnSpec);
    return NULL;
  }
  luaEngine.pop(); //pop fbnSpec
  luaEngine.pop(); //pop loaded defs
  return new CLuaCFBTypeEntry(paTypeNameId, paLuaScriptAsString, interfaceSpec, fbnSpec);
}

CFunctionBlock* CLuaCFBTypeEntry::createFBInstance(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) {
  CLuaEngine* luaEngine = paSrcRes->getLuaEngine();
  if(!luaEngine->load(this) && (!luaEngine->loadString(std::string(cm_sLuaScriptAsString.getValue())))) {
    return NULL;
  }
  TForteByte* connData = new TForteByte[CFunctionBlock::genFBConnDataSize(m_interfaceSpec.m_nNumEOs, m_interfaceSpec.m_nNumDIs, m_interfaceSpec.m_nNumDOs)];
  TForteByte* varsData = new TForteByte[CCompositeFB::genFBVarsDataSize(m_interfaceSpec.m_nNumDIs, m_interfaceSpec.m_nNumDOs, m_interfaceSpec.m_nNumAdapters)];
  return new CLuaCFB(paInstanceNameId, this, getFbnSpec(), connData, varsData, paSrcRes);
}

bool CLuaCFBTypeEntry::initInterfaceSpec(SFBInterfaceSpec& paInterfaceSpec, CLuaEngine* paLuaEngine, int paIndex) {
  //EI
  paInterfaceSpec.m_nNumEIs = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numEIs");
  size_t numEIs = paInterfaceSpec.m_nNumEIs;
  paInterfaceSpec.m_aunEINames = paLuaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "EINames", numEIs);
  size_t numEIWith = SIZE_MAX;
  paInterfaceSpec.m_anEIWith = paLuaEngine->getArrayField<TDataIOID, &CLuaEngine::getInteger<TDataIOID> >(paIndex, "EIWith", numEIWith);
  paInterfaceSpec.m_anEIWithIndexes = paLuaEngine->getArrayField<TForteInt16, &CLuaEngine::getInteger<TForteInt16> >(paIndex, "EIWithIndexes", numEIs);
  //EO
  paInterfaceSpec.m_nNumEOs = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numEOs");
  size_t numEOs = paInterfaceSpec.m_nNumEOs;
  paInterfaceSpec.m_aunEONames = paLuaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "EONames", numEOs);
  size_t numEOWith = SIZE_MAX;
  paInterfaceSpec.m_anEOWith = paLuaEngine->getArrayField<TDataIOID, &CLuaEngine::getInteger<TDataIOID> >(paIndex, "EOWith", numEOWith);
  paInterfaceSpec.m_anEOWithIndexes = paLuaEngine->getArrayField<TForteInt16, &CLuaEngine::getInteger<TForteInt16> >(paIndex, "EOWithIndexes", numEOs);
  //DI
  paInterfaceSpec.m_nNumDIs = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numDIs");
  size_t numDIs = paInterfaceSpec.m_nNumDIs;
  paInterfaceSpec.m_aunDINames = paLuaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "DINames", numDIs);
  size_t numDIDataTypeNames = SIZE_MAX;
  paInterfaceSpec.m_aunDIDataTypeNames = paLuaEngine->getCustomArrayField<CStringDictionary::TStringId, luatype::getTypeNameId>(paIndex, "DIDataTypeNames",
    numDIDataTypeNames);
  //DO
  paInterfaceSpec.m_nNumDOs = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numDOs");
  size_t numDOs = paInterfaceSpec.m_nNumDOs;
  paInterfaceSpec.m_aunDONames = paLuaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "DONames", numDOs);
  size_t numDODataTypeNames = SIZE_MAX;
  paInterfaceSpec.m_aunDODataTypeNames = paLuaEngine->getCustomArrayField<CStringDictionary::TStringId, luatype::getTypeNameId>(paIndex, "DODataTypeNames",
    numDODataTypeNames);
  //Adapters
  paInterfaceSpec.m_nNumAdapters = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numAdapters");
  size_t numAdapters = paInterfaceSpec.m_nNumAdapters;
  paInterfaceSpec.m_pstAdapterInstanceDefinition = paLuaEngine->getCustomArrayField<SAdapterInstanceDef, luatype::getAdapterInstanceDefinition>(paIndex,
    "adapterInstanceDefinition", numAdapters);
  //checks
  if(paInterfaceSpec.m_aunEINames == NULL || paInterfaceSpec.m_anEIWith == NULL || paInterfaceSpec.m_anEIWithIndexes == NULL
    || paInterfaceSpec.m_aunEONames == NULL || paInterfaceSpec.m_anEOWith == NULL || paInterfaceSpec.m_anEOWithIndexes == NULL
    || paInterfaceSpec.m_aunDINames == NULL || paInterfaceSpec.m_aunDIDataTypeNames == NULL || paInterfaceSpec.m_aunDONames == NULL
    || paInterfaceSpec.m_aunDODataTypeNames == NULL || paInterfaceSpec.m_pstAdapterInstanceDefinition == NULL) {
    return false;
  }
  for(size_t i = 0; i < numEIs; i++) {
    if(paInterfaceSpec.m_anEIWithIndexes[i] >= (TForteInt16) numEIWith) {
      return false;
    }
  }
  for(size_t i = 0; i < numEOs; i++) {
    if(paInterfaceSpec.m_anEOWithIndexes[i] >= (TForteInt16) numEOWith) {
      return false;
    }
  }
  return true;
}

void CLuaCFBTypeEntry::deleteInterfaceSpec(SFBInterfaceSpec& paInterfaceSpec) {
  delete[] paInterfaceSpec.m_aunEINames;
  delete[] paInterfaceSpec.m_anEIWith;
  delete[] paInterfaceSpec.m_anEIWithIndexes;
  delete[] paInterfaceSpec.m_aunEONames;
  delete[] paInterfaceSpec.m_anEOWith;
  delete[] paInterfaceSpec.m_anEOWithIndexes;
  delete[] paInterfaceSpec.m_aunDINames;
  delete[] paInterfaceSpec.m_aunDIDataTypeNames;
  delete[] paInterfaceSpec.m_aunDONames;
  delete[] paInterfaceSpec.m_aunDODataTypeNames;
  delete[] paInterfaceSpec.m_pstAdapterInstanceDefinition;
}

bool CLuaCFBTypeEntry::initFbnSpec(SCFB_FBNData& paFbnSpec, CLuaEngine* paLuaEngine, int paIndex) {
  //internalFBs
  paFbnSpec.m_nNumFBs = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numFBs");
  size_t numFBs = paFbnSpec.m_nNumFBs;
  paFbnSpec.m_pstFBInstances = paLuaEngine->getCustomArrayField<SCFB_FBInstanceData, luatype::getFBInstanceData>(paIndex, "internalFBs", numFBs);
  //eventConnections
  paFbnSpec.m_nNumEventConnections = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numECons");
  size_t numECons = paFbnSpec.m_nNumEventConnections;
  paFbnSpec.m_pstEventConnections = paLuaEngine->getCustomArrayField<SCFB_FBConnectionData, luatype::getFBConnectionData>(paIndex, "eventConnections",
    numECons);
  //fannedOutEventConnections
  paFbnSpec.m_nNumFannedOutEventConnections = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numFECons");
  size_t numFECons = paFbnSpec.m_nNumFannedOutEventConnections;
  paFbnSpec.m_pstFannedOutEventConnections = paLuaEngine->getCustomArrayField<SCFB_FBFannedOutConnectionData, luatype::getFBFannedOutConnectionData>(paIndex,
    "fannedOutEventConnections", numFECons);
  //dataConnections
  paFbnSpec.m_nNumDataConnections = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numDCons");
  size_t numDCons = paFbnSpec.m_nNumDataConnections;
  paFbnSpec.m_pstDataConnections = paLuaEngine->getCustomArrayField<SCFB_FBConnectionData, luatype::getFBConnectionData>(paIndex, "dataConnections", numDCons);
  //fannedOutDataConnections
  paFbnSpec.m_nNumFannedOutDataConnections = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numFDCons");
  size_t numFDCons = paFbnSpec.m_nNumFannedOutDataConnections;
  paFbnSpec.m_pstFannedOutDataConnections = paLuaEngine->getCustomArrayField<SCFB_FBFannedOutConnectionData, luatype::getFBFannedOutConnectionData>(paIndex,
    "fannedOutDataConnections", numFDCons);
  //parameters
  paFbnSpec.m_nNumParams = paLuaEngine->getField<TForteUInt8, &CLuaEngine::getInteger<TForteUInt8> >(paIndex, "numParams");
  size_t numParams = paFbnSpec.m_nNumParams;
  paFbnSpec.m_pstParams = paLuaEngine->getCustomArrayField<SCFB_FBParameter, luatype::getFBParameter>(paIndex, "parameters", numParams);
  //checks
  if(paFbnSpec.m_pstFBInstances == NULL) {
    return false;
  }
  return true;
}

void CLuaCFBTypeEntry::deleteFbnSpec(SCFB_FBNData& paFbnSpec) {
  delete[] paFbnSpec.m_pstFBInstances;
  delete[] paFbnSpec.m_pstEventConnections;
  delete[] paFbnSpec.m_pstFannedOutEventConnections;
  delete[] paFbnSpec.m_pstDataConnections;
  delete[] paFbnSpec.m_pstFannedOutDataConnections;
  delete[] paFbnSpec.m_pstParams;
}
