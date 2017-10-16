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

#include "luabfbtypeentry.h"

#include "luaengine.h"
#include "luabfb.h"
#include "luatype.h"
#include "resource.h"

CLuaBFBTypeEntry::CLuaBFBTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING paLuaScriptAsString, SFBInterfaceSpec& interfaceSpec,
    SInternalVarsInformation& internalVarsInformation)
    : CTypeLib::CFBTypeEntry(typeNameId, 0), cm_sLuaScriptAsString(paLuaScriptAsString), m_interfaceSpec(interfaceSpec), m_internalVarsInformation(internalVarsInformation) {
}

CLuaBFBTypeEntry::~CLuaBFBTypeEntry() {
  deleteInterfaceSpec(m_interfaceSpec);
  deleteInternalVarsInformation(m_internalVarsInformation);
}

CLuaBFBTypeEntry* CLuaBFBTypeEntry::createLuaFBTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING& paLuaScriptAsString) {
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
  //internalVarsInformation
  SInternalVarsInformation internalVarsInformation = { };
  if (!luaEngine.pushField(-1, "internalVarsInformation", LUA_TTABLE)) {
    deleteInterfaceSpec(interfaceSpec);
    return NULL;
  }
  if (!initInternalVarsInformation(internalVarsInformation, &luaEngine, -1)) {
    deleteInterfaceSpec(interfaceSpec);
    deleteInternalVarsInformation(internalVarsInformation);
    return NULL;
  }
  luaEngine.pop(); //pop internalVarsInformation
  luaEngine.pop(); //pop loaded defs
  return new CLuaBFBTypeEntry(typeNameId, paLuaScriptAsString, interfaceSpec, internalVarsInformation);
}

CFunctionBlock* CLuaBFBTypeEntry::createFBInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) {
  CLuaEngine* luaEngine = pa_poSrcRes->getLuaEngine();
  if (!luaEngine->load(this)) {
    if (!luaEngine->loadString(std::string(cm_sLuaScriptAsString.getValue()))) {
      return NULL;
    }
    luaEngine->pushField(-1, "ECC", LUA_TFUNCTION);
    luaEngine->store(this); //store ECC
  }
  luaEngine->pop(); //pop ECC / loaded defs
  TForteByte* connData = new TForteByte[CFunctionBlock::genFBConnDataSize(m_interfaceSpec.m_nNumEOs, m_interfaceSpec.m_nNumDIs, m_interfaceSpec.m_nNumDOs)];
  TForteByte* varsData = new TForteByte[CBasicFB::genBasicFBVarsDataSize(m_interfaceSpec.m_nNumDIs, m_interfaceSpec.m_nNumDOs,
      m_internalVarsInformation.m_nNumIntVars, m_interfaceSpec.m_nNumAdapters)];
  return new CLuaBFB(pa_nInstanceNameId, this, connData, varsData, pa_poSrcRes);
}

bool CLuaBFBTypeEntry::initInterfaceSpec(SFBInterfaceSpec& interfaceSpec, CLuaEngine* luaEngine, int index) {
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

void CLuaBFBTypeEntry::deleteInterfaceSpec(SFBInterfaceSpec& interfaceSpec) {
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

bool CLuaBFBTypeEntry::initInternalVarsInformation(SInternalVarsInformation& internalVarsInformation, CLuaEngine* luaEngine, int index) {
  internalVarsInformation.m_nNumIntVars = luaEngine->getField<TPortId, &CLuaEngine::getInteger<TPortId> >(index, "numIntVars");
  size_t numIntVars = internalVarsInformation.m_nNumIntVars;
  internalVarsInformation.m_aunIntVarsNames = luaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "intVarsNames",
      numIntVars);
  size_t numIntVarTypes = SIZE_MAX;
  internalVarsInformation.m_aunIntVarsDataTypeNames = luaEngine->getCustomArrayField<CStringDictionary::TStringId, luatype::getTypeNameId>(index,
      "intVarsDataTypeNames", numIntVarTypes);
  if (internalVarsInformation.m_aunIntVarsNames == NULL || internalVarsInformation.m_aunIntVarsDataTypeNames == NULL) {
    return false;
  }
  return true;
}

void CLuaBFBTypeEntry::deleteInternalVarsInformation(SInternalVarsInformation& internalVarsInformation) {
  delete[] internalVarsInformation.m_aunIntVarsNames;
  delete[] internalVarsInformation.m_aunIntVarsDataTypeNames;
}
