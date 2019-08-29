/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_CORE_LUAFBTYPEENTRY_H_
#define SRC_CORE_LUAFBTYPEENTRY_H_

#include "typelib.h"
#include "basicfb.h"

class CLuaEngine;

class CLuaBFBTypeEntry: public CTypeLib::CFBTypeEntry {
private:

  const CIEC_STRING cm_sLuaScriptAsString;
  SFBInterfaceSpec m_interfaceSpec;
  SInternalVarsInformation m_internalVarsInformation;

  CLuaBFBTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING paLuaScriptAsString, SFBInterfaceSpec& interfaceSpec,
      SInternalVarsInformation& internalVarsInformation);

  virtual ~CLuaBFBTypeEntry();

  static bool initInterfaceSpec(SFBInterfaceSpec& interfaceSpec, CLuaEngine* luaEngine, int index);
  static void deleteInterfaceSpec(SFBInterfaceSpec& interfaceSpec);
  static bool initInternalVarsInformation(SInternalVarsInformation& internalVarsInformation, CLuaEngine* luaEngine, int index);
  static void deleteInternalVarsInformation(SInternalVarsInformation& internalVarsInformation);
public:
  static CLuaBFBTypeEntry* createLuaFBTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING& paLuaScriptAsString);

  virtual CFunctionBlock* createFBInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  const SFBInterfaceSpec* getInterfaceSpec() const {
    return &m_interfaceSpec;
  }

  const SInternalVarsInformation* getInternalVarsInformation() const {
    return &m_internalVarsInformation;
  }
};

#endif /* SRC_CORE_LUABFBTYPEENTRY_H_ */
