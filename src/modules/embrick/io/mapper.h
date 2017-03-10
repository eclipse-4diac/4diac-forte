/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_IO_MAPPER_H_
#define SRC_MODULES_EMBRICK_IO_MAPPER_H_

#include <utils/singlet.h>
#include <forte_wstring.h>
#include <map>
#include <string>
#include <forte_sync.h>

namespace EmBrick {

class IOHandle;
class IOObserver;

class IOMapper {
DECLARE_SINGLETON (IOMapper)

public:
  enum Direction
    : char {
      UnknownDirection, In, Out, InOut
  };

public:
  bool registerHandle(CIEC_WSTRING const &id, IOHandle* handle);
  void deregisterHandle(IOHandle* handle);

  bool registerObserver(CIEC_WSTRING const &id, IOObserver* observer);
  void deregisterObserver(IOObserver* observer);

private:
  typedef std::map<std::string, IOHandle*> THandleMap;
  THandleMap handles;

  typedef std::map<std::string, IOObserver*> TObserverMap;
  TObserverMap observers;

  CSyncObject syncMutex;
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_IO_MAPPER_H_ */
