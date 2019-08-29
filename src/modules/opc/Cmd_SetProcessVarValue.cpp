/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "Cmd_SetProcessVarValue.h"

#include "opcprocessvar.h"
#include "opcconnectionimpl.h"

CCmd_SetProcessVarValue::CCmd_SetProcessVarValue(COpcProcessVar * pa_pOpcVar) :
    m_pOpcVar(pa_pOpcVar){
}

CCmd_SetProcessVarValue::~CCmd_SetProcessVarValue(void){
}

void CCmd_SetProcessVarValue::runCommand(){
  m_pOpcVar->sendItemData();
}
const char* CCmd_SetProcessVarValue::getCommandName() const{
  return "Cmd_SetProcessVarValue";
}
