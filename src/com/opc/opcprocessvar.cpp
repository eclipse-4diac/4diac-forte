/*******************************************************************************
 * Copyright (c) 2012, 2022 AIT, HIT robot group
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Tibalt Zhao - use stl deque and polish the logs
 *******************************************************************************/
#include "opcprocessvar.h"
#include "OPCGroup.h"
#include "OPCItem.h"
#include "../../arch/devlog.h"
#include <criticalregion.h>

COpcProcessVar::COpcProcessVar(const char* paItemGroupName, const char* paItemName, EOpcProcessVarFunctions paFunction) :
  mItemGroupName(paItemGroupName), mItemName(paItemName), mActive(false), mFunction(paFunction){
  mCurrentValue.set<SHORT>(0);
}

void COpcProcessVar::setNewValue(Variant paNewValue){
  CCriticalRegion criticalRegion(mSync);  
  try{
    paNewValue.get<bool>();
  }catch(...){
    return;
  }
  mNewValueQueue.push_back(paNewValue);
}

Variant COpcProcessVar::peekNewValue(){
  Variant retVal;
  CCriticalRegion criticalRegion(mSync);  
  TVariantList::iterator itBegin = mNewValueQueue.begin();
  if(itBegin != mNewValueQueue.end()){
    retVal = (*itBegin);
  } else {
    retVal = mCurrentValue;
  }
  return retVal;
}

Variant COpcProcessVar::updateValue(){
  Variant retVal;
  CCriticalRegion criticalRegion(mSync);  
  TVariantList::iterator itBegin = mNewValueQueue.begin();
  if(itBegin != mNewValueQueue.end()){
    mCurrentValue = retVal = (*itBegin);
    mNewValueQueue.pop_front();
  } else {
    retVal = mCurrentValue;
  }
  return retVal;
}
