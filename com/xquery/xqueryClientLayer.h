/*********************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 **********************************************************************/

#pragma once

#include "forte/cominfra/comlayer.h"

namespace forte::com_infra::xquery {

  class CXqueryClientLayer : public CComLayer {
    public:
      CXqueryClientLayer(CComLayer *paUpperLayer, CBaseCommFB *paComFB);
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

} // namespace forte::com_infra::xquery
