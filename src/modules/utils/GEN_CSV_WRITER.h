/*******************************************************************************
 * Copyright (c) 2012 - 2015 ACIN, fortiss GmbH
 *                      2018 Johannes Kepler University
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

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }
    ;

    CIEC_STRING &STATUS(){
      return *static_cast<CIEC_STRING*>(getDO(1));
    }
    ;

    static const TEventID scm_nEventINITID = 0;
    static const TEventID scm_nEventREQID = 1;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TEventID scm_nEventCNFID = 1;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    void executeEvent(int paEIID);
    virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec);

  public:
    GEN_CSV_WRITER(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    virtual ~GEN_CSV_WRITER();

  private:

    void openCSVFile();
    void closeCSVFile();
    void writeCSVFileLine();

    FILE *mCSVFile;

    CStringDictionary::TStringId *m_anDataInputNames;
    CStringDictionary::TStringId *m_anDataInputTypeIds;

    TDataIOID *m_anEIWith;

    static const char * const scmOK;
    static const char * const scmFileAlreadyOpened;
    static const char * const scmFileNotOpened;

    static const size_t scmWriteBufferSize = 100;

};

#endif //_GEN_CSV_WRITER_H_
