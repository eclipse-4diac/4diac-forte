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

struct SEplMapping{
    struct SEplMappingValues{
        unsigned int mDataSize;
        unsigned int mPiOffset;
        unsigned int mBitOffset;
        char* mCurrentValue;

        SEplMappingValues(unsigned int paDataSize, unsigned int paPiOffset, unsigned int paBitOffset) :
            mDataSize(paDataSize), mPiOffset(paPiOffset), mBitOffset(paBitOffset){
          mCurrentValue = new char[paDataSize];
          for(unsigned int i = 0; i < paDataSize; i++){
            mCurrentValue[i] = 0x00;
          }
        }

        ~SEplMappingValues(){
          delete[] mCurrentValue;
        }

      private:
        SEplMappingValues(const SEplMappingValues &obj);
        SEplMappingValues& operator=(const SEplMappingValues &obj);
    };

    typedef CSinglyLinkedList<SEplMappingValues*> TEplMappingList;
    TEplMappingList mCurrentValues;

    ~SEplMapping(){
      while(!mCurrentValues.isEmpty()){
        delete *(TEplMappingList::Iterator) mCurrentValues.begin();
        mCurrentValues.popFront();
      }
    }
};

// CEplStackWrapper implemented as class
// cppcheck-suppress noConstructor
class CEplStackWrapper{
  DECLARE_SINGLETON(CEplStackWrapper)
    ;
  public:
    /*! \brief Blocking of real-time signals
     *
     *  This must be called in main.cpp before the event execution thread is started
     */
    static void eplMainInit();

    int eplStackInit(char* paXmlFile, char* paCdcFile, char* paEthDeviceName);

    int eplStackShutdown();

    CProcessImageMatrix* getProcessImageMatrixIn();
    CProcessImageMatrix* getProcessImageMatrixOut();

    char* getProcImageIn();
    char* getProcImageOut();

    void waitUntilOperational(bool paWait);

    void registerCallback(IEplCNCallback* paCallback);

    void executeAllCallbacks();

  private:
    char* allocProcImage(unsigned int n_bytes);

    bool findMAC(const char* paUserMAC, char* paDevieName);

    bool compareMACs(const char* paMACa, const char* paMACb);

    CProcessImageMatrix mProcMatrixIn;
    CProcessImageMatrix mProcMatrixOut;

    unsigned int mProcInSize;
    char* mAppProcessImageIn_g;
    unsigned int mProcOutSize;
    char* mAppProcessImageOut_g;

    bool mWait;

    CSinglyLinkedList<IEplCNCallback*> mCallbackList;

    CSyncObject mSync;

};

#endif
