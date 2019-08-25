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

COpcProcessVar::COpcProcessVar(const char* pa_acItemGroupName, const char* pa_acItemName, EOpcProcessVarFunctions pa_eFunction) :
  m_pOpcItem(0), m_acItemGroupName(pa_acItemGroupName), m_acItemName(pa_acItemName), m_bActive(false), m_eFunction(pa_eFunction){
  m_oCurrentValue.set<SHORT>(0);
}

void COpcProcessVar::sendItemData(){
  try{
    if(getIsActive()) {
      m_pOpcItem->writeSync(updateValue());
    }
  } catch (OPCException &e){
    setIsActive(false);
  }
}

void COpcProcessVar::setNewValue(Variant pa_oNewValue){
  m_oSync.lock();
  m_lNewValueQueue.pushBack(pa_oNewValue);
  m_oSync.unlock();
}

Variant COpcProcessVar::peekNewValue(){
  Variant retVal;

  m_oSync.lock();
  TVariantList::Iterator itBegin = m_lNewValueQueue.begin();
  if(itBegin != m_lNewValueQueue.end()){
    retVal = (*itBegin);
  } else {
    retVal = m_oCurrentValue;
  }

  m_oSync.unlock();

  return retVal;
}

Variant COpcProcessVar::updateValue(){
  Variant retVal;

  m_oSync.lock();
  TVariantList::Iterator itBegin = m_lNewValueQueue.begin();
  TVariantList::Iterator itEnd = m_lNewValueQueue.end();
  if(itBegin != m_lNewValueQueue.end()){
    m_oCurrentValue = retVal = (*itBegin);
    m_lNewValueQueue.popFront();
  } else {
    retVal = m_oCurrentValue;
  }

  m_oSync.unlock();

  return retVal;
}
