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
#include "Cmd_SetProcessVarValue.h"

#include "opcprocessvar.h"
#include "opcconnectionimpl.h"

CCmd_SetProcessVarValue::CCmd_SetProcessVarValue(COpcConnectionImpl *paConnection, std::string &paGroupName,
      std::string &paItemName, Variant paVar) :
    mConnection(paConnection), mVar(paVar), mGroupName(paGroupName), mItemName(paItemName) {
}

void CCmd_SetProcessVarValue::runCommand() {
  mConnection->sendItemData(mGroupName.c_str(), mItemName.c_str(), mVar);
}

const char* CCmd_SetProcessVarValue::getCommandName() const {
  return "Cmd_SetProcessVarValue";
}
