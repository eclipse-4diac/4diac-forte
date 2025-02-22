/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * hutschienenmoped_XL_digital_in.h
 *
 *  Created on: 04.10.2023
 *      Author: franz
 *************************************************************************/
#pragma once


#if CONFIG_IDF_TARGET_ESP32S3
  #define I0_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define I1_IO       (GPIO_NUM_13) /*!< GPIO_NUM_13, input and output */
  #define I2_IO       (GPIO_NUM_12) /*!< GPIO_NUM_12, input and output */
  #define I3_IO       (GPIO_NUM_9 ) /*!< GPIO_NUM_9 , input and output */
  #define I4_IO       (GPIO_NUM_3 ) /*!< GPIO_NUM_3 , input and output */
  #define I5_IO       (GPIO_NUM_18) /*!< GPIO_NUM_18, input and output */
  #define I6_IO       (GPIO_NUM_17) /*!< GPIO_NUM_17, input and output */
  #define I7_IO       (GPIO_NUM_7 ) /*!< GPIO_NUM_7 , input and output */
  #define I8_IO       (GPIO_NUM_6 ) /*!< GPIO_NUM_6 , input and output */
  #define ACTIVE_LEVEL (1)
#else
  #error no known Target for hutschienenmoped_XL_digital_in
#endif

