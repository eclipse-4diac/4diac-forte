/*********************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Monika Wenger
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 **********************************************************************/

#include "forte/cominfra/commfb.h"
#include "forte/datatypes/forte_string.h"
#include "forte/util/devlog.h"
#include "xqueryClientLayer.h"
#include "xqueryHandler.h"
#include "forte/cominfra/comlayersmanager.h"
extern "C" {
#include <basexdbc.h>
}

using namespace forte::literals;
using namespace std::literals::string_literals;

namespace forte::com_infra::xquery {

  const char *CXqueryClientLayer::scmParameterSeperator = " ;:";

  namespace {
    [[maybe_unused]] const forte::com_infra::ComLayerManager::EntryImpl<CXqueryClientLayer> entry("xquery"_STRID);
  }

  CXqueryClientLayer::CXqueryClientLayer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
      CComLayer(paUpperLayer, paComFB) {
    mSfd = -1;
    mCommand = std::string("");
    mHost = nullptr;
    mPort = nullptr;
    mUsr = nullptr;
    mPsw = nullptr;
    mDbName = nullptr;
  }

  CXqueryClientLayer::~CXqueryClientLayer() = default;

  bool CXqueryClientLayer::parseParameters(char *paLayerParameter) {
    bool retVal = true;
    mHost = strtok(paLayerParameter, scmParameterSeperator);
    mPort = strtok(nullptr, scmParameterSeperator);
    mDbName = strtok(nullptr, scmParameterSeperator);
    mUsr = strtok(nullptr, scmParameterSeperator);
    mPsw = strtok(nullptr, scmParameterSeperator);
    if (nullptr == mHost || nullptr == mPort || nullptr == mUsr || nullptr == mPsw || nullptr == mDbName) {
      retVal = false;
    }
    return retVal;
  }

  forte::com_infra::EComResponse CXqueryClientLayer::openConnection(char *paLayerParameter) {
    EComResponse retVal = e_InitTerminated;
    if (e_Client == mFb->getComServiceType() && mFb->getNumRD() == 1 && mFb->getNumSD() == 1 &&
        parseParameters(paLayerParameter)) {
      mSfd = basex_connect(mHost, mPort);
      if (mSfd == -1) {
        DEVLOG_ERROR("Can not connect to BaseX server at '%s:%s'\n", mHost, mPort);
      } else {
        int rc = basex_authenticate(mSfd, mUsr, mPsw);
        if (rc == -1) {
          DEVLOG_ERROR("Access to DB denied.\n");
        } else {
          mFb->QO() = true_BOOL;
          DEVLOG_INFO("Connected to DB.\n");
          openDB();
          retVal = e_InitOk;
        }
      }
    } else {
      DEVLOG_ERROR("[xquery] supports only CLIENT_1 with ID = xquery[IP:port; dbName; usr; psw]\n");
    }
    return retVal;
  }

  void CXqueryClientLayer::openDB() {
    char *r = nullptr;
    char *i = nullptr;
    mCommand = "open "s + mDbName;
    int ret = basex_execute(mSfd, mCommand.c_str(), &r, &i);
    if (ret == -1) { // general (i/o or the like) error
      DEVLOG_ERROR("An error occured during opening of DB'.\n");
      free(r);
      free(i);
    }
    if (ret == 1) { // database error while processing command
      DEVLOG_ERROR("Processing of '%s' failed.\n", mCommand.c_str());
    } else {
      DEVLOG_INFO("DB %s opend.\n", mDbName);
    }
  }

  void CXqueryClientLayer::closeConnection() {
    if (mSfd > -1) {
      basex_close(mSfd);
      DEVLOG_INFO("Disonnected from DB.\n");
    }
  }

  forte::com_infra::EComResponse CXqueryClientLayer::sendData(void *paData, unsigned int paSize) {
    EComResponse retVal = e_ProcessDataOk;
    if (0 == paSize) {
      retVal = e_ProcessDataInvalidObject;
    } else {
      const CIEC_ANY &SDs0 = getSDx(paData, 0);
      if (CIEC_ANY::e_STRING == SDs0.getDataTypeID()) {
        mCommand = static_cast<const CIEC_STRING &>(SDs0).getStorage();
        getExtEvHandler<CXqueryHandler>().registerLayer(this);
      } else {
        retVal = e_ProcessDataInvalidObject;
      }
    }
    return retVal;
  }

  forte::com_infra::EComResponse CXqueryClientLayer::recvData(const void *paData, unsigned int paSize) {
    EComResponse retVal = e_Nothing;
    if (paSize > 0) {
      mFb->getRDs()[0]->setValue(CIEC_STRING(static_cast<const char *>(paData), paSize));
      mFb->interruptCommFB(this);
      retVal = e_ProcessDataOk;
    }
    return retVal;
  }

  forte::com_infra::EComResponse CXqueryClientLayer::processInterrupt() {
    return e_ProcessDataOk;
  }

} // namespace forte::com_infra::xquery
