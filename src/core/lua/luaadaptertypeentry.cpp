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

#include "luaengine.h"
#include "luaadaptertypeentry.h"
#include "luatype.h"
#include "luaadapter.h"

CLuaAdapterTypeEntry::CLuaAdapterTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING paLuaScriptAsString,  SFBInterfaceSpec& interfaceSpec)
    : CTypeLib::CAdapterTypeEntry(typeNameId, 0), cm_sLuaScriptAsString(paLuaScriptAsString), mSocketInterfaceSpec(interfaceSpec) {
  initPlugInterfaceSpec(mSocketInterfaceSpec);
}

CLuaAdapterTypeEntry::~CLuaAdapterTypeEntry() {
  deleteInterfaceSpec(mSocketInterfaceSpec);
}

CLuaAdapterTypeEntry* CLuaAdapterTypeEntry::createLuaAdapterTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING& paLuaScriptAsString) {
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
  luaEngine.pop(); //pop loaded defs
  return new CLuaAdapterTypeEntry(typeNameId, paLuaScriptAsString, interfaceSpec);
}

CAdapter* CLuaAdapterTypeEntry::createAdapterInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, bool pa_bIsPlug) {
  CLuaEngine* luaEngine = pa_poSrcRes->getLuaEngine();
  if (!luaEngine->load(this)) {
    if (!luaEngine->loadString(std::string(cm_sLuaScriptAsString.getValue()))) {
      return NULL;
    }
  }
  TForteByte* connData = 0;
  TForteByte* varsData = 0;
  if(pa_bIsPlug){
    connData = new TForteByte[CAdapter::genAdapterFBConnDataSize(mPlugInterfaceSpec.m_nNumEIs, mPlugInterfaceSpec.m_nNumEOs, mPlugInterfaceSpec.m_nNumDIs, mPlugInterfaceSpec.m_nNumDOs)];
    varsData = new TForteByte[CAdapter::genFBVarsDataSize(mPlugInterfaceSpec.m_nNumDIs, mPlugInterfaceSpec.m_nNumDOs, mPlugInterfaceSpec.m_nNumAdapters)];
  }else{
    connData = new TForteByte[CAdapter::genAdapterFBConnDataSize(mSocketInterfaceSpec.m_nNumEIs, mSocketInterfaceSpec.m_nNumEOs, mSocketInterfaceSpec.m_nNumDIs, mSocketInterfaceSpec.m_nNumDOs)];
    varsData = new TForteByte[CAdapter::genFBVarsDataSize(mSocketInterfaceSpec.m_nNumDIs, mSocketInterfaceSpec.m_nNumDOs, mSocketInterfaceSpec.m_nNumAdapters)];
  }
  return new CLuaAdapter(pa_nInstanceNameId, this, pa_bIsPlug, connData, varsData, pa_poSrcRes);
}

bool CLuaAdapterTypeEntry::initInterfaceSpec(SFBInterfaceSpec& interfaceSpec, CLuaEngine* luaEngine, int index) {
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
  //checks
  if (interfaceSpec.m_aunEINames == NULL || interfaceSpec.m_anEIWith == NULL || interfaceSpec.m_anEIWithIndexes == NULL || interfaceSpec.m_aunEONames == NULL
      || interfaceSpec.m_anEOWith == NULL || interfaceSpec.m_anEOWithIndexes == NULL || interfaceSpec.m_aunDINames == NULL
      || interfaceSpec.m_aunDIDataTypeNames == NULL || interfaceSpec.m_aunDONames == NULL || interfaceSpec.m_aunDODataTypeNames == NULL) {
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

bool CLuaAdapterTypeEntry::initPlugInterfaceSpec(SFBInterfaceSpec& interfaceSpec) {
  //EI
  mPlugInterfaceSpec.m_nNumEIs = interfaceSpec.m_nNumEOs;
  mPlugInterfaceSpec.m_aunEINames = interfaceSpec.m_aunEONames;
  mPlugInterfaceSpec.m_anEIWith = interfaceSpec.m_anEOWith;
  mPlugInterfaceSpec.m_anEIWithIndexes = interfaceSpec.m_anEOWithIndexes;
  //EO
  mPlugInterfaceSpec.m_nNumEOs = interfaceSpec.m_nNumEIs;
  mPlugInterfaceSpec.m_aunEONames = interfaceSpec.m_aunEINames;
  mPlugInterfaceSpec.m_anEOWith = interfaceSpec.m_anEIWith;
  mPlugInterfaceSpec.m_anEOWithIndexes = interfaceSpec.m_anEIWithIndexes;
  //DI
  mPlugInterfaceSpec.m_nNumDIs = interfaceSpec.m_nNumDOs;
  mPlugInterfaceSpec.m_aunDINames = interfaceSpec.m_aunDONames;
  mPlugInterfaceSpec.m_aunDIDataTypeNames = interfaceSpec.m_aunDODataTypeNames;
  //DO
  mPlugInterfaceSpec.m_nNumDOs = interfaceSpec.m_nNumDIs;
  mPlugInterfaceSpec.m_aunDONames = interfaceSpec.m_aunDINames;
  mPlugInterfaceSpec.m_aunDODataTypeNames = interfaceSpec.m_aunDIDataTypeNames;
  return true;
}

void CLuaAdapterTypeEntry::deleteInterfaceSpec(SFBInterfaceSpec& interfaceSpec) {
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
}
