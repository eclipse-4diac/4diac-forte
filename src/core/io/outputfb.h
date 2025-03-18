/*************************************************************************
 * Copyright (c) 2025 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - adds initial implementation
 *************************************************************************/

#pragma once

#include "processinterfacefb.h"

namespace forte::core::io {

  template<class T>
  class COutputFB : public forte::core::io::CProcessInterfaceFB {
      static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>, "T must be a subclass of CIEC_ANY_BIT");

    public:
      COutputFB(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
          CProcessInterfaceFB(paContainer, paInterfaceSpec, paInstanceNameId),
          var_OUT(),
          conn_OUT(nullptr) {
      }

      T var_OUT;
      CDataConnection *conn_OUT;

      CIEC_ANY *getDI(size_t paIndex) override final {
        if(paIndex == 2) {
          return &var_OUT;
        }
        return CProcessInterfaceFB::getDI(paIndex);
      }

      CIEC_ANY::EDataTypeID getIOObserverDataType() override {
        return forte::CDataTypeTrait<T>::scmDataTypeId;
      }

      IOMapper::Direction getDirection() final override {
        return IOMapper::Out;
      }

    protected:
      void onHandle(IOHandle *const paHandle) final override {
        CProcessInterfaceFB::onHandle(paHandle);
        if(isReady()) {
          var_QO = write();
        }
      }

      CDataConnection **getDIConUnchecked(TPortId paIndex) override final {
        if(paIndex == 2) {
          return &conn_OUT;
        }
        return CProcessInterfaceFB::getDIConUnchecked(paIndex);
      }

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) final override {
        if(paEIID == scmEventREQID) {
          if(var_QI) {
            var_QO = write();
          } else {
            var_QO = false_BOOL;
          }
          sendOutputEvent(scmEventCNFID, paECET);
        } else {
          CProcessInterfaceFB::executeEvent(paEIID, paECET);
        }
      }

      void readInputData(TEventID paEIID) final override {
        switch(paEIID){
          case scmEventREQID: {
            readData(0, var_QI, conn_QI);
            readData(2, var_OUT, conn_OUT);
            break;
          }
          default:
            CProcessInterfaceFB::readInputData(paEIID);
            break;
        }
      }

      void setInitialValues() final override {
        CProcessInterfaceFB::setInitialValues();
        var_OUT = T(0);
      }

      CIEC_BOOL write() {
        auto curHandle = getHandle();
        if(!isReady() || curHandle == nullptr) {
          return false_BOOL;
        }

        curHandle->set(var_OUT);

        return true_BOOL;
      }

  };

}
