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
#ifndef CMDREMOVECONNECTION_H_
#define CMDREMOVECONNECTION_H_
#include "ICmd.h"
#include "comlayer.h"
#include <vector>
#include <string>

class COpcConnectionImpl;

class CCmd_RemoveConnection : public ICmd{
  public:
    explicit CCmd_RemoveConnection(COpcConnectionImpl &paConnection, const std::string& paGroupName, bool paIfDisconnect) ;
    ~CCmd_RemoveConnection() = default;

    void runCommand() override;
    const char* getCommandName() const override {
      if (mDisconnect) {
        return "Cmd_RemoveConnection";
      }
      else {
        return "Cmd_RemoveGroup";
      }
    }

  private:
    COpcConnectionImpl &mConnection;
    const std::string mGroupName;
    bool mDisconnect;
};

#endif // CMDREMOVECONNECTION_H_
