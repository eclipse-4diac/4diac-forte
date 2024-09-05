/*******************************************************************************
 * Copyright (c) 2012 - 2024 AIT, ACIN, fortiss
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _EPLWRAPPER_H_
#define _EPLWRAPPER_H_

#include "ProcessImageMatrix.h"
#include "EplCNCallback.h"
#include <singlet.h>
#include <fortelist.h>
#include <forte_sync.h>

struct EplMapping {
    struct EplMappingValues {
        unsigned int mDataSize;
        unsigned int mPiOffset;
        unsigned int mBitOffset;
        char *mCurrentValue;

        EplMappingValues(unsigned int pa_nDataSize, unsigned int pa_nPiOffset,
                          unsigned int pa_nBitOffset) : mDataSize(pa_nDataSize), mPiOffset(pa_nPiOffset),
                                                        mBitOffset(pa_nBitOffset) {
            mCurrentValue = new char[pa_nDataSize];
            for (unsigned int i = 0; i < pa_nDataSize; i++) {
                mCurrentValue[i] = 0x00;
            }
        }

        ~EplMappingValues() {
            delete[] mCurrentValue;
        }

    private:
        EplMappingValues(const EplMappingValues &obj);

        EplMappingValues &operator=(const EplMappingValues &obj);
    };

    typedef CSinglyLinkedList<EplMappingValues *> TEplMappingList;
    TEplMappingList mCurrentValues;

    ~EplMapping() {
        while (!mCurrentValues.isEmpty()) {
            delete *(TEplMappingList::Iterator) mCurrentValues.begin();
            mCurrentValues.popFront();
        }
    }
};

// CEplStackWrapper implemented as class
// cppcheck-suppress noConstructor
class CEplStackWrapper {
    DECLARE_SINGLETON(CEplStackWrapper)

public:
    /*! \brief Blocking of real-time signals
     *
     *  This must be called in main.cpp before the event execution thread is started
     */
    static void eplMainInit();

    int eplStackInit(const char *pa_pXmlFile, const char *pa_pCdcFile, const char *pa_pEthDeviceName);

    int eplStackShutdown();

    CProcessImageMatrix *getProcessImageMatrixIn();

    CProcessImageMatrix *getProcessImageMatrixOut();

    char *getProcImageIn();

    char *getProcImageOut();

    void waitUntilOperational(bool pa_bWait);

    void registerCallback(IEplCNCallback *pa_pCallback);

    void executeAllCallbacks();

private:
    char *allocProcImage(unsigned int pa_nNumOfBytes);

    bool findMAC(const char *pa_pUserMAC, char *pa_pChDeviceName);

    bool compareMACs(const char *pa_pMacA, const char *pa_pMacB);

    CProcessImageMatrix mProcMatrixIn;
    CProcessImageMatrix mProcMatrixOut;

    unsigned int mProcInSize;
    char *mAppProcessImageIn;
    unsigned int mProcOutSize;
    char *mAppProcessImageOut;

    bool mInitWait;

    CSinglyLinkedList<IEplCNCallback *> mCallbackList;

    CSyncObject mSync;
};

#endif
