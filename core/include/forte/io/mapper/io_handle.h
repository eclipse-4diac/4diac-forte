/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"
#include "forte/io/mapper/io_mapper.h"

namespace forte::io {

  class IODeviceController;

  class IOHandle {
      friend class IOMapper;

    public:
      IOHandle(IODeviceController &paController, IOMapper::Direction paDirection, CIEC_ANY::EDataTypeID paType);
      virtual ~IOHandle() = default;

      bool isBound() const {
        return mMapping != nullptr;
      }

      bool hasObserver() const;

      std::string_view getId() const {
        return mMapping ? std::string_view(mMapping->mId) : std::string_view{};
      }

      CIEC_ANY::EDataTypeID getIOHandleDataType() const {
        return mType;
      }

      IOMapper::Direction getDirection() const {
        return mDirection;
      }

      bool isInput() const {
        return mDirection == IOMapper::In;
      }

      bool isOutput() const {
        return mDirection == IOMapper::Out;
      }

      virtual void set(const CIEC_ANY &) = 0;
      virtual void get(CIEC_ANY &) = 0;

      void onChange();

    protected:
      virtual void onNewObserver(IOObserver &paObserver);

      IODeviceController &mController;
      CIEC_ANY::EDataTypeID mType;
      IOMapper::Direction mDirection;

    private:
      void bind(IOMappingData *paMapping);

      const IOMappingData *mMapping = nullptr;
  };

} // namespace forte::io
