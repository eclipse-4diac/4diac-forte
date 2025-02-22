/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *
 * hutschienenmoped_XL_analog_in.h
 *
 *  Created on: 04.10.2023
 *      Author: franz
 *************************************************************************/
#pragma once


#if CONFIG_IDF_TARGET_ESP32S3
  #define AI0_IO       (static_cast<adc_channel_t>(SOC_ADC_CHANNEL_NUM(ADC_UNIT))) /*!< Use to signal not connected to S/W */
  #define AI1_IO       (static_cast<adc_channel_t>(SOC_ADC_CHANNEL_NUM(ADC_UNIT))) /*!< Use to signal not connected to S/W */
  #define AI2_IO       (static_cast<adc_channel_t>(SOC_ADC_CHANNEL_NUM(ADC_UNIT))) /*!< Use to signal not connected to S/W */
  #define AI3_IO       (ADC_CHANNEL_8 ) /*!< ADC1 channel 8 is GPIO9  */
  #define AI4_IO       (ADC_CHANNEL_2 ) /*!< ADC1 channel 2 is GPIO3  */
  #define AI5_IO       (static_cast<adc_channel_t>(SOC_ADC_CHANNEL_NUM(ADC_UNIT))) /*!< Use to signal not connected to S/W */
  #define AI6_IO       (static_cast<adc_channel_t>(SOC_ADC_CHANNEL_NUM(ADC_UNIT))) /*!< Use to signal not connected to S/W */
  #define AI7_IO       (ADC_CHANNEL_6 ) /*!< ADC1 channel 6 is GPIO7  */
  #define AI8_IO       (ADC_CHANNEL_5 ) /*!< ADC1 channel 5 is GPIO6  */
#else
  #error no known Target for hutschienenmoped_XL_analog_in
#endif

