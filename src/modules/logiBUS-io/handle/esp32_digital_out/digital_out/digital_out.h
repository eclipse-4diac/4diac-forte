/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * digital_out.h
 *
 *  Created on: 12.02.2024
 *      Author: Franz Höpfinger
 *************************************************************************/
#pragma once

#include "soc/gpio_num.h"

#if defined(FORTE_MODULE_HUTSCHIENENMOPED)
#include "hutschienenmoped_digital_out.h"
#endif //FORTE_MODULE_HUTSCHIENENMOPED

#if defined(FORTE_MODULE_HUTSCHIENENMOPED_XL)
#include "hutschienenmoped_XL_digital_out.h"
#endif //FORTE_MODULE_HUTSCHIENENMOPED_XL

#if defined(FORTE_MODULE_DUALOUT)
#include "dual_out_digital_out.h"
#endif //FORTE_MODULE_DUALOUT

#if defined(FORTE_MODULE_DUAL_CAN)
#include "dual_can_digital_out.h"
#endif //FORTE_MODULE_DUAL_CAN

#if defined(FORTE_MODULE_ESP32_TESTBOARD)
#include "ESP32_Testboard_digital_out.h"
#endif //FORTE_MODULE_ESP32_TESTBOARD


