/*******************************************************************************
 * Copyright (c) 2021, 2025 Jonathan Lainer (kontakt@lainer.co.at), HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *   Franz Höpfinger
 *******************************************************************************/

#ifndef SRC_MODULES_logiBUSIO_HANDLE_IOHANDLEESP32DIGITALO_H_
#define SRC_MODULES_logiBUSIO_HANDLE_IOHANDLEESP32DIGITALO_H_

#include <core/io/mapper/io_handle.h>
#include <core/io/mapper/io_observer.h>
#include <logiBUSIODeviceController.h>
#include <stdint.h>

class logiBUSIODeviceController;

class IOHandleESP32DigitalO : public forte::core::io::IOHandle {

public:
    IOHandleESP32DigitalO(
        logiBUSIODeviceController *paDeviceCtrl,
      forte::core::io::IOMapper::Direction paDirection,
      CIEC_ANY::EDataTypeID paDataType,
      logiBUSIOEnums::DigitalOutPinNumber paPin
      );
  void get(CIEC_ANY &) override;
  void set(const CIEC_ANY &) override;



private:
  int32_t gpio_num; //only used for Outputs.
};

#endif /* SRC_MODULES_logiBUSIO_HANDLE_IOHANDLEESP32DIGITALO_H_ */
