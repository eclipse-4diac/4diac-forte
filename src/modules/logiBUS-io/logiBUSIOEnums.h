/*********************************************************************
* Copyright (c) 2025 HR Agrartechnik GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
*
* logiBUSIOEnums.h
*
*  Created on: 13.08.2022
*
* Contributors:
*   Franz Höpfinger - Initial implementation.
*
**********************************************************************/

#ifndef SRC_MODULES_LOGIBUS_IO_LOGIBUSIOENUMS_H_
#define SRC_MODULES_LOGIBUS_IO_LOGIBUSIOENUMS_H_

#include <cstdint>

class logiBUSIOEnums {

  public:

    enum class DigitalInPinNumber : int32_t {
      DigitalInPinNumber_Invalid = -1,
      //Digital In
      PinI0 = 0,
      PinI1,
      PinI2,
      PinI3,
      PinI4,
      PinI5,
      PinI6,
      PinI7,
      PinI8,
      DigitalInNumberOfPins,
    };



    enum class AnalogInPinNumber : int32_t {
      AnalogInPinNumber_Invalid = -1,
      //Digital In
      PinAI0 = 0,
      PinAI1,
      PinAI2,
      PinAI3,
      PinAI4,
      PinAI5,
      PinAI6,
      PinAI7,
      PinAI8,
      AnalogInNumberOfPins,
    };

    enum class DigitalOutPinNumber : int32_t {
      DigitalOutPinNumber_Invalid = -1,
      //Digital Out
      PinQ00 = 0,
      PinQ01,
      PinQ02,
      PinQ03,
      PinQ04,
      PinQ05,
      PinQ06,
      PinQ07,
      PinQ08,
      PinQ09,
      PinQ10,
      PinQ11,
      DigitalOutNumberOfPins,
    };


    enum class LEDStripsPinNumber : int32_t {
      LEDStripsPinNumber_Invalid = -1,
      //LED Strips
      PinS00 = 0,
      LEDStripsNumberOfPins,
    };


};

#endif /* SRC_MODULES_LOGIBUS_IO_LOGIBUSIOENUMS_H_ */
