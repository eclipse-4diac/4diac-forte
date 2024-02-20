/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_REVPI_REVPIHANDLE_H_
#define SRC_MODULES_REVPI_REVPIHANDLE_H_

#include <io/mapper/io_handle.h>
#include "RevPiController.h"

class RevPiHandle: public forte::core::IO::IOHandle {
public:
  RevPiHandle(RevPiController *controller, CIEC_ANY::EDataTypeID type, forte::core::IO::IOMapper::Direction direction,
      uint16_t offset, uint8_t position);

  ~RevPiHandle();

  virtual void set(const CIEC_ANY &);
  void get(CIEC_ANY &);

  bool check();

protected:
  SPIValueStr control;
  CIEC_ANY* valueLastCheck;

  virtual void dropObserver();
};

#endif /* SRC_MODULES_REVPI_REVPIHANDLE_H_ */
