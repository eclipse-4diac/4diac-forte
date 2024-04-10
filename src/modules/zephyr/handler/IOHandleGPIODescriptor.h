/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef IOHANDLEGPIODESCRIPTOR_H
#define IOHANDLEGPIODESCRIPTOR_H

#include <handler/IOHandleDescriptor.h>
#include <zephyr/drivers/gpio.h>

class IOHandleGPIODescriptor : public IOHandleDescriptor {
public:
  IOHandleGPIODescriptor(std::string const &paId, forte::core::io::IOMapper::Direction paDirection,
    const gpio_dt_spec* paGpioSpec, gpio_flags_t paFlags) :
    IOHandleDescriptor(paId, paDirection, Gpio), mGpioSpec(paGpioSpec), mFlags(paFlags) {}

  const gpio_dt_spec* getGpioSpec() const {
    return mGpioSpec;
  }

  gpio_flags_t getFlags() const {
    return mFlags;
  }

protected:
  const gpio_dt_spec* mGpioSpec;
  gpio_flags_t mFlags;
};

#endif // IOHANDLEGPIODESCRIPTOR_H
