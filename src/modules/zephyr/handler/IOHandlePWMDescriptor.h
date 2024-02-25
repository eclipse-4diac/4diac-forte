/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef IOHANDLEPMWDESCRIPTOR_H
#define IOHANDLEPMWDESCRIPTOR_H

#include <handler/IOHandleDescriptor.h>
#include <zephyr/drivers/pwm.h>

class IOHandlePWMDescriptor : public IOHandleDescriptor {
public:
  IOHandlePWMDescriptor(std::string const &paId, const pwm_dt_spec* paPwmSpec) :
    IOHandleDescriptor(paId, forte::core::io::IOMapper::In, Pwm), mPwmSpec(paPwmSpec) {}

  const pwm_dt_spec* getPwmSpec() const {
    return mPwmSpec;
  }

protected:
  const pwm_dt_spec* mPwmSpec;
};

#endif // IOHANDLEPWMDESCRIPTOR_H
