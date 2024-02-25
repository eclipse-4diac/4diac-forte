/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "IOHandleADC.h"
#include <forte_dword.h>
 
IOHandleADC::IOHandleADC(IODeviceController *paDeviceCtrl, const adc_dt_spec* paADCSpec) :
  IOHandle(paDeviceCtrl, IOMapper::In, CIEC_ANY::e_DWORD), mADCSpec(paADCSpec) {
  int ret = adc_channel_setup_dt(getADCSpec());
  if (ret < 0) {
    DEVLOG_ERROR("IOHandleADC::IOHandleADC: error %d, failed to configure %s channel %d\n",
      ret, getADCSpec()->dev->name, getADCSpec()->channel_id);
    return;
  }
  ret = adc_sequence_init_dt(getADCSpec(), &mSequence);
  if (ret < 0) {
    DEVLOG_ERROR("IOHandleADC::get: error %d, failed to initialize sequence %s channel %d\n",
      ret, getADCSpec()->dev->name, getADCSpec()->channel_id);
    return;
  }
#ifdef CONFIG_ADC_ASYNC
  k_work_init(&mADCWorkCtx.work, work_callback);
  mADCWorkCtx.self = this;
  k_poll_signal_init(&mSamplingSignal);
#endif // CONFIG_ADC_ASYNC
}

IOHandleADC::~IOHandleADC() {
  DEVLOG_INFO("IOHandleADC dtor\n");
#ifdef CONFIG_ADC_ASYNC
  if (mState == SAMPLING) {
    struct k_poll_event events[1] = {
      K_POLL_EVENT_INITIALIZER(K_POLL_TYPE_SIGNAL,
      K_POLL_MODE_NOTIFY_ONLY,
      &mSamplingSignal),
    };
    k_poll(events, 1, K_FOREVER);
  }
  mState = DESTROY;
#endif // CONFIG_ADC_ASYNC
}

void IOHandleADC::onObserver(IOObserver *paObserver) {
  DEVLOG_INFO("IOHandleADC::onObserver\n");
  IOHandle::onObserver(paObserver);
  mDirection = paObserver->getDirection();
}

void IOHandleADC::dropObserver() {
  DEVLOG_INFO("IOHandleADC::dropObserver\n");
  IOHandle::dropObserver();
  mDirection = IOMapper::UnknownDirection;
}

void IOHandleADC::get(CIEC_ANY &paState) {
  DEVLOG_DEBUG("IOHandleADC::get\n");

#ifdef CONFIG_ADC_ASYNC
  unsigned int signaled;
  int result;
  k_poll_signal_check(&mSamplingSignal, &signaled, &result);
  if (signaled && mState == SAMPLING) {
    mValue = 0;
    for (size_t i = 0; i < OversamplingCount; ++i) {
      mValue += mSamplingBuffer[i];
    }
    mValue /= OversamplingCount;

    mState = IDLE;
    k_poll_signal_reset(&mSamplingSignal);
  } else if (mState == IDLE) {
    int ret = adc_read_async(getADCSpec()->dev, &mSequence, &mSamplingSignal);
    if (ret == 0) {
      mState = SAMPLING;
    } else {
      DEVLOG_ERROR("IOHandleADC::get: error %d, failed to async read %s channel %d\n",
        ret, getADCSpec()->dev->name, getADCSpec()->channel_id);
      // to definitely repeat next try
      k_poll_signal_reset(&mSamplingSignal);
    }
  }
#else // CONFIG_ADC_ASYNC
  int ret = adc_read_dt(getADCSpec(), &mSequence);
  if (ret < 0) {
    DEVLOG_ERROR("IOHandleADC::get: error %d, failed to read %s channel %d\n",
      ret, getADCSpec()->dev->name, getADCSpec()->channel_id);
  } else {
    mValue = 0;
    for (size_t i = 0; i < OversamplingCount; ++i) {
      mValue += mSamplingBuffer[i];
    }
    mValue /= OversamplingCount;
  }
#endif // CONFIG_ADC_ASYNC
  static_cast<CIEC_DWORD &>(paState) = CIEC_DWORD(mValue);
}

void IOHandleADC::set(const CIEC_ANY &paState) {
  (void)paState;
  DEVLOG_ERROR("IOHandleADC::set\n");
}

#ifdef CONFIG_ADC_ASYNC
adc_action IOHandleADC::sampling_callback(const struct device *paDev,
  const struct adc_sequence *paSequence, uint16_t paSamplingIndex) {
  if (paSamplingIndex == OversamplingCount - 1)
  {
    auto self = static_cast<IOHandleADC*>(paSequence->options->user_data);
    k_work_submit(&self->mADCWorkCtx.work);
  }
  return ADC_ACTION_CONTINUE;
}

void IOHandleADC::work_callback(struct k_work* item) {
  DEVLOG_DEBUG("IOHandleADC::work_callback\n");
  adc_work_context_t* ctx = CONTAINER_OF(item, adc_work_context_t, work);
  // Use this to notify IOHandle of asynchronous value changes, i.e. IRQs
  ctx->self->onChange();
}
#endif // CONFIG_ADC_ASYNC

