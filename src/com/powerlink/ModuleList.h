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

    void addEntry(const char* paName, unsigned int paPiOffset, unsigned int paBitOffset, unsigned int paModuleNr);

    int getModuleNr(const char* paName, unsigned int paPiOffset, unsigned int paBitOffset);

    int getModuleNr(const char* paName, unsigned int paOccurence);

    int getNrOfModules(const char* paName);

    bool moduleNameExist(const char* paName);

  private:
    class IoModule{
      public:
        IoModule(const char* paName, unsigned int paPiOffset, unsigned int paBitOffset, unsigned int paModuleNr);
        ~IoModule();

        char* mName;
        unsigned int mPiOffset;
        unsigned int mBitOffset;

        unsigned int mModuleNr;

      private:
        IoModule(const IoModule& obj);
        IoModule& operator=(const IoModule& obj);
    };

    //std::vector<IoModule*> mModules;

    typedef CSinglyLinkedList<IoModule*> TModuleList;
    TModuleList mModules;

    unsigned int mNumberOfModules;
};

#endif
