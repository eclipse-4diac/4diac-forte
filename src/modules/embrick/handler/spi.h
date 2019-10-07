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

#ifndef SRC_MODULES_EMBRICK_HANDLER_SPI_H_
#define SRC_MODULES_EMBRICK_HANDLER_SPI_H_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <devlog.h>
#include <forte_wstring.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

class EmbrickSPIHandler {
    friend class EmbrickBusHandler;

  protected:
    explicit EmbrickSPIHandler(unsigned int paInterface);
    virtual ~EmbrickSPIHandler();

    bool transfer(unsigned char* paSendBuffer, unsigned char* paRreceiveBuffer, int paLength);

    void init(const char* paSpidev);
    void deInit();
    template<typename T> bool config(unsigned int paConfig, unsigned int paConfigVerify, T paValue);

    bool hasError() {
      return mError != 0;
    }
    const char* mError;

    static unsigned long const scmDefaultSpiSpeed;
    static unsigned long const scmMaxSpiSpeed;
    void setSpeed(const unsigned long paSpeed);

  private:
    int mFd;
    unsigned long mSpiSpeed;

    static char const mSpiMode;
    static char const mSpiBitOrder;

    void fail(const char* paReason);

    static const char * const scmFailedToInitHandler;
    static const char * const scmFailedToConfigMode;
    static const char * const scmFailedToConfigBitOrder;
    static const char * const scmFailedToConfigSpeed;
    static const char * const scmFailedToTestBus;
    static const char * const scmFailedToTransferBuffer;
};

#endif /* SRC_MODULES_EMBRICK_HANDLER_SPI_H_ */
