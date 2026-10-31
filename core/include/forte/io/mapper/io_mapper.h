/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                    Jonathan Lainer, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral      - Cleaning of namespaces
 *   Jonathan Lainer  - Add method for deregistering Handles by ID
 *   Malte Grave, Alois Zoitl - reworked handle and oberserver handling reducing
 *                      memory overhead and allowing for to have several read
 *                      only observers
 *******************************************************************************/

#pragma once

#include "forte/util/singlet.h"
#include <memory>
#include <string_view>
#include <vector>
#include <string>
#include "forte/arch/forte_sync.h"

namespace forte::io {

  class IOHandle;
  class IOObserver;

  struct IOMappingData {
      const std::string mId;
      IOHandle *mHandle = nullptr;
      std::vector<IOObserver *> mObservers;

      explicit IOMappingData(std::string_view paId) : mId(paId) {
      }
  };

  class IOMapper {
      DECLARE_SINGLETON(IOMapper)

    public:
      enum Direction { In, Out };

      bool registerHandle(std::string_view paId, IOHandle &paHandle);
      void deregisterHandle(IOHandle &paHandle);

      bool registerObserver(std::string_view paId, IOObserver &paObserver);
      void deregisterObserver(std::string_view paId, IOObserver &paObserver);

    private:
      using MappingDataStorage = std::vector<std::unique_ptr<IOMappingData>>;

      MappingDataStorage::const_iterator lowerBound(std::string_view paId) const;
      bool isMatch(MappingDataStorage::const_iterator paIt, std::string_view paId) const;
      void purgeIfUnused(MappingDataStorage::const_iterator paIt);

      MappingDataStorage mMappings;
      arch::CSyncObject mSyncMutex;
  };

} // namespace forte::io
