/*******************************************************************************
 * Copyright (c) 2012, 2022 AIT, ACIN, fortiss GmbH, Hit robot group
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Tibalt Zhao - replace forte list with stl vector
 *   Ketut Kumajaya - Code refactoring from char* to std::string
 *******************************************************************************/
#ifndef OPCCOMLAYER_H_
#define OPCCOMLAYER_H_

#include "forte_config.h"
#include "comlayer.h"
#include "opcprocessvar.h"
#include <vector>
#include <deque>

class CIEC_ANY;
class COpcConnection;
struct Variant;

namespace forte {

  namespace com_infra {

    class COpcComLayer : public CComLayer{
      public:
        COpcComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB);
        ~COpcComLayer() override;

        EComResponse sendData(void *paData, unsigned int paSize) override; // top interface, called from top
        EComResponse recvData(const void *paData, unsigned int paSize) override;
        EComResponse processInterrupt() override;

      private:
        EComResponse openConnection(char *paLayerParameter) override;
        void closeConnection() override;
        void setOutputValue(CIEC_ANY *paDataOut, Variant *paValue);
        void processClientParams(char* paLayerParams);
        void convertInputData(void *paData, unsigned int paSize);

        unsigned int getInputValueSize(CIEC_ANY* paData, Variant * paNewValue);

        template<typename T>
        void getInputValue(void * paData, Variant * paNewValue);

        std::string mHost;
        std::string mServerName;
        long mUpdateRate;
        float mDeadBand;
        const std::string mOpcGroupName;

        bool mLayerParamsOK;

        COpcConnection *mOpcConnection;

        typedef std::vector<COpcProcessVar*> TOpcProcessVarList;
        TOpcProcessVarList mFBOutputVars;
        TOpcProcessVarList mFBInputVars;

        EComResponse mInterruptResp;
        typedef std::deque<EComResponse> TComResponseList;
        TComResponseList mComResponses;

        CSyncObject mSync;

        enum Parameters {
          Host,
          ServerName,
          UpdateRate,
          DeadBand,
          FbInputItems,
          FbOutputItems,
          ParameterAmount
        };
    };

  }

}

#endif /* OPCCOMLAYER_H_ */
