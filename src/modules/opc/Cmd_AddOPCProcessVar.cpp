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
