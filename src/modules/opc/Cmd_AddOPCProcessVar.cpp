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
#include "Cmd_AddOPCProcessVar.h"
#include "opcconnectionimpl.h"

CCmd_AddOPCProcessVar::CCmd_AddOPCProcessVar(COpcConnectionImpl *pa_pConnection, COpcProcessVar* pa_pNewItem) :
    m_pOpcConn(pa_pConnection), m_pNewProcessVar(pa_pNewItem){
}

CCmd_AddOPCProcessVar::~CCmd_AddOPCProcessVar(void){
}

void CCmd_AddOPCProcessVar::runCommand(){
  m_pOpcConn->addItem(m_pNewProcessVar);
}

const char* CCmd_AddOPCProcessVar::getCommandName() const{
  return "Cmd_AddOPCProcessVar";
}
