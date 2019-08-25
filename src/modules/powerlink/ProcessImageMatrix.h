/*******************************************************************************
 * Copyright (c) 2012 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _PROCIMAGEMATRIX_H_
#define _PROCIMAGEMATRIX_H_

//#include <vector>
#include <fortelist.h>

class CProcessImageMatrix{
  public:
    CProcessImageMatrix();
    ~CProcessImageMatrix();

    void addEntry(unsigned int pa_nCN, unsigned int pa_nModule, unsigned int pa_nIoId, unsigned int pa_nDataSize, unsigned int pa_nPiOffset, unsigned int pa_nBitOffset);

    // getEntry: Returns an array with [dataSize, PIOffset, BitOffset]
    unsigned int* getEntry(unsigned int pa_nCN, unsigned int pa_nModule, unsigned int pa_nIoId);
    unsigned int* getEntry(unsigned int pa_nIndex);

    unsigned int getNrOfEntries() const;

    //ProcessImageMatrix* is owned by caller
    CProcessImageMatrix* getModuleEntries(unsigned int pa_nCN, unsigned int pa_nModule);

    unsigned long getProcessImageSize() const ;

    void clearAll();

  private:
    struct SChannelEntry{
        unsigned int m_nCN;
        unsigned int m_nModuleId;
        unsigned int m_nIOid;
        unsigned int m_nDataSize;
        unsigned int m_nPIOffset;
        unsigned int m_nBitOffset;

        SChannelEntry(unsigned int pa_nCN, unsigned int pa_nModuleId, unsigned int pa_nIOid, unsigned int pa_nDataSize, unsigned int pa_nPIOffset, unsigned int pa_nBitOffset) :
            m_nCN(pa_nCN), m_nModuleId(pa_nModuleId), m_nIOid(pa_nIOid), m_nDataSize(pa_nDataSize), m_nPIOffset(pa_nPIOffset), m_nBitOffset(pa_nBitOffset){
        }
        ;

    };

    typedef CSinglyLinkedList<SChannelEntry*> TChannelList;
    TChannelList m_lMatrix;

    unsigned long m_nBitSize;

    unsigned int m_nNumberOfEntries;
};

#endif
