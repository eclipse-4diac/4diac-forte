/*******************************************************************************
 * Copyright (c) 2012 AIT, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef OPCPROCESSVAR_H_
#define OPCPROCESSVAR_H_

#include "windows.h"
#include "Variant.h"
#include "fortelist.h"
#include <forte_sync.h>

class COPCItem;

class COpcProcessVar{
  public:
    enum EOpcProcessVarFunctions{
      e_FBInput, e_FBOutput
    };

    COpcProcessVar(const char* paItemGroupName, const char* paItemName, EOpcProcessVarFunctions paFunction);
    ~COpcProcessVar(){
    }

    const char* getItemName() const {
      return mItemName;
    }
    const char* getItemGroupName() const {
      return mItemGroupName;
    }
    EOpcProcessVarFunctions getItemFunction() const {
      return mFunction;
    }

    void setOpcItem(COPCItem* paOpcItem){
      mOpcItem = paOpcItem;
    }

    void sendItemData();

    void setNewValue(Variant paNewValue);
    Variant peekNewValue();

    Variant updateValue();

    bool getIsActive() const {
      return mActive;
    }

    void setIsActive(bool paActive){
      mActive = paActive;
    }

  private:
    COPCItem* mOpcItem;

    const char* mItemGroupName;
    const char* mItemName;
    bool mActive;

    Variant mCurrentValue;

    typedef CSinglyLinkedList<Variant> TVariantList;
    TVariantList mNewValueQueue;

    Variant mNewValue;

    CSyncObject mSync;

    EOpcProcessVarFunctions mFunction;

};

#endif // OPCPROCESSVAR_H_
