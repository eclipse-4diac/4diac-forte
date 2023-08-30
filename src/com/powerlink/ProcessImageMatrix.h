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

    void addEntry(unsigned int paCN, unsigned int paModule, unsigned int paIoId, unsigned int paDataSize, unsigned int paPiOffset, unsigned int paBitOffset);

    // getEntry: Returns an array with [dataSize, PIOffset, BitOffset]
    unsigned int* getEntry(unsigned int paCN, unsigned int paModule, unsigned int paIoId);
    unsigned int* getEntry(unsigned int paIndex);

    unsigned int getNrOfEntries() const;

    //ProcessImageMatrix* is owned by caller
    CProcessImageMatrix* getModuleEntries(unsigned int paCN, unsigned int paModule);

    unsigned long getProcessImageSize() const ;

    void clearAll();

  private:
    struct SChannelEntry{
        unsigned int mCN;
        unsigned int mModuleId;
        unsigned int mIOid;
        unsigned int mDataSize;
        unsigned int mPIOffset;
        unsigned int mBitOffset;

        SChannelEntry(unsigned int paCN, unsigned int paModuleId, unsigned int paIOid, unsigned int paDataSize, unsigned int paPIOffset, unsigned int paBitOffset) :
            mCN(paCN), mModuleId(paModuleId), mIOid(paIOid), mDataSize(paDataSize), mPIOffset(paPIOffset), mBitOffset(paBitOffset){
        }
        ;

    };

    typedef CSinglyLinkedList<SChannelEntry*> TChannelList;
    TChannelList mMatrix;

    unsigned long mBitSize;

    unsigned int mNumberOfEntries;
};

#endif
