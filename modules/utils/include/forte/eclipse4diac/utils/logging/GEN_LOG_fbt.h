/************************************************************************* 
 *** Copyright (c) 2025 HR Agrartechnik GmbH  
 ***  
 *** This program and the accompanying materials are made  
 *** available under the terms of the Eclipse Public License 2.0  
 *** which is available at https://www.eclipse.org/legal/epl-2.0/  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***  
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: LOG_16
 *** Description: Ring Logger for ANY Datattype.
 *** Version:
 ***     1.0: 2025-10-18/Franz Höpfinger -  - - initial implementation
 *************************************************************************/

#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_any_variant.h"

namespace forte::eclipse4diac::utils::logging {
  class GEN_LOG final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_LOG)


    protected:
       void createGenOutputData() override;


    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      std::unique_ptr<CIEC_ANY_VARIANT[]> mGenDOs;
      std::vector<StringId> mDONames;
      size_t mCurrentIndex = 0;

    public:
      GEN_LOG(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_VARIANT var_IN;

      CEventConnection conn_CNF;

      CDataConnection *conn_IN;


      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::eclipse4diac::utils::logging
