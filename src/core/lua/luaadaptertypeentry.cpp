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

#include "luaadaptertypeentry.h"
#include "luaengine.h"
#include "luatype.h"
#include "luaadapter.h"
#include "resource.h"
#include "adapter.h"

CLuaAdapterTypeEntry::CLuaAdapterTypeEntry(CStringDictionary::TStringId paTypeNameId, CIEC_STRING paLuaScriptAsString, SFBInterfaceSpec& paInterfaceSpec) :
    CTypeLib::CAdapterTypeEntry(paTypeNameId, 0, &mSocketInterfaceSpec), cm_sLuaScriptAsString(paLuaScriptAsString), mSocketInterfaceSpec(paInterfaceSpec) {
  initPlugInterfaceSpec(mSocketInterfaceSpec);
}

CLuaAdapterTypeEntry::~CLuaAdapterTypeEntry() {
  deleteInterfaceSpec(mSocketInterfaceSpec);
}

CLuaAdapterTypeEntry* CLuaAdapterTypeEntry::createLuaAdapterTypeEntry(CStringDictionary::TStringId paTypeNameId, CIEC_STRING& paLuaScriptAsString) {
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
  luaEngine.pop(); //pop loaded defs
  return new CLuaAdapterTypeEntry(paTypeNameId, paLuaScriptAsString, interfaceSpec);
}

CAdapter* CLuaAdapterTypeEntry::createAdapterInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, bool pa_bIsPlug) {
  CLuaEngine* luaEngine = pa_poSrcRes->getLuaEngine();
  if(!luaEngine->load(this) && (!luaEngine->loadString(std::string(cm_sLuaScriptAsString.getValue())))) {
    return NULL;
  }
  TForteByte* connData = 0;
  TForteByte* varsData = 0;
  if(pa_bIsPlug) {
    connData = new TForteByte[CAdapter::genAdapterFBConnDataSize(mPlugInterfaceSpec.m_nNumEIs, mPlugInterfaceSpec.m_nNumEOs, mPlugInterfaceSpec.m_nNumDIs,
      mPlugInterfaceSpec.m_nNumDOs)];
    varsData = new TForteByte[CAdapter::genFBVarsDataSize(mPlugInterfaceSpec.m_nNumDIs, mPlugInterfaceSpec.m_nNumDOs, mPlugInterfaceSpec.m_nNumAdapters)];
  } else {
    connData = new TForteByte[CAdapter::genAdapterFBConnDataSize(mSocketInterfaceSpec.m_nNumEIs, mSocketInterfaceSpec.m_nNumEOs, mSocketInterfaceSpec.m_nNumDIs,
      mSocketInterfaceSpec.m_nNumDOs)];
    varsData = new TForteByte[CAdapter::genFBVarsDataSize(mSocketInterfaceSpec.m_nNumDIs, mSocketInterfaceSpec.m_nNumDOs, mSocketInterfaceSpec.m_nNumAdapters)];
  }
  return new CLuaAdapter(pa_nInstanceNameId, this, pa_bIsPlug, connData, varsData, pa_poSrcRes);
}

bool CLuaAdapterTypeEntry::initInterfaceSpec(SFBInterfaceSpec& paInterfaceSpec, CLuaEngine* paLuaEngine, int paIndex) {
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
  paInterfaceSpec.m_nNumAdapters = 0;
  paInterfaceSpec.m_pstAdapterInstanceDefinition = 0;
  //checks
  if(paInterfaceSpec.m_aunEINames == NULL || paInterfaceSpec.m_anEIWith == NULL || paInterfaceSpec.m_anEIWithIndexes == NULL
    || paInterfaceSpec.m_aunEONames == NULL || paInterfaceSpec.m_anEOWith == NULL || paInterfaceSpec.m_anEOWithIndexes == NULL
    || paInterfaceSpec.m_aunDINames == NULL || paInterfaceSpec.m_aunDIDataTypeNames == NULL || paInterfaceSpec.m_aunDONames == NULL
    || paInterfaceSpec.m_aunDODataTypeNames == NULL) {
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

bool CLuaAdapterTypeEntry::initPlugInterfaceSpec(SFBInterfaceSpec& paInterfaceSpec) {
  //EI
  mPlugInterfaceSpec.m_nNumEIs = paInterfaceSpec.m_nNumEOs;
  mPlugInterfaceSpec.m_aunEINames = paInterfaceSpec.m_aunEONames;
  mPlugInterfaceSpec.m_anEIWith = paInterfaceSpec.m_anEOWith;
  mPlugInterfaceSpec.m_anEIWithIndexes = paInterfaceSpec.m_anEOWithIndexes;
  //EO
  mPlugInterfaceSpec.m_nNumEOs = paInterfaceSpec.m_nNumEIs;
  mPlugInterfaceSpec.m_aunEONames = paInterfaceSpec.m_aunEINames;
  mPlugInterfaceSpec.m_anEOWith = paInterfaceSpec.m_anEIWith;
  mPlugInterfaceSpec.m_anEOWithIndexes = paInterfaceSpec.m_anEIWithIndexes;
  //DI
  mPlugInterfaceSpec.m_nNumDIs = paInterfaceSpec.m_nNumDOs;
  mPlugInterfaceSpec.m_aunDINames = paInterfaceSpec.m_aunDONames;
  mPlugInterfaceSpec.m_aunDIDataTypeNames = paInterfaceSpec.m_aunDODataTypeNames;
  //DO
  mPlugInterfaceSpec.m_nNumDOs = paInterfaceSpec.m_nNumDIs;
  mPlugInterfaceSpec.m_aunDONames = paInterfaceSpec.m_aunDINames;
  mPlugInterfaceSpec.m_aunDODataTypeNames = paInterfaceSpec.m_aunDIDataTypeNames;
  mPlugInterfaceSpec.m_nNumAdapters = 0;
  mPlugInterfaceSpec.m_pstAdapterInstanceDefinition = 0;
  return true;
}

void CLuaAdapterTypeEntry::deleteInterfaceSpec(SFBInterfaceSpec& paInterfaceSpec) {
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
}
