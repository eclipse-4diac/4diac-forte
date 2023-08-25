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

CCmd_AddOPCProcessVar::CCmd_AddOPCProcessVar(COpcConnectionImpl *paConnection, COpcProcessVar* paNewItem) :
    mOpcConn(paConnection), mNewProcessVar(paNewItem){
}

CCmd_AddOPCProcessVar::~CCmd_AddOPCProcessVar(){
}

void CCmd_AddOPCProcessVar::runCommand(){
  mOpcConn->addItem(mNewProcessVar);
}

const char* CCmd_AddOPCProcessVar::getCommandName() const{
  return "Cmd_AddOPCProcessVar";
}
