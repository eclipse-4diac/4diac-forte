/*******************************************************************************
 * Copyright (c) 2023 Johannes Kepler University Linz
 *               2024 Samator Indo Gas
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Ketut Kumajaya - Remove groups and items on disconnect
 *******************************************************************************/
#include "Cmd_RemoveConnection.h"
#include "opcconnectionimpl.h"

CCmd_RemoveConnection::CCmd_RemoveConnection(COpcConnectionImpl &paConnectionimpl, const std::string& paGroupName, bool paIfDisconnect) :
      mConnection(paConnectionimpl), mGroupName(paGroupName), mDisconnect(paIfDisconnect){
}

void CCmd_RemoveConnection::runCommand(){
  if (mDisconnect) {
    mConnection.removeGroup(mGroupName); // the last FB group
    mConnection.disconnect();
  }
  else {
    mConnection.removeGroup(mGroupName);
  }
}
