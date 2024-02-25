/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef IOHANDLEADCDESCRIPTOR_H
#define IOHANDLEADCDESCRIPTOR_H

#include <handler/IOHandleDescriptor.h>
#include <zephyr/drivers/adc.h>

class IOHandleADCDescriptor : public IOHandleDescriptor {
public:
  IOHandleADCDescriptor(std::string const &paId, const adc_dt_spec* paAdcSpec) :
    IOHandleDescriptor(paId, forte::core::io::IOMapper::In, Adc), mAdcSpec(paAdcSpec) {}

  const adc_dt_spec* getAdcSpec() const {
    return mAdcSpec;
  }

protected:
  const adc_dt_spec* mAdcSpec;
};

#endif // IOHANDLEADCDESCRIPTOR_H
