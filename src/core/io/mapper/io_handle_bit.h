/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
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

#ifndef SRC_CORE_IO_HANDLES_IO_HANDLE_BIT_H_
#define SRC_CORE_IO_HANDLES_IO_HANDLE_BIT_H_

#include <io/mapper/io_handle.h>
#include <io/device/io_controller.h>

namespace forte {
  namespace core {
    namespace io {

      class IOHandleBit : public IOHandle {
        public:
          IOHandleBit(IODeviceController *paController, IOMapper::Direction paDirection, uint8_t paOffset, uint8_t paPosition, uint8_t* paImage);

          virtual void set(const CIEC_ANY &);
          void get(CIEC_ANY &);

          bool equal(unsigned char* paOldImage) const;

        protected:
          virtual void onObserver(IOObserver *paObserver);

          virtual void dropObserver();

          virtual void reset() {
            CIEC_BOOL s = false;
            set(s);
          }

          const uint8_t mOffset;
          const uint8_t mMask;

        private:
          uint8_t* mImage;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_HANDLES_IO_HANDLE_BIT_H_ */
