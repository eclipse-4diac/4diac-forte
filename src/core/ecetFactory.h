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

class CEventChainExecutionThreadFactory {
  public:
    template<typename T>
    static T* createEventChainExecutionThread() {
      static_assert(std::is_base_of<CEventChainExecutionThreadAbstract, T>::value, "Is not a type of EventChainExecutionThread");
      return new T();
    }
};
