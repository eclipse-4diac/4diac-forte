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

#ifndef SRC_CORE_LUACFBTYPEENTRY_H_
#define SRC_CORE_LUACFBTYPEENTRY_H_

#include "typelib.h"
#include "cfb.h"

class CResource;
class CLuaEngine;

class CLuaCFBTypeEntry: public CTypeLib::CFBTypeEntry {
private:
  const CIEC_STRING cm_sLuaScriptAsString;
  SFBInterfaceSpec m_interfaceSpec;
  SCFB_FBNData m_fbnSpec;

  CLuaCFBTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING paLuaScriptAsString,  SFBInterfaceSpec& interfaceSpec, SCFB_FBNData& fbnSpec);

  virtual ~CLuaCFBTypeEntry();

  static bool initInterfaceSpec(SFBInterfaceSpec& interfaceSpec, CLuaEngine* luaEngine, int index);
  static void deleteInterfaceSpec(SFBInterfaceSpec& interfaceSpec);
  static bool initFbnSpec(SCFB_FBNData& fbnSpec, CLuaEngine* luaEngine, int index);
  static void deleteFbnSpec(SCFB_FBNData& fbnSpec);

public:
  static CLuaCFBTypeEntry* createLuaFBTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING& paLuaScriptAsString);

  virtual CFunctionBlock* createFBInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  const SFBInterfaceSpec* getInterfaceSpec() const {
    return &m_interfaceSpec;
  }

  SCFB_FBNData* getFbnSpec(){
    return &m_fbnSpec;
  }
};

#endif /* SRC_CORE_LUACFBTYPEENTRY_H_ */
