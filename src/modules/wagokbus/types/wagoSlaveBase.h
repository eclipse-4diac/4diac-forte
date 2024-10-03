/*************************************************************************
 * Copyright (c) 2016, 2024 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#pragma once

#include "../../../../core/io/configFB/io_slave_multi.h"

#define FUNCTION_BLOCK_CTOR_FOR_WAGO_SLAVES(fbclass, type) \
  fbclass(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) : \
  WagoSlaveBase(type, paContainer, scmFBInterfaceSpec, paInstanceNameId)

#define INIT_HANDLES(noOfBoolInputs, noOfBoolOutputs, noOfAnalogInputs, noOfAnalogOutputs) \
   void initHandles() override {initHandlesBase(noOfBoolInputs, noOfBoolOutputs, noOfAnalogInputs, noOfAnalogOutputs);};

class WagoSlaveBase : public forte::core::io::IOConfigFBMultiSlave {

  public:
    WagoSlaveBase(int paType, forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId);

    ~WagoSlaveBase() override = default;

  protected:

    void initHandlesBase(size_t paNumberOfBoolInputs, size_t paNumberOfBoolOutputs, size_t paNumberOfAnalogInputs, size_t paNumberOfAnalogOutputs);

    virtual void initHandles() = 0;

  private:

    static const TForteUInt8 scmSlaveConfigurationIO[];
    static const TForteUInt8 scmSlaveConfigurationIONum;
};
