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

#include "forte/adapterconn.h"
#include "forte/io/inputfb.h"
#include "forte/io/eIO/eGenAdapter_adp.h"
#include "forte/io/eIO/eConfigFB.h"

using namespace forte::literals;

namespace forte::io {

  template<class T>
  class CeInputFB : public CInputFB<T> {
    private:
      using CInputFB<T>::scmEventInputNames;
      using CInputFB<T>::scmEventInputTypeIds;

      using CInputFB<T>::scmEventOutputNames;
      using CInputFB<T>::scmEventOutputTypeIds;

      using CInputFB<T>::scmDataInputNames;
      using CInputFB<T>::scmDataOutputNames;
      static const std::array<const StringId, 1> scmPlugs;

    protected:
      using CInputFB<T>::var_IN;

    public:
      CeInputFB(CFBContainer &paContainer, const StringId paInstanceNameId) :
          CInputFB<T>(paContainer, scmFBInterfaceSpec, paInstanceNameId),
          var_eCONF("eCONF"_STRID, *this, 0) {};

      CPlugPin<FORTE_eGenAdapter_Plug> var_eCONF;

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
  const std::array<const StringId, 1> CeInputFB<T>::scmPlugs = {"eCONF"_STRID};

  template<class T>
  const SFBInterfaceSpec CeInputFB<T>::scmFBInterfaceSpec = {
      .mEINames = scmEventInputNames,
      .mEITypeNames = scmEventInputTypeIds,
      .mEONames = scmEventOutputNames,
      .mEOTypeNames = scmEventOutputTypeIds,
      .mDINames = scmDataInputNames,
      .mDONames = scmDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = scmPlugs,
  };
} // namespace forte::io
