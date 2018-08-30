/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_CORE_IO_MAPPER_MAPPER_H_
#define SRC_CORE_IO_MAPPER_MAPPER_H_

#include <utils/singlet.h>
#include <forte_wstring.h>
#include <map>
#include <string>
#include <forte_sync.h>

namespace forte {
  namespace core {
    namespace io {

      class IOHandle;
      class IOObserver;

      class IOMapper {
        DECLARE_SINGLETON (IOMapper)

        public:
          enum Direction {
            UnknownDirection,
            In,
            Out,
            InOut
          };

          bool registerHandle(CIEC_WSTRING const &paId, IOHandle* paHandle);
          void deregisterHandle(IOHandle* paHandle);

          bool registerObserver(CIEC_WSTRING const &paId, IOObserver* paObserver);
          void deregisterObserver(IOObserver* paObserver);

        private:
          typedef std::map<std::string, IOHandle*> THandleMap;
          THandleMap mHandles;

          typedef std::map<std::string, IOObserver*> TObserverMap;
          TObserverMap mObservers;

          CSyncObject mSyncMutex;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_MAPPER_MAPPER_H_ */
