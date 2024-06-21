/*******************************************************************************
 * Copyright (c) 2012 - 2104 AIT, ACIN, fortiss
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
        unsigned int dataSize;
        unsigned int mPiOffset;
        unsigned int mBitOffset;
        char *mCurrentValue;

        EplMappingValues(unsigned int pa_nDataSize, unsigned int pa_nPiOffset,
                          unsigned int pa_nBitOffset) : dataSize(pa_nDataSize), mPiOffset(pa_nPiOffset),
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

    int eplStackInit(const char *xmlFile, const char *cdcFile, const char *ethDeviceName);

    int eplStackShutdown();

    CProcessImageMatrix *getProcessImageMatrixIn();

    CProcessImageMatrix *getProcessImageMatrixOut();

    char *getProcImageIn();

    char *getProcImageOut();

    void waitUntilOperational(bool pa_bWait);

    void registerCallback(IEplCNCallback *pa_pCallback);

    void executeAllCallbacks();

private:
    char *allocProcImage(unsigned int n_bytes);

    bool findMAC(const char *userMAC, char *pa_pchDevieName);

    bool compareMACs(const char *macA, const char *macB);

    CProcessImageMatrix procMatrixIn;
    CProcessImageMatrix procMatrixOut;

    unsigned int mProcInSize;
    char *mAppProcessImageIn;
    unsigned int mPprocOutSize;
    char *mAppProcessImageOut;

    bool mInitWait;

    CSinglyLinkedList<IEplCNCallback *> mCallbackList;

    CSyncObject mSync;
};

#endif
