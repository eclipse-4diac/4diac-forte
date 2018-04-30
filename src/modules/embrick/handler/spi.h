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
#include <devlog.h>
#include <forte_wstring.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

namespace forte {
  namespace core {
    namespace IO {

class EmbrickSPIHandler {
  friend class EmbrickBusHandler;

protected:
  explicit EmbrickSPIHandler(unsigned int interface);
  virtual ~EmbrickSPIHandler();

  bool transfer(unsigned char* sendBuffer, unsigned char* receiveBuffer,
      int length);

  void init(const char* spidev);
  void deInit();
  template<typename T> bool config(unsigned int config,
      unsigned int configVerify, T value);

  bool hasError() {
    return error != 0;
  }
  const char* error;

  static unsigned long const DefaultSpiSpeed;
  static unsigned long const MaxSpiSpeed;
  void setSpeed(const unsigned long speed);

private:
  int fd;
  unsigned long spiSpeed;

  static char const spiMode;
  static char const spiBitOrder;

  void fail(const char* reason);

  static const char * const scmFailedToInitHandler;
  static const char * const scmFailedToConfigMode;
  static const char * const scmFailedToConfigBitOrder;
  static const char * const scmFailedToConfigSpeed;
  static const char * const scmFailedToTestBus;
  static const char * const scmFailedToTransferBuffer;
};

    } //namespace IO
  } //namepsace core
} //namespace forte


#endif /* SRC_MODULES_EMBRICK_HANDLER_SPI_H_ */
