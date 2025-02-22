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

#include "IOHandleESP32AnalogIN.h"

#include <cstdbool>
#include <cstdlib>

#include "button_handle.h"
#include "forte_bool.h"
#include "forte_usint.h"
#include "forte_uint.h"
#include "forte_dword.h"


#include "analog_in.h" //Pin Definitions


#include "esp_log.h"







#define _ADC_UNIT_STR(unit)         #unit
#define ADC_UNIT_STR(unit)          _ADC_UNIT_STR(unit)
#define ADC_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define ADC_ATTEN                   ADC_ATTEN_DB_0
#define ADC_BIT_WIDTH               SOC_ADC_DIGI_MAX_BITWIDTH

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define ADC_GET_CHANNEL(p_data)     ((p_data)->type1.channel)
#define ADC_GET_DATA(p_data)        ((p_data)->type1.data)
#else
#define ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE2
#define ADC_GET_CHANNEL(p_data)     ((p_data)->type2.channel)
#define ADC_GET_DATA(p_data)        ((p_data)->type2.data)
#endif

#define READ_LEN                    256

adc_channel_t IOHandleESP32AnalogIN::channel[SOC_ADC_CHANNEL_NUM(ADC_UNIT)] = {static_cast<adc_channel_t>(SOC_ADC_CHANNEL_NUM(ADC_UNIT))};
uint8_t IOHandleESP32AnalogIN::channel_num = 0;


TaskHandle_t IOHandleESP32AnalogIN::s_task_handle = nullptr;
const char * const IOHandleESP32AnalogIN::TAG = "IOHandleESP32AnalogIN";


bool IOHandleESP32AnalogIN::has_to_stop = false;

//Callback

IOHandleESP32AnalogIN* IOHandleESP32AnalogIN::mCallbackDirectory[SOC_ADC_CHANNEL_NUM(ADC_UNIT)] = {nullptr}; //0-init.

/*
 * A working test was done using this priority, the ethernet thread used priority 2 and the IP thread used 3.
 */
const int IOHandleESP32AnalogIN::scmTaskPriority = tskIDLE_PRIORITY + 1;


IOHandleESP32AnalogIN::IOHandleESP32AnalogIN(
    logiBUSIODeviceController            *paDeviceCtrl,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID                paDataType,
    logiBUSIOEnums::AnalogInPinNumber     paPin,
    uint32_t                             paAnalogInputHysteresis
    )
    : forte::core::io::IOHandle(static_cast<forte::core::io::IODeviceController*>(paDeviceCtrl), paDirection, paDataType)
    , chan_num(static_cast<adc_channel_t>(SOC_ADC_CHANNEL_NUM(ADC_UNIT)))  //invalid Channel.
    , newData(0)
    , oldData(0)
    , hysteresis(paAnalogInputHysteresis)
    , adc_reading(0)
    , sample_num(0)
  {
  switch(paPin){
    case logiBUSIOEnums::AnalogInPinNumber::PinAI0:      chan_num = AI0_IO;      break;
    case logiBUSIOEnums::AnalogInPinNumber::PinAI1:      chan_num = AI1_IO;      break;
    case logiBUSIOEnums::AnalogInPinNumber::PinAI2:      chan_num = AI2_IO;      break;
    case logiBUSIOEnums::AnalogInPinNumber::PinAI3:      chan_num = AI3_IO;      break;
    case logiBUSIOEnums::AnalogInPinNumber::PinAI4:      chan_num = AI4_IO;      break;
    case logiBUSIOEnums::AnalogInPinNumber::PinAI5:      chan_num = AI5_IO;      break;
    case logiBUSIOEnums::AnalogInPinNumber::PinAI6:      chan_num = AI6_IO;      break;
    case logiBUSIOEnums::AnalogInPinNumber::PinAI7:      chan_num = AI7_IO;      break;
    case logiBUSIOEnums::AnalogInPinNumber::PinAI8:      chan_num = AI8_IO;      break;
    default:      break;
  }

  if (chan_num < SOC_ADC_CHANNEL_NUM(ADC_UNIT))  {
    channel[channel_num] = chan_num;
    channel_num++;
    IOHandleESP32AnalogIN::mCallbackDirectory[chan_num] = this;
  }
}



void IOHandleESP32AnalogIN::get(CIEC_ANY &paState) {
  static_cast<CIEC_DWORD &>(paState) = CIEC_DWORD(this->newData);
}

IOHandleESP32AnalogIN::~IOHandleESP32AnalogIN() {

  IOHandleESP32AnalogIN::mCallbackDirectory[chan_num] = nullptr;
  DEVLOG_DEBUG("IOHandleESP32AnalogIN unregistered");

}

void IOHandleESP32AnalogIN::set(const CIEC_ANY &paState) {
}


bool IRAM_ATTR IOHandleESP32AnalogIN::s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

    return (mustYield == pdTRUE);
}




