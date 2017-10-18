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

#ifndef SRC_CORE_IO_MAPPER_MAPPER_H_
#define SRC_CORE_IO_MAPPER_MAPPER_H_

#include <utils/singlet.h>
#include <forte_wstring.h>
#include <map>
#include <string>
#include <forte_sync.h>

namespace IO {

class Handle;
class Observer;

class Mapper {
  DECLARE_SINGLETON (Mapper)

public:
  enum Direction {
    UnknownDirection, In, Out, InOut
  };

public:
  bool registerHandle(CIEC_WSTRING const &id, Handle* handle);
  void deregisterHandle(Handle* handle);

  bool registerObserver(CIEC_WSTRING const &id, Observer* observer);
  void deregisterObserver(Observer* observer);

private:
  typedef std::map<std::string, Handle*> THandleMap;
  THandleMap handles;

  typedef std::map<std::string, Observer*> TObserverMap;
  TObserverMap observers;

  CSyncObject syncMutex;
};

} /* namespace IO */

#endif /* SRC_CORE_IO_MAPPER_MAPPER_H_ */
