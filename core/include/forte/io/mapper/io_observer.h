/*******************************************************************************
 * Copyright (c) 2016 - 2022 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                           Jonathan Lainer
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Jonathan Lainer - Adding getter for mDirection.
 *******************************************************************************/

#pragma once

#include "forte/io/mapper/io_handle.h"
#include "forte/io/mapper/io_mapper.h"

namespace forte::io {

  class IOObserver {
      friend class IOMapper;

    public:
      IOObserver();
      virtual ~IOObserver();

      virtual bool onChange() = 0;

      virtual CIEC_ANY::EDataTypeID getIOObserverDataType() = 0;
      virtual IOMapper::Direction getDirection() = 0;

      IOHandle *getHandle() {
        return mHandle;
      }

    protected:
      virtual void onHandle(IOHandle *const paHandle);
      virtual void dropHandle();

    private:
      IOHandle *mHandle;
  };

} // namespace forte::io
