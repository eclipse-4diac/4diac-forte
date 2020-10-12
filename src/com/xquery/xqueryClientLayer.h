/*********************************************************************
* Copyright (c) 2017 fortiss GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef SRC_MODULES_XQUERY_XQUERYCLIENT_H_
#define SRC_MODULES_XQUERY_XQUERYCLIENT_H_

#include <comlayer.h>
#include <extevhan.h>

using namespace forte::com_infra;

class CXqueryClientLayer : public forte::com_infra::CComLayer{
  public:
    CXqueryClientLayer(forte::com_infra::CComLayer* pa_poUpperLayer, forte::com_infra::CBaseCommFB* pa_poComFB);
    virtual ~CXqueryClientLayer();

    EComResponse sendData(void *pa_pvData, unsigned int pa_unSize);
    EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);
    EComResponse processInterrupt();

    const char * getCommand(){
      return command;
    }
    int getSfd(){
      return sfd;
    }

  private:
    void closeConnection();
    EComResponse openConnection(char *pa_acLayerParameter);
    bool parseParameters(char *pa_acLayerParameter);
    void openDB();

    int sfd;
    char *host;
    char *port;
    char *dbName;
    char *usr;
    char *psw;
    static const char *scmParameterSeperator;
    char *command;
};

#endif /* SRC_MODULES_XQUERY_XQUERYCLIENT_H_ */
