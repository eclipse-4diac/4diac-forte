/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * hutschienenmoped_analog_in.h
 *
 *  Created on: 20.02.2023
 *      Author: franz
 *************************************************************************/
#pragma once


#if CONFIG_IDF_TARGET_ESP32
  #define AI0_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define AI1_IO       (GPIO_NUM_26) /*!< GPIO_NUM_26, input and output */
  #define AI2_IO       (GPIO_NUM_32) /*!< GPIO_NUM_32, input and output */
  #define AI3_IO       (GPIO_NUM_39) /*!< GPIO_NUM_39, input and output */
  #define AI4_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define AI5_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define AI6_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define AI7_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define AI8_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
#else
  #error no known Target for hutschienenmoped_analog_in
#endif

