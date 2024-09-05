/*******************************************************************************
 * Copyright (c) 2012 AIT, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _EPLXMLREADER_H_
#define _EPLXMLREADER_H_

#include "ProcessImageMatrix.h"
#include "ModuleList.h"

class TiXmlNode;

class CEplXmlReader{
  public:
    CEplXmlReader(CProcessImageMatrix* paIn, CProcessImageMatrix* paOut);
    ~CEplXmlReader();

    void setProcessImageIn(CProcessImageMatrix* paIn);
    void setProcessImageOut(CProcessImageMatrix* paOut);

    void readXmlFile(const char* paFileName);

    int getProcessImageIn();

  private:
    CModuleList mModuleListOut;
    CModuleList mModuleListIn;
    CProcessImageMatrix* mProcImageOut;
    CProcessImageMatrix* mProcImageIn;

    void createProcImageOut(TiXmlNode* paProcessImage);
    void createProcImageIn(TiXmlNode* paProcessImage);

    int getModuleNr(const char* paIoId);

};

#endif
