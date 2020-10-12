/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
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

#ifndef SRC_CORE_IO_MAPPER_HANDLE_H_
#define SRC_CORE_IO_MAPPER_HANDLE_H_

#include <forte_any.h>
#include <forte_bool.h>

#include "io_mapper.h"

namespace forte {
  namespace core {
    namespace io {

      class IODeviceController;

      class IOHandle {
          friend class IOMapper;

        public:
          IOHandle(IODeviceController *paController, IOMapper::Direction paDirection, CIEC_ANY::EDataTypeID paType);
          virtual ~IOHandle();

          bool hasObserver() const {
            return !!mObserver;
          }

          IOObserver* getObserver() {
            return mObserver;
          }

          bool is(CIEC_ANY::EDataTypeID paType) const {
            return this->mType == paType;
          }

          bool is(IOMapper::Direction paDirection) const {
            return this->mDirection == paDirection;
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

        private:

          IOObserver *mObserver;


      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_MAPPER_HANDLE_H_ */
