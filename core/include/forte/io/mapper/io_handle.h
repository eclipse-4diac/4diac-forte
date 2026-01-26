/*******************************************************************************
 * Copyright (c) 2016 - 2026 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                           AVL List GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Thomas Oellinger - Add support for multiple observers per input.
 *******************************************************************************/

#ifndef SRC_CORE_IO_MAPPER_HANDLE_H_
#define SRC_CORE_IO_MAPPER_HANDLE_H_

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_bool.h"

#include "forte/io/mapper/io_mapper.h"

namespace forte::io {

  class IODeviceController;

  class IOHandle {
      friend class IOMapper;

    public:
      IOHandle(IODeviceController *paController, IOMapper::Direction paDirection, CIEC_ANY::EDataTypeID paType);
      virtual ~IOHandle();

      bool hasObserver() const {
        return !!mObserver;
      }

      IOObserver *getObserver() {
        return mObserver;
      }

      std::vector<IOObserver*>& getObservers() {
        return mObservers;
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
      IODeviceController *mController;
      CIEC_ANY::EDataTypeID mType;

      IOMapper::Direction mDirection;

      virtual void onObserver(IOObserver *paObserver);
      virtual void dropObserver();
      virtual void dropObserver(IOObserver* paObserver);

    private:
      std::vector<IOObserver*>mObservers; // We now support multiple observers on inputs.
      IOObserver *mObserver;  // The first or only observer for faster checking.
  };

} // namespace forte::io

#endif /* SRC_CORE_IO_MAPPER_HANDLE_H_ */
