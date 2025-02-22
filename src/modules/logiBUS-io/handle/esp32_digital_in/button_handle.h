/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * button_handle.h
 *
 *  Created on: 16.08.2022
 *      Author: Franz Höpfinger
 *************************************************************************/
#pragma once


#include <cstdint>
#include "iot_button.h"
#include "logiBUSIOEnums.h"

#define VOLUME_NUM 6


extern button_handle_t g_btns[static_cast<int32_t>(logiBUSIOEnums::DigitalInPinNumber::DigitalInNumberOfPins)];

extern const uint16_t volmin[VOLUME_NUM];
extern const uint16_t volmax[VOLUME_NUM];

extern size_t get_btn_index(button_handle_t btn);



#
