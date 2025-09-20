/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/com/basefb.h"

#include "forte/genfb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_any_variant.h"

#include "forte/arch/forte_sync.h"

namespace forte::com {

  class CommunicationFB : public CGenFunctionBlock<BaseCommunicationFB> {
    public:
      static constexpr TEventID scmEventINITID = 0;
      static constexpr TEventID scmEventINITOID = 0;
      static constexpr TEventID scmEventSentID = cgExternalEventID - 1;
      static constexpr TEventID scmEventReceiveID = cgExternalEventID - 2;

    protected:
      CommunicationFB(CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, StringId paInstanceNameId);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *paECET) override;

      ComResult open(std::string_view paID) override;
      ComResult send(std::span<CIEC_ANY_VARIANT> paData) override;
      ComResult poll() override;
      void close() override;

      ComResult receive(std::span<CIEC_ANY_VARIANT> paData,
                        ComChannel<std::span<CIEC_ANY_VARIANT>> &paChannel) override;
      void sent(ComResult paResult, ComChannel<std::span<CIEC_ANY_VARIANT>> &paChannel) override;
      std::optional<std::span<CIEC_ANY_VARIANT>> getReceiveBuffer() override;
      std::optional<std::span<CIEC_ANY_VARIANT>> getSendBuffer() override;

      void setStatus(ComResult result);

      static std::array<TPortId, 2> parseConfigString(std::string_view paConfigString);
      void configureDIs(TPortId paGenDINum, SFBInterfaceSpec &paInterfaceSpec);
      void configureDOs(TPortId paGenDONum, SFBInterfaceSpec &paInterfaceSpec);

      void createGenInputData() override;
      void createGenOutputData() override;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      size_t getGenDIOffset() override {
        return 2;
      }

      size_t getGenDOOffset() override {
        return 2;
      }

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      CIEC_BOOL var_QI;
      CIEC_WSTRING var_ID;

      CIEC_BOOL var_QO;
      CIEC_WSTRING var_STATUS;

      CEventConnection conn_INITO;

      CDataConnection *conn_QI;
      CDataConnection *conn_ID;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_WSTRING> conn_STATUS;

      std::vector<CIEC_ANY_VARIANT> mGenDIs;
      std::vector<CIEC_ANY_VARIANT> mGenDOs;

    private:
      arch::CSyncObject mFBLock;

      std::vector<StringId> mDINames;
      std::vector<StringId> mDONames;
  };

} // namespace forte::com
