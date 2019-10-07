/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
 
#ifndef _FILEWRITER_H_
#define _FILEWRITER_H_

#include <funcbloc.h>
#include <fstream>

class fileWriter: public CFunctionBlock{
    DECLARE_FIRMWARE_FB(fileWriter)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL*>(getDI(0));
    };

    CIEC_STRING &FILE_NAME() {
      return *static_cast<CIEC_STRING*>(getDI(1));
    };

    CIEC_STRING &S1() {
      return *static_cast<CIEC_STRING*>(getDI(2));
    };

    CIEC_BOOL &APPEND() {
      return *static_cast<CIEC_BOOL*>(getDI(3));
    };

    CIEC_STRING &APPCHAR() {
      return *static_cast<CIEC_STRING*>(getDI(4));
    };

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL*>(getDO(0));
    };

    CIEC_WSTRING &STATUS() {
      return *static_cast<CIEC_WSTRING*>(getDO(1));
    };

    static const TEventID scm_nEventINITID = 0;
    static const TEventID scm_nEventREQID = 1;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TEventID scm_nEventCNFID = 1;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    static const char * const scmOK;
    static const char * const scmNotInitialised;
    static const char * const scmCouldNotWrite;

    void executeEvent(int pa_nEIID);

  public:
    fileWriter(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
    virtual ~fileWriter(){};

  private:
    bool openFile();
    void closeFile();
    bool writeFile();

    std::fstream mFile;

    FORTE_FB_DATA_ARRAY(2, 5, 2, 0);
};

#endif //close the ifdef sequence from the beginning of the file
