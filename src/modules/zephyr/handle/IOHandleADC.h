/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef IOHANDLEADC_H
#define IOHANDLEADC_H

#include <core/io/mapper/io_observer.h>
#include <handler/IODeviceController.h>
#include <handler/IOHandleADCDescriptor.h>
#include <stdint.h>

class IODeviceController;

class IOHandleADC : public forte::core::io::IOHandle {
  using IOObserver = forte::core::io::IOObserver;
  using IOHandle = forte::core::io::IOHandle;
  using IOMapper = forte::core::io::IOMapper;

public:
  IOHandleADC(IODeviceController *paDeviceCtrl, const adc_dt_spec* paADCSpec);
  ~IOHandleADC() override;
  void get(CIEC_ANY &) override;
  void set(const CIEC_ANY &) override;

protected:
  void onObserver(IOObserver *paObserver) override;
  void dropObserver() override;
  const adc_dt_spec* getADCSpec() const { return mADCSpec; }

private:
  const adc_dt_spec* mADCSpec;
#ifdef CONFIG_ADC_ASYNC
  // The total number of samples taken during a single sequence; 1 selects no oversampling.
  static constexpr size_t OversamplingCount = 50;
#else // CONFIG_ADC_ASYNC
  static constexpr size_t OversamplingCount = 1;
#endif // CONFIG_ADC_ASYNC
  uint16_t mSamplingBuffer[OversamplingCount] = { 0 };
  adc_sequence_options mSequenceOpts {
    .interval_us = 0,
#ifdef CONFIG_ADC_ASYNC
    .callback = sampling_callback,
    .user_data = this,
#endif // CONFIG_ADC_ASYNC
    .extra_samplings = OversamplingCount - 1
  };
  adc_sequence mSequence {
    .options = &mSequenceOpts,
    .buffer = mSamplingBuffer,
    .buffer_size = sizeof(mSamplingBuffer)
  };

#ifdef CONFIG_ADC_ASYNC
  static adc_action sampling_callback(const struct device *dev,
    const struct adc_sequence *sequence, uint16_t sampling_index);
  static void work_callback(struct k_work* item);
  struct adc_work_context_t {
    struct k_work work;
    IOHandleADC* self;
  };
  adc_work_context_t mADCWorkCtx;
  k_poll_signal mSamplingSignal;
  enum State { IDLE, SAMPLING, DESTROY };
  State mState{ IDLE };
#endif // CONFIG_ADC_ASYNC
  uint32_t mValue = 0;
};

#endif // IOHANDLEADC_H

