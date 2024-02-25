/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef IOHANDLEGPIO_H
#define IOHANDLEGPIO_H

#include <core/io/mapper/io_observer.h>
#include <handler/IODeviceController.h>
#include <handler/IOHandleGPIODescriptor.h>
#include <stdint.h>

class IODeviceController;

class IOHandleGPIO : public forte::core::io::IOHandle {
  using IOObserver = forte::core::io::IOObserver;
  using IOHandle = forte::core::io::IOHandle;
  using IOMapper = forte::core::io::IOMapper;

public:
  IOHandleGPIO(IODeviceController *paDeviceCtrl, IOMapper::Direction paDirection,
    const gpio_dt_spec* paGPIOSpec, gpio_flags_t paFlags);
  ~IOHandleGPIO() override;
  void get(CIEC_ANY &) override;
  void set(const CIEC_ANY &) override;
  bool equal();

protected:
  void onObserver(IOObserver *paObserver) override;
  void dropObserver() override;
  const gpio_dt_spec* getGPIOSpec() const { return mGPIOSpec; }

private:
  static void irq_callback(const struct device* dev, struct gpio_callback* cb, uint32_t pins);

  const gpio_dt_spec* mGPIOSpec;
  bool mUsingCallback;
  struct gpio_callback_context_t {
    struct gpio_callback gpio_cb_data;
    IOHandleGPIO* self;
  };
  static void work_callback(struct k_work* item);
  struct gpio_work_context_t {
    struct k_work work;
    IOHandleGPIO* self;
  };
  gpio_work_context_t mGPIOWorkCtx;
  gpio_callback_context_t mGPIOCallbackCtx;
  bool mLastValue;
};

#endif // IOHANDLEGPIO_H

