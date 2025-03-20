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

USE_STRING_ID(BOOL);
USE_STRING_ID(BYTE);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(OUT);
USE_STRING_ID(PARAMS);
USE_STRING_ID(QB);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);

namespace forte::core::io {

  template<class T>
  class COutputFB : public forte::core::io::CProcessInterfaceFB {
      static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>, "T must be a subclass of CIEC_ANY_BIT");

    public:
      COutputFB(forte::core::CFBContainer &paContainer, const CStringDictionary::TStringId paInstanceNameId) :
          CProcessInterfaceFB(paContainer, scmFBInterfaceSpec, paInstanceNameId),
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

      void evt_REQ(const CIEC_BOOL &paQI, const CIEC_STRING &paPARAMS, const CIEC_BYTE &paOUT, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS) {
        var_QI = paQI;
        var_PARAMS = paPARAMS;
        var_OUT = paOUT;
        receiveInputEvent(scmEventREQID, nullptr);
        paQO = var_QO;
        paSTATUS = var_STATUS;
      }

    protected:
      static const SFBInterfaceSpec scmFBInterfaceSpec;

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
      static const CStringDictionary::TStringId scmDataInputNames[];
      static const CStringDictionary::TStringId scmDataInputTypeIds[];
      static const CStringDictionary::TStringId scmDataOutputNames[];
      static const CStringDictionary::TStringId scmDataOutputTypeIds[];
      static const TDataIOID scmEIWith[];
      static const TForteInt16 scmEIWithIndexes[];
      static const CStringDictionary::TStringId scmEventInputNames[];
      static const CStringDictionary::TStringId scmEventInputTypeIds[];
      static const TDataIOID scmEOWith[];
      static const TForteInt16 scmEOWithIndexes[];
      static const CStringDictionary::TStringId scmEventOutputNames[];
      static const CStringDictionary::TStringId scmEventOutputTypeIds[];

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

  template<class T>
  const CStringDictionary::TStringId COutputFB<T>::scmDataInputNames[] = {STRID(QI), STRID(PARAMS), STRID(OUT)};
  template<class T>
  const CStringDictionary::TStringId COutputFB<T>::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING), forte::CDataTypeTrait<T>::scmDataTypeName};
  template<class T>
  const CStringDictionary::TStringId COutputFB<T>::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
  template<class T>
  const CStringDictionary::TStringId COutputFB<T>::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(STRING)};
  template<class T>
  const TDataIOID COutputFB<T>::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, 2, scmWithListDelimiter};
  template<class T>
  const TForteInt16 COutputFB<T>::scmEIWithIndexes[] = {0, 3};
  template<class T>
  const CStringDictionary::TStringId COutputFB<T>::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
  template<class T>
  const CStringDictionary::TStringId COutputFB<T>::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
  template<class T>
  const TDataIOID COutputFB<T>::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
  template<class T>
  const TForteInt16 COutputFB<T>::scmEOWithIndexes[] = {0, 3};
  template<class T>
  const CStringDictionary::TStringId COutputFB<T>::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
  template<class T>
  const CStringDictionary::TStringId COutputFB<T>::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event)};
  template<class T>
  const SFBInterfaceSpec COutputFB<T>::scmFBInterfaceSpec = {
    2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
    2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
    3, scmDataInputNames, scmDataInputTypeIds,
    2, scmDataOutputNames, scmDataOutputTypeIds,
    0, nullptr,
    0, nullptr
  };

}
