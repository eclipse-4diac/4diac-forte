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
#ifndef _MODULELIST_H_
#define _MODULELIST_H_

#include <fortelist.h>

class CModuleList{
  public:
    CModuleList();
    ~CModuleList();

    void addEntry(const char* pa_pchName, unsigned int pa_nPiOffset, unsigned int pa_nBitOffset, unsigned int pa_nModuleNr);

    int getModuleNr(const char* pa_pchName, unsigned int pa_nPiOffset, unsigned int pa_nBitOffset);

    int getModuleNr(const char* pa_pchName, unsigned int pa_nOccurence);

    int getNrOfModules(const char* pa_pchName);

    bool moduleNameExist(const char* pa_pchName);

  private:
    class IoModule{
      public:
        IoModule(const char* pa_pchName, unsigned int pa_nPiOffset, unsigned int pa_nBitOffset, unsigned int pa_nModuleNr);
        ~IoModule();

        char* m_pchName;
        unsigned int m_nPiOffset;
        unsigned int m_nBitOffset;

        unsigned int m_nModuleNr;

      private:
        IoModule(const IoModule& obj);
        IoModule& operator=(const IoModule& obj);
    };

    //std::vector<IoModule*> m_lModules;

    typedef CSinglyLinkedList<IoModule*> TModuleList;
    TModuleList m_lModules;

    unsigned int m_nNumberOfModules;
};

#endif
