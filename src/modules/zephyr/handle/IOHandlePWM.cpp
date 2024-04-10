/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "IOHandlePWM.h"

class IOHandlePWMPeriod : public forte::core::io::IOHandle {
  using IOObserver = forte::core::io::IOObserver;
  using IOHandle = forte::core::io::IOHandle;
  using IOMapper = forte::core::io::IOMapper;

public:
  IOHandlePWMPeriod(IODeviceController* paDeviceCtrl, IOHandlePWM* paMasterHandle) :
    IOHandle(paDeviceCtrl, IOMapper::Out, CIEC_ANY::e_DWORD), mMasterHandle(paMasterHandle) {
    DEVLOG_DEBUG("IOHandlePWMPeriod::IOHandlePWMPeriod\n");
  }

  void get(CIEC_ANY& paState) override {
    DEVLOG_DEBUG("IOHandlePWMPeriod::get\n");
    static_cast<CIEC_DWORD &>(paState) = mValue;
  }
  void set(const CIEC_ANY& paState) override {
    DEVLOG_DEBUG("IOHandlePWMPeriod::set\n");
    mValue = static_cast<const CIEC_DWORD &>(paState);
    mMasterHandle->reload();
  }

protected:
  void onObserver(IOObserver *paObserver) {
    DEVLOG_INFO("IOHandlePWMPeriod::onObserver\n");
    IOHandle::onObserver(paObserver);
    mDirection = paObserver->getDirection();
  }

  void dropObserver() {
    DEVLOG_INFO("IOHandlePWMPeriod::dropObserver\n");
    IOHandle::dropObserver();
    mDirection = IOMapper::UnknownDirection;
  }

private:
  IOHandlePWM* const mMasterHandle{ 0 };
  CIEC_DWORD mValue{ 0 };
};
 
IOHandlePWM::IOHandlePWM(IODeviceController* paDeviceCtrl,
  std::string const& paId, const pwm_dt_spec* paPWMSpec) :
  IOHandle(paDeviceCtrl, IOMapper::Out, CIEC_ANY::e_DWORD), mPWMSpec(paPWMSpec) {
  if (!pwm_is_ready_dt(getPWMSpec())) {
    DEVLOG_ERROR("IOHandlePWM::IOHandlePWM: failed to configure %s channel %d\n",
      getPWMSpec()->dev->name, getPWMSpec()->channel);
    return;
  }
  // to register auxilliary PWM period handle with the mapper
  if(!paId.empty()) {
    mPwmPeriodId = paId + "_Period";
    mPwmPeriodHandle = new IOHandlePWMPeriod(paDeviceCtrl, this);
    IOMapper::getInstance().registerHandle(mPwmPeriodId, mPwmPeriodHandle);
  }
}

IOHandlePWM::~IOHandlePWM() {
  DEVLOG_INFO("IOHandlePWM dtor\n");
  // stop PWM, or set idle to 100%
  if (mPwmPeriodHandle != nullptr) {
    delete mPwmPeriodHandle;
    mPwmPeriodHandle = nullptr;
  }
  pwm_set_pulse_dt(getPWMSpec(), 0);
}

void IOHandlePWM::get(CIEC_ANY& paState) {
  DEVLOG_DEBUG("IOHandlePWM::get\n");
  static_cast<CIEC_DWORD &>(paState) = mValue;
}

void IOHandlePWM::set(const CIEC_ANY& paState) {
  DEVLOG_DEBUG("IOHandlePWM::set\n");
  mValue = static_cast<const CIEC_DWORD&>(paState);
  reload();
}

void IOHandlePWM::reload() {
  if (mPwmPeriodHandle != nullptr && mPwmPeriodHandle->hasObserver()) {
    CIEC_DWORD period;
    mPwmPeriodHandle->get(period);
    DEVLOG_DEBUG("IOHandlePWM::reload %s channel %d, duty %d, period %d\n",
      getPWMSpec()->dev->name, getPWMSpec()->channel, static_cast<uint32_t>(mValue), static_cast<uint32_t>(period));
    pwm_set_dt(getPWMSpec(), period, mValue);
  } else {
    DEVLOG_DEBUG("IOHandlePWM::reload %s channel %d, duty %d\n",
      getPWMSpec()->dev->name, getPWMSpec()->channel, static_cast<uint32_t>(mValue));
    pwm_set_pulse_dt(getPWMSpec(), mValue);
  }
}

void IOHandlePWM::onObserver(IOObserver *paObserver) {
  DEVLOG_INFO("IOHandlePWM::onObserver\n");
  IOHandle::onObserver(paObserver);
  mDirection = paObserver->getDirection();
}

void IOHandlePWM::dropObserver() {
  DEVLOG_INFO("IOHandlePWM::dropObserver\n");
  IOHandle::dropObserver();
  mDirection = IOMapper::UnknownDirection;
}
