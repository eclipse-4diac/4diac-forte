/*******************************************************************************
 * Copyright (c) 2012, 2023 AIT, Hit robot group
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Tibalt Zhao  - Merge additem into opc connect
 *   Ketut Kumajaya - Code refactoring from char* to std::string
 *******************************************************************************/
#ifndef CMDADDCONNECTION_H_
#define CMDADDCONNECTION_H_
#include "ICmd.h"
#include "comlayer.h"
#include <vector>
#include <string>


class COpcProcessVar;
class COpcConnectionImpl;
class COpcConnection;

class CCmd_AddConnection : public ICmd{
  public:
    explicit CCmd_AddConnection(COpcConnectionImpl *paConnection, const std::string& paGroupName, unsigned long paReqUpdateRate, float paDeadBand,
        std::vector<COpcProcessVar*> paNewItems) ;
    ~CCmd_AddConnection() = default;

    void runCommand() override;
    const char* getCommandName() const override;

  private:
    COpcConnectionImpl* mConnection;
    const std::string mGroupName;
    unsigned long mReqUpdateRate;
    float mDeadBand;
    std::vector<std::string> mReadItems;
    std::vector<std::string> mWriteItems;
};

#endif // CMDADDCONNECTION_H_
