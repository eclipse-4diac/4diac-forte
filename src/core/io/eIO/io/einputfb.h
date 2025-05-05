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

#include "core/io/inputfb.h"
#include "core/io/device/io_controller.h"
#include "eGenAdapter_adp.h"
#include "ESpec.h"
#include "EConfig.h"
#include "eConfigFB.h"

USE_STRING_ID(eCONF);
USE_STRING_ID(eGenAdapter);

namespace forte::core::io {

  template<class T>
  class CeInputFB : public CInputFB<T> {
    protected:
      static const int scmeCONFAdpNum = 0;

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
      CeInputFB(forte::core::CFBContainer &paContainer, const CStringDictionary::TStringId paInstanceNameId)
        : CInputFB<T>(paContainer, scmFBInterfaceSpec, paInstanceNameId),
          var_eCONF(STRID(eCONF), *this, true) {};

      FORTE_eGenAdapter var_eCONF;

      EConfigFB *peerFB;

      CAdapter *getAdapterUnchecked(const size_t paIndex) override {
        switch(paIndex) {
          case 0: return &var_eCONF;
        }
        return nullptr;
      }

      bool eventConfig() {
        CAdapter* peerAdapter = nullptr;
        FORTE_eGenAdapter* eGenAdapter = nullptr;

        if ((peerAdapter = getAdapterUnchecked(scmeCONFAdpNum)->getPeer()) != nullptr){
          eGenAdapter = static_cast<FORTE_eGenAdapter*>(peerAdapter);
          peerFB = static_cast<EConfigFB *>(eGenAdapter->parentFB);
          peerFB->eventGen();
        }
        return true;
      }

    protected:
      static const SFBInterfaceSpec scmFBInterfaceSpec;
      std::forward_list<ESpecBase*> *eventGenList;

      virtual bool checkEventTriggers() {
        return false;
      };

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) final override {
        switch(paEIID){
          case cgExternalEventID:
            if(this->var_QI) {
              this->var_QO = this->read();
              if (this->var_QO && peerFB && peerFB->checkEventTriggersConf(&var_IN)) {
                this->sendOutputEvent(this->scmEventINDID, paECET);
              }
            } else {
              this->var_QO = false_BOOL;
            }
            break;
          case this->scmEventREQID:
            if(this->var_QI) {
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

      bool initialize() override {
        if(!var_eCONF.initialize()) { return false; }
        var_eCONF.setParentFB(this, 0);
        return CFunctionBlock::initialize();
      }
  };

  template<class T>
  const SAdapterInstanceDef CeInputFB<T>::scmAdapterInstances[] = {
    {STRID(eGenAdapter), STRID(eCONF), true}
  };

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
                                                             scmAdapterInstances
  };
}
