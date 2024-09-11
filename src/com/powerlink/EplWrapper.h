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

    EplMappingValues(unsigned int paDataSize, unsigned int paPiOffset,
              unsigned int paBitOffset) : mDataSize(paDataSize), mPiOffset(paPiOffset),
                            mBitOffset(paBitOffset) {
      mCurrentValue = new char[paDataSize];
      for (unsigned int i = 0; i < paDataSize; i++) {
        mCurrentValue[i] = 0x00;
      }
    }

    ~EplMappingValues() {
      delete[] mCurrentValue;
    }

  private:
    EplMappingValues(const EplMappingValues &paObj);

    EplMappingValues &operator=(const EplMappingValues &paObj);
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

  int eplStackInit(const char *paXmlFile, const char *paCdcFile, const char *paEthDeviceName);

  int eplStackShutdown();

  CProcessImageMatrix *getProcessImageMatrixIn();

  CProcessImageMatrix *getProcessImageMatrixOut();

  char *getProcImageIn();

  char *getProcImageOut();

  void waitUntilOperational(bool paWait);

  void registerCallback(IEplCNCallback *paCallback);

  void executeAllCallbacks();

private:
  char *allocProcImage(unsigned int paNumOfBytes);

  bool findMAC(const char *paUserMAC, char *paChDeviceName);

  bool compareMACs(const char *paMacA, const char *paMacB);

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
