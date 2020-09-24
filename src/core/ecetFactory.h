/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Martin Melik Merkumians - template based event-chain execution thread factory
 *******************************************************************************/
#pragma once

#include "staticassert.h"
#include "iec61131_cast_helper.h"

class CEventChainExecutionThreadFactory {
  public:
    template<typename T>
    static T* createEventChainExecutionThread() {
      FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CEventChainExecutionThreadAbstract, T>::value),
        PassedTypeNotASubTypeOfEventChainExeCutionThreadAbstract);
      return new T();
    }
};
