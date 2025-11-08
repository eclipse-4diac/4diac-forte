/*******************************************************************************
 * Copyright (c) 2016, 2022 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                          Jonathan Lainer
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Jonathan Lainer - Add method for deregistering Handles by ID.
 *******************************************************************************/

#ifndef SRC_CORE_IO_MAPPER_MAPPER_H_
#define SRC_CORE_IO_MAPPER_MAPPER_H_

#include "forte/util/singlet.h"
#include "forte/datatypes/forte_wstring.h"
#include <map>
#include <string>
#include "forte/arch/forte_sync.h"

namespace forte::io {

  class IOHandle;
  class IOObserver;

  class IOMapper {
      DECLARE_SINGLETON(IOMapper)

    public:
      enum Direction { UnknownDirection, In, Out, InOut };

      bool registerHandle(std::string const &paId, IOHandle &paHandle);
      void deregisterHandle(IOHandle &paHandle);
      void deregisterHandle(std::string const &paId);

      /*! @brief Get the handle instance with the given ID.
       *
       * @param paId The ID of the IOHandle.
       * @return Pointer to the IOHandle instance, or nullptr if not found.
       */
      IOHandle *getHandle(std::string const &paId);

      bool registerObserver(std::string const &paId, IOObserver *paObserver);
      void deregisterObserver(IOObserver *paObserver);

    private:
      typedef std::map<std::string, IOHandle *> THandleMap;
      THandleMap mHandles;

      typedef std::map<std::string, IOObserver *> TObserverMap;
      TObserverMap mObservers;

      arch::CSyncObject mSyncMutex;
  };

} // namespace forte::io

#endif /* SRC_CORE_IO_MAPPER_MAPPER_H_ */
