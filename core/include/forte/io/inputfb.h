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
  class CInputFB : public CProcessInterfaceFB {
      static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>, "T must be a subclass of CIEC_ANY_BIT");

    protected:
      static const TEventID scmEventINDID = 2;

    public:
      CInputFB(CFBContainer &paContainer, const StringId paInstanceNameId) :
          CProcessInterfaceFB(paContainer, scmFBInterfaceSpec, paInstanceNameId),
          var_IN(),
          conn_IND(*this, 2),
          conn_IN(*this, 2, var_IN) {
      }

      CInputFB(CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, const StringId paInstanceNameId) :
          CProcessInterfaceFB(paContainer, paInterfaceSpec, paInstanceNameId),
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
        return CDataTypeTrait<T>::scmDataTypeId;
      }

      IOMapper::Direction getDirection() final override {
        return IOMapper::In;
      }

      bool onChange() override {
        return read();
      }

      void evt_REQ(const CIEC_BOOL &paQI,
                   const CIEC_STRING &paPARAMS,
                   CAnyBitOutputParameter<CIEC_BOOL> paQO,
                   COutputParameter<CIEC_STRING> paSTATUS,
                   CAnyBitOutputParameter<T> &paIN) {
        COutputGuard guard_paQO(paQO);
        COutputGuard guard_paSTATUS(paSTATUS);
        COutputGuard guard_paIN(paIN);
        var_QI = paQI;
        var_PARAMS = paPARAMS;
        receiveInputEvent(scmEventREQID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
        *paIN = var_IN;
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

      static const std::array<const StringId, 2> scmEventInputNames;
      static const std::array<const StringId, 2> scmEventInputTypeIds;
      static const std::array<const StringId, 3> scmEventOutputNames;
      static const std::array<const StringId, 3> scmEventOutputTypeIds;
      static const std::array<const StringId, 2> scmDataInputNames;
      static const std::array<const StringId, 3> scmDataOutputNames;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override {
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
            writeData(scmFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
            writeData(scmFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
            writeData(scmFBInterfaceSpec.getNumDIs() + 2, var_IN, conn_IN);
            break;
          }
          case scmEventINDID: {
            writeData(scmFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
            writeData(scmFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
            writeData(scmFBInterfaceSpec.getNumDIs() + 2, var_IN, conn_IN);
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
  const std::array<const StringId, 2> CInputFB<T>::scmEventInputNames = {"INIT"_STRID, "REQ"_STRID};
  template<class T>
  const std::array<const StringId, 2> CInputFB<T>::scmEventInputTypeIds = {"EInit"_STRID, "Event"_STRID};
  template<class T>
  const std::array<const StringId, 3> CInputFB<T>::scmEventOutputNames = {"INITO"_STRID, "CNF"_STRID, "IND"_STRID};
  template<class T>
  const std::array<const StringId, 3> CInputFB<T>::scmEventOutputTypeIds = {"EInit"_STRID, "Event"_STRID,
                                                                            "Event"_STRID};
  template<class T>
  const std::array<const StringId, 2> CInputFB<T>::scmDataInputNames = {"QI"_STRID, "PARAMS"_STRID};
  template<class T>
  const std::array<const StringId, 3> CInputFB<T>::scmDataOutputNames = {"QO"_STRID, "STATUS"_STRID, "IN"_STRID};
  template<class T>
  const SFBInterfaceSpec CInputFB<T>::scmFBInterfaceSpec = {
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
