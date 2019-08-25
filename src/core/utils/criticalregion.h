/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef CRITICALREGION_H_
#define CRITICALREGION_H_

#include <forte_sync.h>

class CCriticalRegion{
  public:
    explicit CCriticalRegion(CSyncObject &paSyncObject) : mSyncObject(paSyncObject){
      mSyncObject.lock();
    }

    ~CCriticalRegion(){
      mSyncObject.unlock();
    }

  private:
    CSyncObject &mSyncObject;

};



#endif /* CRITICALREGION_H_ */
