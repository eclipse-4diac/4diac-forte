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
  class CInputFB : public forte::core::io::CProcessInterfaceFB {
      static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>, "T must be a subclass of CIEC_ANY_BIT");

    private:
      static const TEventID scmEventINDID = 2;

    public:
      CInputFB(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
          CProcessInterfaceFB(paContainer, paInterfaceSpec, paInstanceNameId),
          var_IN(),
          var_conn_IN(var_IN),
          conn_IND(this, 2),
          conn_IN(this, 2, &var_conn_IN) {
       }

      CIEC_ANY* getDO(size_t paIndex) final override {
        if(paIndex == 2) {
          return &var_IN;
        }
        return CProcessInterfaceFB::getDO(paIndex);
      }

      CEventConnection* getEOConUnchecked(TPortId paIndex) final override {
        if(paIndex == 2) {
          return &conn_IND;
        }
        return CProcessInterfaceFB::getEOConUnchecked(paIndex);
      }

      CDataConnection* getDOConUnchecked(TPortId paIndex) final override {
        if(paIndex == 2) {
          return &conn_IN;
        }
        return CProcessInterfaceFB::getDOConUnchecked(paIndex);
      }

      CIEC_ANY::EDataTypeID getIOObserverDataType() override {
        return forte::CDataTypeTrait<T>::scmDataTypeId;
      }

      IOMapper::Direction getDirection() final override {
        return IOMapper::In;
      }

      bool onChange() final override {
        return CProcessInterfaceFB::read(var_IN);
      }

      T var_IN;
      T var_conn_IN;
      CEventConnection conn_IND;
      CDataConnection conn_IN;

    protected:
      void onHandle(IOHandle* const paHandle) final override{
        CProcessInterfaceFB::onHandle(paHandle);
        if(isReady()) {
           var_QO = CProcessInterfaceFB::read(var_IN);
        }
      }

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) final override {
        switch(paEIID){
          case cgExternalEventID:
            sendOutputEvent(scmEventINDID, paECET);
            break;
          case scmEventREQID:
            if(var_QI) {
              var_QO = CProcessInterfaceFB::read(var_IN);
            } else {
              var_QO = false_BOOL;
            }
            sendOutputEvent(scmEventCNFID, paECET);
            break;
          default:
            CProcessInterfaceFB::executeEvent(paEIID, paECET);
            break;
        }
      }

      void writeOutputData(TEventID paEIID) final override {
        switch(paEIID) {
           case scmEventCNFID: {
             writeData(0, var_QO, conn_QO);
             writeData(1, var_STATUS, conn_STATUS);
             writeData(2, var_IN, conn_IN);
             break;
           }
           case scmEventINDID: {
             writeData(0, var_QO, conn_QO);
             writeData(1, var_STATUS, conn_STATUS);
             writeData(2, var_IN, conn_IN);
             break;
           }
           default:
             CProcessInterfaceFB::writeOutputData(paEIID);
             break;
         }
      }

      void setInitialValues() final override {
        CProcessInterfaceFB::setInitialValues();
        var_IN = T(0);
      }

  };

}
