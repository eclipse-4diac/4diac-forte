/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Jobst
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luatype.h"
#include "luaengine.h"
#include "luafb.h"

const CLuaFBTypeEntry::Test CLuaFBTypeEntry::testEntry;

namespace luatype {
  bool getAdapterInstanceDefinition(SAdapterInstanceDef& def, CLuaEngine* luaEngine, int index) {
    def.m_nAdapterNameID = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "adapterNameID");
    def.m_nAdapterTypeNameID = luaEngine->getField<CStringDictionary::TStringId, &CLuaEngine::getStringId>(index, "adapterTypeNameID");
    def.m_bIsPlug = luaEngine->getField<bool, &CLuaEngine::getBoolean>(index, "isPlug");
    if (def.m_nAdapterNameID == CStringDictionary::scm_nInvalidStringId || def.m_nAdapterTypeNameID == CStringDictionary::scm_nInvalidStringId) {
      return false;
    }
    return true;
  }

  bool getTypeNameId(CStringDictionary::TStringId& id, CLuaEngine* luaEngine, int index) {
    switch (luaEngine->type(index)) {
      case CLuaEngine::TNUMBER:
        id = luaEngine->getInteger<CStringDictionary::TStringId>(index);
        break;
      case CLuaEngine::TSTRING:
        id = luaEngine->getStringId(index);
        break;
      default:
        return false;
    }
    return true;
  }
}

CLuaFBTypeEntry::CLuaFBTypeEntry(CStringDictionary::TStringId typeNameId, std::string& definitionsPath, SFBInterfaceSpec& interfaceSpec,
    SInternalVarsInformation& internalVarsInformation)
    : CTypeLib::CFBTypeEntry(typeNameId, 0), definitionsPath(definitionsPath), interfaceSpec(interfaceSpec), internalVarsInformation(internalVarsInformation) {
}

CLuaFBTypeEntry::~CLuaFBTypeEntry() {
  deleteInterfaceSpec(interfaceSpec);
  deleteInternalVarsInformation(internalVarsInformation);
}

CLuaFBTypeEntry* CLuaFBTypeEntry::createLuaFBTypeEntry(CStringDictionary::TStringId typeNameId, std::string& definitionsPath) {
  CLuaEngine luaEngine;
  if (!luaEngine.loadFile(definitionsPath)) {
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
  return new CLuaFBTypeEntry(typeNameId, definitionsPath, interfaceSpec, internalVarsInformation);
}

CFunctionBlock* CLuaFBTypeEntry::createFBInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) {
  CLuaEngine* luaEngine = pa_poSrcRes->getLuaEngine();
  if (!luaEngine->load(this)) {
    if (!luaEngine->loadFile(definitionsPath)) {
      return NULL;
    }
    luaEngine->pushField(-1, "ECC", LUA_TFUNCTION);
    luaEngine->store(this); //store ECC
  }
  luaEngine->pop(); //pop ECC / loaded defs
  TForteByte* connData = new TForteByte[CFunctionBlock::genFBConnDataSize(interfaceSpec.m_nNumEOs, interfaceSpec.m_nNumDIs, interfaceSpec.m_nNumDOs)];
  TForteByte* varsData = new TForteByte[CBasicFB::genBasicFBVarsDataSize(interfaceSpec.m_nNumDIs, interfaceSpec.m_nNumDOs,
      internalVarsInformation.m_nNumIntVars, interfaceSpec.m_nNumAdapters)];
  return new CLuaFB(pa_nInstanceNameId, this, connData, varsData, pa_poSrcRes);
}

bool CLuaFBTypeEntry::initInterfaceSpec(SFBInterfaceSpec& interfaceSpec, CLuaEngine* luaEngine, int index) {
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

void CLuaFBTypeEntry::deleteInterfaceSpec(SFBInterfaceSpec& interfaceSpec) {
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

bool CLuaFBTypeEntry::initInternalVarsInformation(SInternalVarsInformation& internalVarsInformation, CLuaEngine* luaEngine, int index) {
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

void CLuaFBTypeEntry::deleteInternalVarsInformation(SInternalVarsInformation& internalVarsInformation) {
  delete[] internalVarsInformation.m_aunIntVarsNames;
  delete[] internalVarsInformation.m_aunIntVarsDataTypeNames;
}
