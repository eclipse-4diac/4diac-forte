/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * hutschienenmoped_XL_digital_out.h
 *
 *  Created on: 20.02.2023
 *      Author: Franz Höpfinger
 *************************************************************************/
#pragma once

#if CONFIG_IDF_TARGET_ESP32S3
  #define Q00_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q01_IO       (GPIO_NUM_21) /*!< GPIO_NUM_21, input and output */
  #define Q02_IO       (GPIO_NUM_47) /*!< GPIO_NUM_47, input and output */
  #define Q03_IO       (GPIO_NUM_39) /*!< GPIO_NUM_39, input and output */
  #define Q04_IO       (GPIO_NUM_40) /*!< GPIO_NUM_40, input and output */
  #define Q05_IO       (GPIO_NUM_41) /*!< GPIO_NUM_41, input and output */
  #define Q06_IO       (GPIO_NUM_42) /*!< GPIO_NUM_42, input and output */
  #define Q07_IO       (GPIO_NUM_1)  /*!< GPIO_NUM_1, input and output */
  #define Q08_IO       (GPIO_NUM_2)  /*!< GPIO_NUM_2, input and output */
  #define Q09_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q10_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q11_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */

#define LEDC_IO_0    (12)
#define LEDC_IO_1    (11)

#else
  #error no known Target for hutschienenmoped_XL_digital_out
#endif


