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

#ifndef SRC_CORE_LUAADAPTER_H_
#define SRC_CORE_LUAADAPTER_H_

#include "adapter.h"
#include "luaadaptertypeentry.h"

class CLuaAdapter : public CAdapter {
  public:
    CLuaAdapter(CStringDictionary::TStringId paInstanceNameId, const CLuaAdapterTypeEntry* paTypeEntry, bool paIsPlug, TForteByte *paConnData,
        TForteByte *paVarsData, CResource *paResource);
    virtual ~CLuaAdapter();

    virtual CStringDictionary::TStringId getFBTypeId(void) const {
      return mTypeEntry->getTypeNameId();
    }

  private:
    const CLuaAdapterTypeEntry* mTypeEntry;
};

#endif /* SRC_CORE_LUAADAPTER_H_ */
