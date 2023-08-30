/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "opcprocessvar.h"
#include "OPCGroup.h"
#include "OPCItem.h"

COpcProcessVar::COpcProcessVar(const char* paItemGroupName, const char* paItemName, EOpcProcessVarFunctions paFunction) :
  mOpcItem(0), mItemGroupName(paItemGroupName), mItemName(paItemName), mActive(false), mFunction(paFunction){
  mCurrentValue.set<SHORT>(0);
}

void COpcProcessVar::sendItemData(){
  try{
    if(getIsActive()) {
      mOpcItem->writeSync(updateValue());
    }
  } catch (OPCException &e){
    setIsActive(false);
  }
}

void COpcProcessVar::setNewValue(Variant paNewValue){
  mSync.lock();
  mNewValueQueue.pushBack(paNewValue);
  mSync.unlock();
}

Variant COpcProcessVar::peekNewValue(){
  Variant retVal;

  mSync.lock();
  TVariantList::Iterator itBegin = mNewValueQueue.begin();
  if(itBegin != mNewValueQueue.end()){
    retVal = (*itBegin);
  } else {
    retVal = mCurrentValue;
  }

  mSync.unlock();

  return retVal;
}

Variant COpcProcessVar::updateValue(){
  Variant retVal;

  mSync.lock();
  TVariantList::Iterator itBegin = mNewValueQueue.begin();
  TVariantList::Iterator itEnd = mNewValueQueue.end();
  if(itBegin != mNewValueQueue.end()){
    mCurrentValue = retVal = (*itBegin);
    mNewValueQueue.popFront();
  } else {
    retVal = mCurrentValue;
  }

  mSync.unlock();

  return retVal;
}
