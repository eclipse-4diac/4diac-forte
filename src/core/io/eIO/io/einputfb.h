/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#pragma once

#include "adapterconn.h"
#include "core/io/inputfb.h"
#include "eGenAdapter_adp.h"
#include "eConfigFB.h"

USE_STRING_ID(eCONF);
USE_STRING_ID(eGenAdapter);

namespace forte::core::io {

  template<class T>
  class CeInputFB : public CInputFB<T> {
    private:
      static const SAdapterInstanceDef scmAdapterInstances[];

      using CInputFB<T>::scmEventInputNames;
      using CInputFB<T>::scmEventInputTypeIds;
      using CInputFB<T>::scmEIWith;
      using CInputFB<T>::scmEIWithIndexes;

      using CInputFB<T>::scmEventOutputNames;
      using CInputFB<T>::scmEventOutputTypeIds;
      using CInputFB<T>::scmEOWith;
      using CInputFB<T>::scmEOWithIndexes;

      using CInputFB<T>::scmDataInputNames;
      using CInputFB<T>::scmDataInputTypeIds;
      using CInputFB<T>::scmDataOutputNames;
      using CInputFB<T>::scmDataOutputTypeIds;

    protected:
      using CInputFB<T>::var_IN;

    public:
      CeInputFB(forte::core::CFBContainer &paContainer, const CStringDictionary::TStringId paInstanceNameId) :
          CInputFB<T>(paContainer, scmFBInterfaceSpec, paInstanceNameId),
          var_eCONF(STRID(eCONF), *this, 0) {};

      forte::CPlugPin<FORTE_eGenAdapter_Plug> var_eCONF;

      CeConfigFB *mPeerFB;

      IPlugPin *getPlugPinUnchecked(const size_t paIndex) override {
        switch (paIndex) {
          case 0: return &var_eCONF;
        }
        return nullptr;
      }

      bool eventConfig() {
        if (auto skt = var_eCONF.getAdapterCon().getSocket(); skt != nullptr && skt->getAdapterBlock() != nullptr) {
          mPeerFB = &static_cast<CeConfigFB &>(skt->getAdapterBlock()->getParent());
          mPeerFB->eventGen();
        }
        return true;
      }

      bool onChange() override {
        if (!CInputFB<T>::onChange()) {
          return false;
        }

        if (mPeerFB && mPeerFB->checkEventTriggersConf(&var_IN)) {
          return true;
        }
        return false;
      }

    protected:
      static const SFBInterfaceSpec scmFBInterfaceSpec;

      virtual bool checkEventTriggers() {
        return false;
      };

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) final {
        switch (paEIID) {
          case cgExternalEventID:
            if (this->var_QI) {
              this->sendOutputEvent(this->scmEventINDID, paECET);
            } else {
              this->var_QO = false_BOOL;
            }
            break;
          case CInputFB<T>::scmEventREQID:
            if (this->var_QI) {
              this->var_QO = this->read();
            } else {
              this->var_QO = false_BOOL;
            }
            this->sendOutputEvent(this->scmEventCNFID, paECET);
            break;
          default:
            CProcessInterfaceFB::executeEvent(paEIID, paECET);
            eventConfig();
            break;
        }
      }
  };

  template<class T>
  const SAdapterInstanceDef CeInputFB<T>::scmAdapterInstances[] = {{STRID(eGenAdapter), STRID(eCONF), true}};

  template<class T>
  const SFBInterfaceSpec CeInputFB<T>::scmFBInterfaceSpec = {2,
                                                             scmEventInputNames,
                                                             scmEventInputTypeIds,
                                                             scmEIWith,
                                                             scmEIWithIndexes,
                                                             3,
                                                             scmEventOutputNames,
                                                             scmEventOutputTypeIds,
                                                             scmEOWith,
                                                             scmEOWithIndexes,
                                                             2,
                                                             scmDataInputNames,
                                                             scmDataInputTypeIds,
                                                             3,
                                                             scmDataOutputNames,
                                                             scmDataOutputTypeIds,
                                                             0,
                                                             nullptr,
                                                             1,
                                                             scmAdapterInstances};
} // namespace forte::core::io
