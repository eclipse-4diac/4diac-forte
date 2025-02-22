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

#ifndef SRC_MODULES_LOGIBUS_IO_FB_LOGIBUSIOFUNCTIONBLOCK_H_
#define SRC_MODULES_LOGIBUS_IO_FB_LOGIBUSIOFUNCTIONBLOCK_H_

#include <forte_config.h>
#include <memory>
#include "funcbloc.h"
#include "core/io/mapper/io_mapper.h"
#include "handler/logiBUSIODeviceController.h"


#include <extevhan.h>
#include <esfb.h>
#include <resource.h>
class logiBUSIOFunctionBlock : public CEventSourceFB {

public:



  logiBUSIOFunctionBlock(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& pa_pstInterfaceSpec,
            CStringDictionary::TStringId pa_nInstanceNameId);

    ~logiBUSIOFunctionBlock() override;



    forte::core::io::IOHandle * mapPin(
        std::string                   const &paId,
        forte::core::io::IOMapper::Direction paDirection,
        CIEC_ANY::EDataTypeID                paDataType,
        logiBUSIODeviceController::HandleType paHandleType,
        int32_t                              paPin,
        uint32_t                             paAnalogInputHysteresis = 0,
        uint16_t                             paColour = 0
        );


    forte::core::io::IOHandle * mapPin(
        std::string                   const &paId,
        forte::core::io::IOMapper::Direction paDirection,
        CIEC_ANY::EDataTypeID                paDataType,
        logiBUSIODeviceController::HandleType paHandleType,
        logiBUSIOEnums::DigitalInPinNumber    paPin
        );

    forte::core::io::IOHandle * mapPin(
        std::string                   const &paId,
        forte::core::io::IOMapper::Direction paDirection,
        CIEC_ANY::EDataTypeID                paDataType,
        logiBUSIODeviceController::HandleType paHandleType,
        logiBUSIOEnums::AnalogInPinNumber     paPin,
        uint32_t                             paAnalogInputHysteresis
        );

    forte::core::io::IOHandle * mapPin(
        std::string                   const &paId,
        forte::core::io::IOMapper::Direction paDirection,
        CIEC_ANY::EDataTypeID                paDataType,
        logiBUSIODeviceController::HandleType paHandleType,
        logiBUSIOEnums::DigitalOutPinNumber   paPin
        );

    forte::core::io::IOHandle * mapPin(
        std::string                   const &paId,
        forte::core::io::IOMapper::Direction paDirection,
        CIEC_ANY::EDataTypeID                paDataType,
        logiBUSIODeviceController::HandleType paHandleType,
        logiBUSIOEnums::LEDStripsPinNumber    paPin,
        uint16_t                             paColour
        );



protected:
    static const CIEC_STRING scmOK;
    static const CIEC_STRING scmqTimedOut;
    static const CIEC_STRING scmNotInitialised;
};



#endif /* SRC_MODULES_LOGIBUS_IO_FB_LOGIBUSIOFUNCTIONBLOCK_H_ */
