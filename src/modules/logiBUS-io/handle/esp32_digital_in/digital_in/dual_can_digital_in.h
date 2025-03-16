/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * dual_can_digital_in.h
 *
 *  Created on: 04.10.2023
 *      Author: Franz Höpfinger
 *************************************************************************/
#pragma once


#if CONFIG_IDF_TARGET_ESP32S3
  #define I0_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define I1_IO       (GPIO_NUM_5) /**/
  #define I2_IO       (GPIO_NUM_4) /**/
  #define I3_IO       (GPIO_NUM_6 ) /**/
  #define I4_IO       (GPIO_NUM_16 ) /**/
  #define I5_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define I6_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define I7_IO       (GPIO_NUM_NC ) /*!< Use to signal not connected to S/W */
  #define I8_IO       (GPIO_NUM_NC ) /*!< Use to signal not connected to S/W */
  #define ACTIVE_LEVEL (1)
#else
  #define I0_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define I1_IO       (GPIO_NUM_NC) /**/
  #define I2_IO       (GPIO_NUM_NC) /**/
  #define I3_IO       (GPIO_NUM_NC ) /**/
  #define I4_IO       (GPIO_NUM_NC ) /**/
  #define I5_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define I6_IO       (GPIO_NUM_NC) /*!< Use to signal not connected to S/W */
  #define I7_IO       (GPIO_NUM_NC ) /*!< Use to signal not connected to S/W */
  #define I8_IO       (GPIO_NUM_NC ) /*!< Use to signal not connected to S/W */
  #define ACTIVE_LEVEL (1)
#endif

