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
#ifndef CMDADDCONNECTION_H_
#define CMDADDCONNECTION_H_
#include "ICmd.h"
#include "comlayer.h"
#include <vector>
#include <string>

class COpcConnectionImpl;

class CCmd_RemoveConnection : public ICmd{
  public:
    explicit CCmd_RemoveConnection(COpcConnectionImpl &paConnection) ;
    ~CCmd_RemoveConnection() = default;

    void runCommand() override;
    const char* getCommandName() const override {
      return "Cmd_RemoveConnection";
    }

  private:
    COpcConnectionImpl &mConnection;
};

#endif // CMDADDCONNECTION_H_
