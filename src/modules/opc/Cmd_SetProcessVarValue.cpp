/*******************************************************************************
 * Copyright (c) 2012 AIT
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
