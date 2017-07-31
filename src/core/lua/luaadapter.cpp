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

#include "luaadapter.h"
#include "resource.h"

CLuaAdapter::CLuaAdapter(CStringDictionary::TStringId instanceNameId, const CLuaAdapterTypeEntry* typeEntry, bool pa_bIsPlug, TForteByte *connData, TForteByte *varsData, CResource *resource)
    : CAdapter(resource, typeEntry->getSocketInterfaceSpec(), instanceNameId, typeEntry->getPlugInterfaceSpec(), pa_bIsPlug, connData, varsData){
  this->typeEntry = typeEntry;
}

CLuaAdapter::~CLuaAdapter() {
}
