/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * logiBUSIOFunctionBlock.cpp
 *
 *  Created on: 13.08.2022
 *      Author: Franz Höpfinger
 *************************************************************************/

#include "logiBUSIOFunctionBlock.h"
#include <extevhandlerhelper.h>


logiBUSIOFunctionBlock::logiBUSIOFunctionBlock(
    forte::core::CFBContainer &paContainer,
    const SFBInterfaceSpec& pa_pstInterfaceSpec,
    CStringDictionary::TStringId pa_nInstanceNameId)
:   CEventSourceFB(
    paContainer
  , pa_pstInterfaceSpec
  , pa_nInstanceNameId)
{}

logiBUSIOFunctionBlock::~logiBUSIOFunctionBlock() = default;





const CIEC_STRING logiBUSIOFunctionBlock::scmOK                 = "OK"_STRING;
const CIEC_STRING logiBUSIOFunctionBlock::scmqTimedOut          = "qTimedOut"_STRING;
const CIEC_STRING logiBUSIOFunctionBlock::scmNotInitialised     = "Not initialized"_STRING;




forte::core::io::IOHandle* logiBUSIOFunctionBlock::mapPin(
    std::string                   const &paId,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID                paDataType,
    logiBUSIODeviceController::HandleType paHandleType,
    int32_t                              paPin,
    uint32_t                             paAnalogInputHysteresis,
    uint16_t                             paColour
    )
{
  // Only initialize pins that have an id.
  if (false) {
    //noop
  } else if (paId.empty()) {
    DEVLOG_INFO("logiBUSIOFunctionBlock mapped pin -------- (ID empty)\n");
  } else {
    DEVLOG_INFO("logiBUSIOFunctionBlock mapped pin to ##%s##  \n", paId.c_str());

    // Create a GPIO pin handle using the port struct to identify the MMIO port and
    // a bit mask to identify the pin.
    logiBUSIODeviceController::logiBUSIOHandleDescriptor descr(
        paId,
        paDirection,
        paDataType,
        paHandleType,
        paPin,
        paAnalogInputHysteresis,
        paColour
        );

    logiBUSIODeviceController &ctrl     = getExtEvHandler<logiBUSIODeviceController>(*this);
    forte::core::io::IOHandle * handle = ctrl.createIOHandle(descr);
    forte::core::io::IOMapper& mapper  = forte::core::io::IOMapper::getInstance();

    mapper.registerHandle(paId, handle);
    return handle;
  }
  return nullptr; //all other cases.
}



forte::core::io::IOHandle * logiBUSIOFunctionBlock::mapPin(
    std::string                   const &paId,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID                paDataType,
    logiBUSIODeviceController::HandleType paHandleType,
    logiBUSIOEnums::DigitalInPinNumber    paPin
    )
{
  return mapPin(
       paId,
       paDirection,
       paDataType,
       paHandleType,
       static_cast<int32_t>(paPin)
       );
}

forte::core::io::IOHandle * logiBUSIOFunctionBlock::mapPin(
  std::string                     const &paId,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID                paDataType,
    logiBUSIODeviceController::HandleType paHandleType,
    logiBUSIOEnums::AnalogInPinNumber     paPin,
    uint32_t                             paAnalogInputHysteresis
    )
{
  return mapPin(
      paId,
      paDirection,
      paDataType,
      paHandleType,
      static_cast<int32_t>(paPin),
      paAnalogInputHysteresis
      );
}

forte::core::io::IOHandle * logiBUSIOFunctionBlock::mapPin(
    std::string                   const &paId,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID                paDataType,
    logiBUSIODeviceController::HandleType paHandleType,
    logiBUSIOEnums::DigitalOutPinNumber   paPin
    )
{
  return mapPin(
      paId,
      paDirection,
      paDataType,
      paHandleType,
      static_cast<int32_t>(paPin)
      );
}


forte::core::io::IOHandle * logiBUSIOFunctionBlock::mapPin(
    std::string                   const &paId,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID                paDataType,
    logiBUSIODeviceController::HandleType paHandleType,
    logiBUSIOEnums::LEDStripsPinNumber    paPin,
    uint16_t                             paColour
    )
{
  return mapPin(
      paId,
      paDirection,
      paDataType,
      paHandleType,
      static_cast<int32_t>(paPin),
      (uint32_t)0,
      paColour
      );
}




