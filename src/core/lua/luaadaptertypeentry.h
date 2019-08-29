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

#ifndef SRC_CORE_LUAADAPTERTYPEENTRY_H_
#define SRC_CORE_LUAADAPTERTYPEENTRY_H_

#include "typelib.h"
#include "../funcbloc.h"

class CResource;
class CAdapter;
class CLuaEngine;

class CLuaAdapterTypeEntry: public CTypeLib::CAdapterTypeEntry {
private:

  const CIEC_STRING cm_sLuaScriptAsString;
  SFBInterfaceSpec mSocketInterfaceSpec;
  SFBInterfaceSpec mPlugInterfaceSpec;

  CLuaAdapterTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING paLuaScriptAsString,  SFBInterfaceSpec& interfaceSpec);

  virtual ~CLuaAdapterTypeEntry();

  static bool initInterfaceSpec(SFBInterfaceSpec& interfaceSpec, CLuaEngine* luaEngine, int index);
  bool initPlugInterfaceSpec(SFBInterfaceSpec& interfaceSpec);
  static void deleteInterfaceSpec(SFBInterfaceSpec& interfaceSpec);

public:
  static CLuaAdapterTypeEntry* createLuaAdapterTypeEntry(CStringDictionary::TStringId typeNameId, CIEC_STRING& paLuaScriptAsString);

  virtual CAdapter* createAdapterInstance(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, bool pa_bIsPlug);

  const SFBInterfaceSpec* getSocketInterfaceSpec() const {
    return &mSocketInterfaceSpec;
  }

  const SFBInterfaceSpec* getPlugInterfaceSpec() const {
      return &mPlugInterfaceSpec;
  }
};

#endif /* SRC_CORE_LUAADAPTERTYPEENTRY_H_ */
