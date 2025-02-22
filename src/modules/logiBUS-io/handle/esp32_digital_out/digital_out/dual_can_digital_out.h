/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * dual_can_digital_out.h
 *
 *  Created on: 20.02.2023
 *      Author: Franz Höpfinger
 *************************************************************************/
#pragma once

#if CONFIG_IDF_TARGET_ESP32


  #define Q00_IO       (GPIO_NUM_NC)
  #define Q01_IO       (GPIO_NUM_22)
  #define Q02_IO       (GPIO_NUM_4)
  #define Q03_IO       (GPIO_NUM_2)
  #define Q04_IO       (GPIO_NUM_15)
  #define Q05_IO       (GPIO_NUM_14)
  #define Q06_IO       (GPIO_NUM_32) /*!< GPIO_NUM_NC, input and output */
  #define Q07_IO       (GPIO_NUM_27) /*!< GPIO_NUM_NC, input and output */
  #define Q08_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q09_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q10_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q11_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */

  #define LEDC_IO_0    (GPIO_NUM_NC)
  #define LEDC_IO_1    (GPIO_NUM_NC)



#elif CONFIG_IDF_TARGET_ESP32S3


  #define Q00_IO       (GPIO_NUM_NC)
  #define Q01_IO       (GPIO_NUM_2)
  #define Q02_IO       (GPIO_NUM_10)
  #define Q03_IO       (GPIO_NUM_48)
  #define Q04_IO       (GPIO_NUM_47)
  #define Q05_IO       (GPIO_NUM_20)
  #define Q06_IO       (GPIO_NUM_19) /*!< GPIO_NUM_NC, input and output */
  #define Q07_IO       (GPIO_NUM_15) /*!< GPIO_NUM_NC, input and output */
  #define Q08_IO       (GPIO_NUM_8) /*!< GPIO_NUM_NC, input and output */
  #define Q09_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q10_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q11_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */

  #define LEDC_IO_0    (GPIO_NUM_NC)
  #define LEDC_IO_1    (GPIO_NUM_NC)




#else
  #error no known Target for ESP32_Testboard_digital_out
#endif


