/*******************************************************************************
 * Copyright (c) 2012, 2024 ACIN, fortiss GmbH,  Johannes Kepler University Linz,
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger, Martin Jobst - copy and modified from
 *                                              GEN_CSV_WRITER
 *******************************************************************************/
#pragma once

#include "genfb.h"
#include <stdio.h>
#include "../../arch/forte_fileio.h"
#include <vector>
#include <fstream>
#include <iostream>

class GEN_CSV_READER : public CGenFunctionBlock<CFunctionBlock> {
    DECLARE_GENERIC_FIRMWARE_FB(GEN_CSV_READER)

  protected:
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }

    CIEC_STRING &FILE_NAME(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    }

    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];

    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }

    CIEC_STRING &STATUS(){
      return *static_cast<CIEC_STRING*>(getDO(1));
    }

    TPortId getNumRD() const {
      return getFBInterfaceSpec().mNumDOs - 2;
    }

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;
    static const CStringDictionary::TStringId scmEventInputNames[];

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;
    static const CStringDictionary::TStringId scmEventOutputNames[];

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEI) override;
    void writeOutputData(TEventID paEO) override;

    bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

  public:
    GEN_CSV_READER(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    ~GEN_CSV_READER() override;

  private:

    void openCSVFile();
    void closeCSVFile();
    void readCSVFileLine();

    std::ifstream mCSVFile;

    CStringDictionary::TStringId *mDataOutputNames;
    CStringDictionary::TStringId *mDataOutputTypeIds;

    static const CIEC_STRING scmOK;
    static const CIEC_STRING scmFileAlreadyOpened;
    static const CIEC_STRING scmFileNotOpened;

    std::vector<char> writeBuffer;

};
