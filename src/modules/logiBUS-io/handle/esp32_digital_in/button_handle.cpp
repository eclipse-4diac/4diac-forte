/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * button_handle.cpp
 *
 *  Created on: 16.08.2022
 *      Author: Franz Höpfinger
 *************************************************************************/
#include "button_handle.h"


button_handle_t g_btns[static_cast<int32_t>(logiBUSIOEnums::DigitalInPinNumber::DigitalInNumberOfPins)] = {0};
const uint16_t volmin[VOLUME_NUM] = { 190,  600, 1000, 1375, 1775, 2195 };
const uint16_t volmax[VOLUME_NUM] = { 600, 1000, 1375, 1775, 2195, 2705 };

size_t get_btn_index(button_handle_t btn)
{
    for (int32_t i = 0; i < static_cast<int32_t>(logiBUSIOEnums::DigitalInPinNumber::DigitalInNumberOfPins); i++) {
        if (btn == g_btns[i]) {
            return i;
        }
    }
    return -1;
}
