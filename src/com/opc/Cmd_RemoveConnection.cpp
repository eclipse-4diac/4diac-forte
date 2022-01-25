/*******************************************************************************
 * Copyright (c) 2023 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "Cmd_RemoveConnection.h"
#include "opcconnectionimpl.h"

CCmd_RemoveConnection::CCmd_RemoveConnection(COpcConnectionImpl &paConnectionimpl) :
      mConnection(paConnectionimpl){
}

void CCmd_RemoveConnection::runCommand(){
  mConnection.disconnect();
}
