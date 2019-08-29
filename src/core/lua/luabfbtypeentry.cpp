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

#include "luabfbtypeentry.h"

#include "luaengine.h"
#include "luabfb.h"
#include "luatype.h"
#include "resource.h"

CLuaBFBTypeEntry::CLuaBFBTypeEntry(CStringDictionary::TStringId paTypeNameId, CIEC_STRING paLuaScriptAsString, SFBInterfaceSpec& paInterfaceSpec,
    SInternalVarsInformation& paInternalVarsInformation) :
    CFBTypeEntry(paTypeNameId, 0, &m_interfaceSpec), cm_sLuaScriptAsString(paLuaScriptAsString), m_interfaceSpec(paInterfaceSpec),
        m_internalVarsInformation(paInternalVarsInformation) {
}

CLuaBFBTypeEntry::~CLuaBFBTypeEntry() {
  deleteInterfaceSpec(m_interfaceSpec);
  deleteInternalVarsInformation(m_internalVarsInformation);
}

CLuaBFBTypeEntry* CLuaBFBTypeEntry::createLuaFBTypeEntry(CStringDictionary::TStringId paTypeNameId, CIEC_STRING& paLuaScriptAsString) {
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
  //internalVarsInformation
  SInternalVarsInformation internalVarsInformation = { 0, 0, 0 };
  if(!luaEngine.pushField(-1, "internalVarsInformation", LUA_TTABLE)) {
    deleteInterfaceSpec(interfaceSpec);
    return NULL;
  }
  if(!initInternalVarsInformation(internalVarsInformation, &luaEngine, -1)) {
    deleteInterfaceSpec(interfaceSpec);
    deleteInternalVarsInformation(internalVarsInformation);
    return NULL;
  }
  luaEngine.pop(); //pop internalVarsInformation
  luaEngine.pop(); //pop loaded defs
  return new CLuaBFBTypeEntry(paTypeNameId, paLuaScriptAsString, interfaceSpec, internalVarsInformation);
}

CFunctionBlock* CLuaBFBTypeEntry::createFBInstance(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) {
  CLuaEngine* luaEngine = paSrcRes->getLuaEngine();
  if(!luaEngine->load(this)) {
    if(!luaEngine->loadString(std::string(cm_sLuaScriptAsString.getValue()))) {
      return NULL;
    }
    luaEngine->pushField(-1, "ECC", LUA_TFUNCTION);
    luaEngine->store(this); //store ECC
  }
  luaEngine->pop(); //pop ECC / loaded defs
  TForteByte* connData = new TForteByte[CFunctionBlock::genFBConnDataSize(m_interfaceSpec.m_nNumEOs, m_interfaceSpec.m_nNumDIs, m_interfaceSpec.m_nNumDOs)];
  TForteByte* varsData = new TForteByte[CBasicFB::genBasicFBVarsDataSize(m_interfaceSpec.m_nNumDIs, m_interfaceSpec.m_nNumDOs,
    m_internalVarsInformation.m_nNumIntVars, m_interfaceSpec.m_nNumAdapters)];
  return new CLuaBFB(paInstanceNameId, this, connData, varsData, paSrcRes);
}

bool CLuaBFBTypeEntry::initInterfaceSpec(SFBInterfaceSpec& paInterfaceSpec, CLuaEngine* paLuaEngine, int paIndex) {
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
  return true;
}

void CLuaBFBTypeEntry::deleteInterfaceSpec(SFBInterfaceSpec& paInterfaceSpec) {
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

bool CLuaBFBTypeEntry::initInternalVarsInformation(SInternalVarsInformation& paInternalVarsInformation, CLuaEngine* paLuaEngine, int paIndex) {
  paInternalVarsInformation.m_nNumIntVars = paLuaEngine->getField<TPortId, &CLuaEngine::getInteger<TPortId> >(paIndex, "numIntVars");
  size_t numIntVars = paInternalVarsInformation.m_nNumIntVars;
  paInternalVarsInformation.m_aunIntVarsNames = paLuaEngine->getArrayField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(paIndex, "intVarsNames",
    numIntVars);
  size_t numIntVarTypes = SIZE_MAX;
  paInternalVarsInformation.m_aunIntVarsDataTypeNames = paLuaEngine->getCustomArrayField<CStringDictionary::TStringId, luatype::getTypeNameId>(paIndex,
    "intVarsDataTypeNames", numIntVarTypes);
  if(paInternalVarsInformation.m_aunIntVarsNames == NULL || paInternalVarsInformation.m_aunIntVarsDataTypeNames == NULL) {
    return false;
  }
  return true;
}

void CLuaBFBTypeEntry::deleteInternalVarsInformation(SInternalVarsInformation& paInternalVarsInformation) {
  delete[] paInternalVarsInformation.m_aunIntVarsNames;
  delete[] paInternalVarsInformation.m_aunIntVarsDataTypeNames;
}
