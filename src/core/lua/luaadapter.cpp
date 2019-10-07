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

#include "luaadapter.h"
#include "resource.h"

CLuaAdapter::CLuaAdapter(CStringDictionary::TStringId paInstanceNameId, const CLuaAdapterTypeEntry* paTypeEntry, bool paIsPlug, TForteByte *paConnData,
    TForteByte *paVarsData, CResource *paResource) :
    CAdapter(paResource, paTypeEntry->getSocketInterfaceSpec(), paInstanceNameId, paTypeEntry->getPlugInterfaceSpec(), paIsPlug, paConnData, paVarsData),
        mTypeEntry(paTypeEntry) {
}

CLuaAdapter::~CLuaAdapter() {
}
