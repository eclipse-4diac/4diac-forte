/*******************************************************************************
 * Copyright (c) 2012, 2022 AIT, ACIN, HIT robot group
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Tibalt Zhao - use stl deque and polish the logs
 *   Ketut Kumajaya - Code refactoring from char* to std::string
 *******************************************************************************/
#ifndef OPCPROCESSVAR_H_
#define OPCPROCESSVAR_H_

#include "windows.h"
#include "Variant.h"
#include <deque>
#include "forte_sync.h"

class COpcProcessVar{
  public:
    enum EOpcProcessVarFunctions{
      e_FBInput, e_FBOutput
    };

    COpcProcessVar(const std::string& paItemGroupName, const std::string& paItemName, EOpcProcessVarFunctions paFunction);
    ~COpcProcessVar(){
    }

    const std::string getItemName() const {
      return mItemName;
    }
    const std::string getItemGroupName() const {
      return mItemGroupName;
    }
    EOpcProcessVarFunctions getItemFunction() const {
      return mFunction;
    }

    void setNewValue(Variant paNewValue);
    Variant peekNewValue();
    Variant updateValue();

  private:
    const std::string mItemGroupName;
    const std::string mItemName;
    bool mActive;

    Variant mCurrentValue;

    typedef std::deque<Variant> TVariantList;
    TVariantList mNewValueQueue;

    Variant mNewValue;

    CSyncObject mSync;

    EOpcProcessVarFunctions mFunction;
};

#endif // OPCPROCESSVAR_H_