void IOHandleESP32AnalogIN::continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle) {

    adc_continuous_handle_t handle = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size = READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 20 * 1000,
        .conv_mode = ADC_CONV_MODE,
        .format = ADC_OUTPUT_TYPE,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num = channel_num;
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten = ADC_ATTEN;
        adc_pattern[i].channel = channel[i] & 0x7;
        adc_pattern[i].unit = ADC_UNIT;
        adc_pattern[i].bit_width = ADC_BIT_WIDTH;

        ESP_LOGI(TAG, "adc_pattern[%d].atten is :%" PRIx8 " ", i, adc_pattern[i].atten);
        ESP_LOGI(TAG, "adc_pattern[%d].channel is :%" PRIx8 " ", i, adc_pattern[i].channel);
        ESP_LOGI(TAG, "adc_pattern[%d].unit is :%" PRIx8 " ", i, adc_pattern[i].unit);
    }
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));

    *out_handle = handle;
}

uint32_t calcDiff(uint32_t a, uint32_t b) {
  return (a > b) ? (a - b) : (b - a);
}



void IOHandleESP32AnalogIN::continuous_adc_task( void * pvParameters )
{
    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[READ_LEN] = {0};
    memset(result, 0xcc, READ_LEN);

    s_task_handle = xTaskGetCurrentTaskHandle();

    adc_continuous_handle_t handle = NULL;
    continuous_adc_init(channel, channel_num, &handle);

    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = s_conv_done_cb,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));


    has_to_stop = false;

    ESP_LOGW(TAG, "Test task started");

    while( !has_to_stop ) {

        /**
         * This is to show you the way to use the ADC continuous mode driver event callback.
         * This `ulTaskNotifyTake` will block when the data processing in the task is fast.
         * However in this example, the data processing (print) is slow, so you barely block here.
         *
         * Without using this event callback (to notify this task), you can still just call
         * `adc_continuous_read()` here in a loop, with/without a certain block timeout.
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        char unit[] = ADC_UNIT_STR(ADC_UNIT);

        while( !has_to_stop ) {
            ret = adc_continuous_read(handle, result, READ_LEN, &ret_num, 0);
            if (ret == ESP_OK) {
                ESP_LOGD("TASK", "ret is %x, ret_num is %" PRIu32 " bytes", ret, ret_num);
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
                    adc_digi_output_data_t *p = (adc_digi_output_data_t *)&result[i];
                    uint32_t chan_num = ADC_GET_CHANNEL(p);
                    uint32_t data = ADC_GET_DATA(p);
                    /* Check the channel number validation, the data is invalid if the channel num exceed the maximum channel */
                    if (chan_num < SOC_ADC_CHANNEL_NUM(ADC_UNIT)) {
                      if(mCallbackDirectory[chan_num]) {
                        mCallbackDirectory[chan_num]->sample_num++;
                        mCallbackDirectory[chan_num]->adc_reading += data;
                      }
                        ESP_LOGV(TAG, "Unit: %s, Channel: %" PRIu32 ", Value: %" PRIx32 "   int %" PRIu32 "   ", unit, chan_num, data, data);
                    } else {
                        ESP_LOGW(TAG, "Invalid data [%s_%" PRIu32 "_%" PRIx32 "]", unit, chan_num, data);
                    }
                }
                for (int chan_num = 0; chan_num < SOC_ADC_CHANNEL_NUM(ADC_UNIT); ++chan_num) {
                  if(mCallbackDirectory[chan_num]) {
                    mCallbackDirectory[chan_num]->newData = mCallbackDirectory[chan_num]->adc_reading / mCallbackDirectory[chan_num]->sample_num;
                    uint32_t diff = calcDiff(mCallbackDirectory[chan_num]->newData, mCallbackDirectory[chan_num]->oldData);
                    if(mCallbackDirectory[chan_num]->hysteresis) {
                      if (diff > mCallbackDirectory[chan_num]->hysteresis) {
                        mCallbackDirectory[chan_num]->oldData = mCallbackDirectory[chan_num]->newData;
                        mCallbackDirectory[chan_num]->onChange();
                      }
                    }
                    ESP_LOGV(TAG, "AVG Unit: %s, Channel: %" PRIu32 ", Value: %" PRIx32 "   int %" PRIu32 "   ", unit, chan_num, mCallbackDirectory[chan_num]->newData, mCallbackDirectory[chan_num]->newData);
                    mCallbackDirectory[chan_num]->sample_num = 0;
                    mCallbackDirectory[chan_num]->adc_reading = 0;
                  }
                }
                /**
                 * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will immediately return.
                 * To avoid a task watchdog timeout, add a delay here. When you replace the way you process the data,
                 * usually you don't need this delay (as this task will block for a while).
                 */
                vTaskDelay(1);
            } else if (ret == ESP_ERR_TIMEOUT) {
                //We try to read `READ_LEN` until API returns timeout, which means there's no available data
                break;
            }
        }
    }

    ESP_ERROR_CHECK(adc_continuous_stop(handle));
    ESP_ERROR_CHECK(adc_continuous_deinit(handle));
    ESP_LOGW(TAG, "Test task ended");
    vTaskDelete( NULL );
}




void IOHandleESP32AnalogIN::startTask() {
  /* create thread */
  has_to_stop = false;
  BaseType_t xReturned = xTaskCreate(continuous_adc_task, "continuous_adc_task", 4096, nullptr, scmTaskPriority, nullptr);
}

void IOHandleESP32AnalogIN::stopTask() {
  has_to_stop = true;
}


