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

#include "core/io/processinterfacefb.h"

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
        if (paIndex == 2) {
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

      void evt_REQ(const CIEC_BOOL &paQI,
                   const CIEC_STRING &paPARAMS,
                   const CIEC_BYTE &paOUT,
                   CIEC_BOOL &paQO,
                   CIEC_STRING &paSTATUS) {
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
        if (isReady()) {
          var_QO = write();
        }
      }

      CDataConnection **getDIConUnchecked(TPortId paIndex) override final {
        if (paIndex == 2) {
          return &conn_OUT;
        }
        return CProcessInterfaceFB::getDIConUnchecked(paIndex);
      }

    private:
      static const std::array<const CStringDictionary::TStringId, 2> scmEventInputNames;
      static const std::array<const CStringDictionary::TStringId, 2> scmEventInputTypeIds;
      static const std::array<const CStringDictionary::TStringId, 2> scmEventOutputNames;
      static const std::array<const CStringDictionary::TStringId, 2> scmEventOutputTypeIds;
      static const std::array<const CStringDictionary::TStringId, 3> scmDataInputNames;
      static const std::array<const CStringDictionary::TStringId, 2> scmDataOutputNames;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) final override {
        if (paEIID == scmEventREQID) {
          if (var_QI) {
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
        switch (paEIID) {
          case scmEventREQID: {
            readData(0, var_QI, conn_QI);
            readData(2, var_OUT, conn_OUT);
            break;
          }
          default: CProcessInterfaceFB::readInputData(paEIID); break;
        }
      }

      void setInitialValues() final override {
        CProcessInterfaceFB::setInitialValues();
        var_OUT = T(0);
      }

      CIEC_BOOL write() {
        auto curHandle = getHandle();
        if (!isReady() || curHandle == nullptr) {
          return false_BOOL;
        }

        curHandle->set(var_OUT);

        return true_BOOL;
      }
  };

  template<class T>
  const std::array<const CStringDictionary::TStringId, 2> COutputFB<T>::scmEventInputNames = {STRID(INIT), STRID(REQ)};
  template<class T>
  const std::array<const CStringDictionary::TStringId, 2> COutputFB<T>::scmEventInputTypeIds = {STRID(EInit),
                                                                                                STRID(Event)};
  template<class T>
  const std::array<const CStringDictionary::TStringId, 2> COutputFB<T>::scmEventOutputNames = {STRID(INITO),
                                                                                               STRID(CNF)};
  template<class T>
  const std::array<const CStringDictionary::TStringId, 2> COutputFB<T>::scmEventOutputTypeIds = {STRID(EInit),
                                                                                                 STRID(Event)};
  template<class T>
  const std::array<const CStringDictionary::TStringId, 3> COutputFB<T>::scmDataInputNames = {STRID(QI), STRID(PARAMS),
                                                                                             STRID(OUT)};
  template<class T>
  const std::array<const CStringDictionary::TStringId, 2> COutputFB<T>::scmDataOutputNames = {STRID(QO), STRID(STATUS)};
  template<class T>
  const SFBInterfaceSpec COutputFB<T>::scmFBInterfaceSpec = {
      .mEINames = scmEventInputNames,
      .mEITypeNames = scmEventInputTypeIds,
      .mEONames = scmEventOutputNames,
      .mEOTypeNames = scmEventOutputTypeIds,
      .mDINames = scmDataInputNames,
      .mDONames = scmDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };

} // namespace forte::core::io
