/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * dual_out_digital_out.h
 *
 *  Created on: 20.02.2023
 *      Author: Franz Höpfinger
 *************************************************************************/
#pragma once

#if CONFIG_IDF_TARGET_ESP32


  #define Q00_IO       (GPIO_NUM_NC)
  #define Q01_IO       (GPIO_NUM_4)
  #define Q02_IO       (GPIO_NUM_12)
  #define Q03_IO       (GPIO_NUM_NC)
  #define Q04_IO       (GPIO_NUM_NC)
  #define Q05_IO       (GPIO_NUM_NC)
  #define Q06_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q07_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q08_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q09_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q10_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q11_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */

  #define LEDC_IO_0    (GPIO_NUM_NC)
  #define LEDC_IO_1    (GPIO_NUM_NC)



#elif CONFIG_IDF_TARGET_ESP32S3


  #define Q00_IO       (GPIO_NUM_NC)
  #define Q01_IO       (GPIO_NUM_21)
  #define Q02_IO       (GPIO_NUM_46)
  #define Q03_IO       (GPIO_NUM_NC)
  #define Q04_IO       (GPIO_NUM_NC)
  #define Q05_IO       (GPIO_NUM_NC)
  #define Q06_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q07_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q08_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q09_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q10_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */
  #define Q11_IO       (GPIO_NUM_NC) /*!< GPIO_NUM_NC, input and output */

  #define LEDC_IO_0    (GPIO_NUM_NC)
  #define LEDC_IO_1    (GPIO_NUM_NC)




#else
  #error no known Target for dual_out_digital_out
#endif


