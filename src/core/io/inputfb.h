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
USE_STRING_ID(IN);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(PARAMS);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);

namespace forte::core::io {

  template<class T>
  class CInputFB : public forte::core::io::CProcessInterfaceFB {
      static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>, "T must be a subclass of CIEC_ANY_BIT");

    private:
      static const TEventID scmEventINDID = 2;

    public:
      CInputFB(forte::core::CFBContainer &paContainer, const CStringDictionary::TStringId paInstanceNameId) :
          CProcessInterfaceFB(paContainer, scmFBInterfaceSpec, paInstanceNameId),
          var_IN(),
          conn_IND(*this, 2),
          conn_IN(*this, 2, var_IN) {
      }

      CIEC_ANY *getDO(size_t paIndex) final override {
        if (paIndex == 2) {
          return &var_IN;
        }
        return CProcessInterfaceFB::getDO(paIndex);
      }

      CEventConnection *getEOConUnchecked(TPortId paIndex) final override {
        if (paIndex == 2) {
          return &conn_IND;
        }
        return CProcessInterfaceFB::getEOConUnchecked(paIndex);
      }

      CDataConnection *getDOConUnchecked(TPortId paIndex) final override {
        if (paIndex == 2) {
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
        return read();
      }

      void evt_REQ(
          const CIEC_BOOL &paQI, const CIEC_STRING &paPARAMS, CIEC_BOOL &paQO, CIEC_STRING &paSTATUS, CIEC_BYTE &paIN) {
        var_QI = paQI;
        var_PARAMS = paPARAMS;
        receiveInputEvent(scmEventREQID, nullptr);
        paQO = var_QO;
        paSTATUS = var_STATUS;
        paIN = var_IN;
      }

      T var_IN;
      CEventConnection conn_IND;
      COutDataConnection<T> conn_IN;

    protected:
      static const SFBInterfaceSpec scmFBInterfaceSpec;

      void onHandle(IOHandle *const paHandle) final override {
        CProcessInterfaceFB::onHandle(paHandle);
        if (isReady()) {
          var_QO = read();
        }
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
        switch (paEIID) {
          case cgExternalEventID: sendOutputEvent(scmEventINDID, paECET); break;
          case scmEventREQID:
            if (var_QI) {
              var_QO = read();
            } else {
              var_QO = false_BOOL;
            }
            sendOutputEvent(scmEventCNFID, paECET);
            break;
          default: CProcessInterfaceFB::executeEvent(paEIID, paECET); break;
        }
      }

      void writeOutputData(TEventID paEIID) final override {
        switch (paEIID) {
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
          default: CProcessInterfaceFB::writeOutputData(paEIID); break;
        }
      }

      void setInitialValues() final override {
        CProcessInterfaceFB::setInitialValues();
        var_IN = T(0);
      }

      CIEC_BOOL read() {
        auto curHandle = getHandle();

        if (!isReady() || curHandle == nullptr) {
          return false_BOOL;
        }

        curHandle->get(var_IN);

        return true_BOOL;
      }
  };

  template<class T>
  const CStringDictionary::TStringId CInputFB<T>::scmDataInputNames[] = {STRID(QI), STRID(PARAMS)};
  template<class T>
  const CStringDictionary::TStringId CInputFB<T>::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING)};
  template<class T>
  const CStringDictionary::TStringId CInputFB<T>::scmDataOutputNames[] = {STRID(QO), STRID(STATUS), STRID(IN)};
  template<class T>
  const CStringDictionary::TStringId CInputFB<T>::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(STRING),
                                                                            forte::CDataTypeTrait<T>::scmDataTypeName};
  template<class T>
  const TDataIOID CInputFB<T>::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, scmWithListDelimiter};
  template<class T>
  const TForteInt16 CInputFB<T>::scmEIWithIndexes[] = {0, 3};
  template<class T>
  const CStringDictionary::TStringId CInputFB<T>::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
  template<class T>
  const CStringDictionary::TStringId CInputFB<T>::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
  template<class T>
  const TDataIOID CInputFB<T>::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter, 0,
                                              1, 2, scmWithListDelimiter};
  template<class T>
  const TForteInt16 CInputFB<T>::scmEOWithIndexes[] = {0, 3, 7};
  template<class T>
  const CStringDictionary::TStringId CInputFB<T>::scmEventOutputNames[] = {STRID(INITO), STRID(CNF), STRID(IND)};
  template<class T>
  const CStringDictionary::TStringId CInputFB<T>::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event), STRID(Event)};
  template<class T>
  const SFBInterfaceSpec CInputFB<T>::scmFBInterfaceSpec = {2,
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
                                                            0,
                                                            nullptr};

} // namespace forte::core::io
