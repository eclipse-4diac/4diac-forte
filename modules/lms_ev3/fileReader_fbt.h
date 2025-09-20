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

#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include "forte/funcbloc.h"
#include <fstream>

class fileReader : public CFunctionBlock {
    DECLARE_FIRMWARE_FB(fileReader)

  private:
    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL *>(getDI(0));
    };

    CIEC_STRING &FILE_NAME() {
      return *static_cast<CIEC_STRING *>(getDI(1));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL *>(getDO(0));
    };

    CIEC_WSTRING &STATUS() {
      return *static_cast<CIEC_WSTRING *>(getDO(1));
    };

    CIEC_STRING &S1() {
      return *static_cast<CIEC_STRING *>(getDO(2));
    };

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;

    static const char *const scmOK;
    static const char *const scmNotInitialised;
    static const char *const scmCouldNotRead;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  public:
    fileReader(const forte::StringId paInstanceNameId, CFBContainer &paContainer);
    ~fileReader() override = default;

  private:
    bool openFile();
    void closeFile();

    bool readFile();
    std::fstream mFile;
};

#endif // close the ifdef sequence from the beginning of the file
