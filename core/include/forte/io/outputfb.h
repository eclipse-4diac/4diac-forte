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

#include "forte/io/processinterfacefb.h"

using namespace forte::literals;

namespace forte::io {

  template<class T>
  class COutputFB : public CProcessInterfaceFB {
      static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>, "T must be a subclass of CIEC_ANY_BIT");

    public:
      COutputFB(CFBContainer &paContainer, const StringId paInstanceNameId) :
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
        return CDataTypeTrait<T>::scmDataTypeId;
      }

      IOMapper::Direction getDirection() final override {
        return IOMapper::Out;
      }

      void evt_REQ(const CIEC_BOOL &paQI,
                   const CIEC_STRING &paPARAMS,
                   const T &paOUT,
                   CAnyBitOutputParameter<CIEC_BOOL> paQO,
                   COutputParameter<CIEC_STRING> paSTATUS) {
        COutputGuard guard_paQO(paQO);
        COutputGuard guard_paSTATUS(paSTATUS);
        var_QI = paQI;
        var_PARAMS = paPARAMS;
        var_OUT = paOUT;
        receiveInputEvent(scmEventREQID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
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
      static const std::array<const StringId, 2> scmEventInputNames;
      static const std::array<const StringId, 2> scmEventInputTypeIds;
      static const std::array<const StringId, 2> scmEventOutputNames;
      static const std::array<const StringId, 2> scmEventOutputTypeIds;
      static const std::array<const StringId, 3> scmDataInputNames;
      static const std::array<const StringId, 2> scmDataOutputNames;

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
  const std::array<const StringId, 2> COutputFB<T>::scmEventInputNames = {"INIT"_STRID, "REQ"_STRID};
  template<class T>
  const std::array<const StringId, 2> COutputFB<T>::scmEventInputTypeIds = {"EInit"_STRID, "Event"_STRID};
  template<class T>
  const std::array<const StringId, 2> COutputFB<T>::scmEventOutputNames = {"INITO"_STRID, "CNF"_STRID};
  template<class T>
  const std::array<const StringId, 2> COutputFB<T>::scmEventOutputTypeIds = {"EInit"_STRID, "Event"_STRID};
  template<class T>
  const std::array<const StringId, 3> COutputFB<T>::scmDataInputNames = {"QI"_STRID, "PARAMS"_STRID, "OUT"_STRID};
  template<class T>
  const std::array<const StringId, 2> COutputFB<T>::scmDataOutputNames = {"QO"_STRID, "STATUS"_STRID};
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

} // namespace forte::io
