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

#ifndef SRC_MODULES_EMBRICK_HANDLER_SPI_H_
#define SRC_MODULES_EMBRICK_HANDLER_SPI_H_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

namespace EmBrick {

class SPIHandler {
  friend class BusHandler;

protected:
  SPIHandler();
  virtual ~SPIHandler();

  bool transfer(unsigned char* sendBuffer, unsigned char* receiveBuffer,
      int length);

  void init();
  void deInit();
  void fail(const char* reason);
  template<typename T> bool config(unsigned int config,
      unsigned int configVerify, T value);

  bool ready() {
    return error != NULL;
  }
  char * error;

private:
  int fd;

  static char const spiMode;
  static char const spiBitOrder;
  static unsigned long const spiSpeed;

  static const char * const scmFailedToInitHandler;
  static const char * const scmFailedToConfigMode;
  static const char * const scmFailedToConfigBitOrder;
  static const char * const scmFailedToConfigSpeed;
  static const char * const scmFailedToTestBus;
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_HANDLER_SPI_H_ */
