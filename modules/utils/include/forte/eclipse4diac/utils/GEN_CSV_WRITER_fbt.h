/*******************************************************************************
 * Copyright (c) 2012, 2025 ACIN, fortiss GmbH, Johannes Kepler University
 *                          Martin Erich Jobst, Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger
 *                - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - introduced new CGenFB namespace forte::eclipse4diac::utils {
class for better handling generic FBs
 *   Martin Jobst - add generic readInputData and writeOutputData
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#pragma once

#include "forte/genfb.h"
#include <stdio.h>
#include "forte/arch/forte_fileio.h"
#include "forte/datatypes/forte_any_variant.h"

#include <memory>

namespace forte::eclipse4diac::utils {
  class GEN_CSV_WRITER final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_CSV_WRITER)

    protected:
      CIEC_BOOL var_QI;
      CIEC_STRING var_FILE_NAME;

      CIEC_BOOL var_QO;
      CIEC_STRING var_STATUS;

      CEventConnection conn_INITO;
      CEventConnection conn_CNF;

      CDataConnection *conn_QI;
      CDataConnection *conn_FILE_NAME;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_STRING> conn_STATUS;

      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;

      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      void createGenInputData() override;

      size_t getGenEOOffset() override {
        return 1;
      }

      size_t getGenDIOffset() override {
        return 2;
      }

      size_t getGenDOOffset() override {
        return 2;
      }

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId paDINum) override;
      CDataConnection *getDOConUnchecked(TPortId paDONum) override;

    public:
      GEN_CSV_WRITER(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_CSV_WRITER() override;

    private:
      void openCSVFile();
      void closeCSVFile();
      void writeCSVFileLine();
      bool areDIsSameArrayLength(size_t &commonArraySize);

      decltype(forte_fopen(nullptr, nullptr)) mCSVFile;

      std::vector<StringId> mDataInputNames;

      std::unique_ptr<CIEC_ANY_VARIANT[]> mGenDIs;

      std::string mDataOutPutBuffer;

      static const CIEC_STRING scmOK;
      static const CIEC_STRING scmFileAlreadyOpened;
      static const CIEC_STRING scmFileNotOpened;
  };
} // namespace forte::eclipse4diac::utils
