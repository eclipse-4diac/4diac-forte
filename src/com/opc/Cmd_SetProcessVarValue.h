/*******************************************************************************
 * Copyright (c) 2012, 2022 AIT, HIT robot group
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Tibalt Zhao  - get rid of opcprocessvar from commfb
 *******************************************************************************/
#ifndef CMDSETPROCESSVARVALUE_H_
#define CMDSETPROCESSVARVALUE_H_

#include "ICmd.h"

#include <Windows.h>
#include "Variant.h"
#include <string>

class COpcProcessVar;
class COpcConnectionImpl;

class CCmd_SetProcessVarValue : public ICmd{
  public:
    explicit CCmd_SetProcessVarValue(COpcConnectionImpl *paConnection, std::string& paGroupName, std::string& paItemName, Variant paVar);
    ~CCmd_SetProcessVarValue() = default;

    void runCommand() override;
    const char* getCommandName() const override;

  private:
    COpcConnectionImpl* mConnection;
    COpcProcessVar* mOpcVar;
    std::string mGroupName;
    std::string mItemName;
    Variant mVar;
};

#endif //CMDSETPROCESSVARVALUE_H_
