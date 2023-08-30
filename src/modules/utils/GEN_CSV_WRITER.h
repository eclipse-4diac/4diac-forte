/*******************************************************************************
 * Copyright (c) 2012, 2023 ACIN, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#ifndef _GEN_CSV_WRITER_H_
#define _GEN_CSV_WRITER_H_

#include <genfb.h>
#include <stdio.h>

class GEN_CSV_WRITER : public CGenFunctionBlock<CFunctionBlock> {
    DECLARE_GENERIC_FIRMWARE_FB(GEN_CSV_WRITER)

  private:
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }

    CIEC_STRING &FILE_NAME(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    }

    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }
    ;

    CIEC_STRING &STATUS(){
      return *static_cast<CIEC_STRING*>(getDO(1));
    }
    ;

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;
    static const CStringDictionary::TStringId scmEventInputNames[];

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;
    static const CStringDictionary::TStringId scmEventOutputNames[];

    void executeEvent(TEventID paEIID) override;

    void readInputData(TEventID paEI) override;
    void writeOutputData(TEventID paEO) override;

    bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

  public:
    GEN_CSV_WRITER(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    ~GEN_CSV_WRITER() override;

  private:

    void openCSVFile();
    void closeCSVFile();
    void writeCSVFileLine();

    FILE *mCSVFile;

    CStringDictionary::TStringId *mDataInputNames;
    CStringDictionary::TStringId *mDataInputTypeIds;

    static const CIEC_STRING scmOK;
    static const CIEC_STRING scmFileAlreadyOpened;
    static const CIEC_STRING scmFileNotOpened;

    static const size_t scmWriteBufferSize = 100;

};

#endif //_GEN_CSV_WRITER_H_
