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
    CEplXmlReader(CProcessImageMatrix* pa_pIn, CProcessImageMatrix* pa_pOut);
    ~CEplXmlReader();

    void setProcessImageIn(CProcessImageMatrix* pa_pIn);
    void setProcessImageOut(CProcessImageMatrix* pa_pOut);

    void readXmlFile(const char* pa_pchFileName);

    int getProcessImageIn();

  private:
    CModuleList m_oModuleListOut;
    CModuleList m_oModuleListIn;
    CProcessImageMatrix* m_pProcImageOut;
    CProcessImageMatrix* m_pProcImageIn;

    void createProcImageOut(TiXmlNode* pa_pProcessImage);
    void createProcImageIn(TiXmlNode* pa_pProcessImage);

    int getModuleNr(const char* pa_pchIoId);

};

#endif
