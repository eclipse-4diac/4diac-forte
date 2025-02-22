/*******************************************************************************
 * Copyright (c) 2021, 2025 Jonathan Lainer (kontakt@lainer.co.at), HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *   Franz Höpfinger
 *******************************************************************************/

#pragma once

#include <core/io/mapper/io_handle.h>
#include <core/io/mapper/io_observer.h>
#include <logiBUSIODeviceController.h>
#include <stdint.h>


#include "esp_adc/adc_continuous.h"

#include "soc/soc_caps.h"

#define ADC_UNIT                    ADC_UNIT_1

class logiBUSIODeviceController;

class IOHandleESP32AnalogIN : public forte::core::io::IOHandle {

public:
    IOHandleESP32AnalogIN(
        logiBUSIODeviceController            *paDeviceCtrl,
        forte::core::io::IOMapper::Direction paDirection,
        CIEC_ANY::EDataTypeID                paDataType,
        logiBUSIOEnums::AnalogInPinNumber     paPin,
        uint32_t                             paAnalogInputHysteresis
        );
    ~IOHandleESP32AnalogIN();

  void get(CIEC_ANY &) override;
  void set(const CIEC_ANY &) override;

  static void startTask();
  static void stopTask();

private:
  adc_channel_t chan_num;
  uint32_t newData;
  uint32_t oldData;
  uint32_t hysteresis;
  uint32_t adc_reading;
  uint32_t sample_num;
  static const char * const TAG;
  static const int scmTaskPriority;
  static TaskHandle_t s_task_handle;
  static uint8_t channel_num;
  static adc_channel_t channel[SOC_ADC_CHANNEL_NUM(ADC_UNIT)];
  static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data);
  static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle);
  static void continuous_adc_task( void * pvParameters );
  static bool has_to_stop;


  //Callback
  static IOHandleESP32AnalogIN* mCallbackDirectory[SOC_ADC_CHANNEL_NUM(ADC_UNIT)];

};

