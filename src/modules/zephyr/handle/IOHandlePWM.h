/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef IOHANDLEPWM_H
#define IOHANDLEPWM_H

#include <core/io/mapper/io_observer.h>
#include <handler/IODeviceController.h>
#include <handler/IOHandlePWMDescriptor.h>
#include <forte_dword.h>
#include <stdint.h>

class IODeviceController;

class IOHandlePWM : public forte::core::io::IOHandle {
  using IOObserver = forte::core::io::IOObserver;
  using IOHandle = forte::core::io::IOHandle;
  using IOMapper = forte::core::io::IOMapper;

public:
  IOHandlePWM(IODeviceController* paDeviceCtrl, std::string const& paId, const pwm_dt_spec* paPWMSpec);

  ~IOHandlePWM() override;

  void get(CIEC_ANY& paState) override;
  void set(const CIEC_ANY& paState) override;

  void reload();

protected:
  void onObserver(IOObserver *paObserver) override;

  void dropObserver() override;

  const pwm_dt_spec* getPWMSpec() const { return mPWMSpec; }

private:
  const pwm_dt_spec* mPWMSpec;
  std::string mPwmPeriodId;
  IOHandle* mPwmPeriodHandle{ nullptr };
  CIEC_DWORD mValue{ 0 };
};

#endif // IOHANDLEPWM_H

