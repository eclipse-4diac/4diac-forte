/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * ESP32_Testboard_analog_in.h
 *
 *  Created on: 04.10.2023
 *      Author: Franz Höpfinger
*************************************************************************/
#pragma once


#if CONFIG_IDF_TARGET_ESP32S3
  #define AI0_IO       (GPIO_NUM_14) /*!< GPIO_NUM_14, input and output */
  #define AI1_IO       (GPIO_NUM_13) /*!< GPIO_NUM_13, input and output */
  #define AI2_IO       (GPIO_NUM_12) /*!< GPIO_NUM_12, input and output */
  #define AI3_IO       (GPIO_NUM_3 ) /*!< GPIO_NUM_3 , input and output */
  #define AI4_IO       (GPIO_NUM_17) /*!< GPIO_NUM_17, input and output */
  #define AI5_IO       (GPIO_NUM_16) /*!< GPIO_NUM_16, input and output */
  #define AI6_IO       (GPIO_NUM_15) /*!< GPIO_NUM_15, input and output */
  #define AI7_IO       (GPIO_NUM_38) /*!< GPIO_NUM_38, input and output */
  #define AI8_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
#else
  #error no known Target for ESP32_Testboard_analog_in
#endif


