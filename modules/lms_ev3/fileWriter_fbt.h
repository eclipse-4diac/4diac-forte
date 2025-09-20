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

#include "forte/funcbloc.h"
#include <fstream>

class fileWriter : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(fileWriter)

  private:
    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL *>(getDI(0));
    };

    CIEC_STRING &FILE_NAME() {
      return *static_cast<CIEC_STRING *>(getDI(1));
    };

    CIEC_STRING &S1() {
      return *static_cast<CIEC_STRING *>(getDI(2));
    };

    CIEC_BOOL &APPEND() {
      return *static_cast<CIEC_BOOL *>(getDI(3));
    };

    CIEC_STRING &APPCHAR() {
      return *static_cast<CIEC_STRING *>(getDI(4));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL *>(getDO(0));
    };

    CIEC_WSTRING &STATUS() {
      return *static_cast<CIEC_WSTRING *>(getDO(1));
    };

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;

    static const char *const scmOK;
    static const char *const scmNotInitialised;
    static const char *const scmCouldNotWrite;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  public:
    fileWriter(const forte::StringId paInstanceNameId, CFBContainer &paContainer);
    ~fileWriter() override = default;

  private:
    bool openFile();
    void closeFile();
    bool writeFile();

    std::fstream mFile;
};

#endif // close the ifdef sequence from the beginning of the file
