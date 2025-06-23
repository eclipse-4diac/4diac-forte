/*********************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 **********************************************************************/

#ifndef SRC_MODULES_XQUERY_XQUERYCLIENT_H_
#define SRC_MODULES_XQUERY_XQUERYCLIENT_H_

#include "comlayer.h"
#include "extevhan.h"
#include <string.h>

using namespace forte::com_infra;

class CXqueryClientLayer : public forte::com_infra::CComLayer {
  public:
    CXqueryClientLayer(forte::com_infra::CComLayer *paUpperLayer, forte::com_infra::CBaseCommFB *paComFB);
    ~CXqueryClientLayer() override;

    EComResponse sendData(void *paData, unsigned int paSize) override;
    EComResponse recvData(const void *paData, unsigned int paSize) override;
    EComResponse processInterrupt() override;

    const char *getCommand() {
      return mCommand.c_str();
    }
    int getSfd() {
      return mSfd;
    }

  private:
    void closeConnection() override;
    EComResponse openConnection(char *paLayerParameter) override;
    bool parseParameters(char *paLayerParameter);
    void openDB();

    int mSfd;
    char *mHost;
    char *mPort;
    char *mDbName;
    char *mUsr;
    char *mPsw;
    std::string mCommand;
    static const char *scmParameterSeperator;
};

#endif /* SRC_MODULES_XQUERY_XQUERYCLIENT_H_ */
