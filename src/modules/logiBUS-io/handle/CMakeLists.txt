#*******************************************************************************
# Copyright (c) 2025 HR Agrartechnik GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Franz Hoepfinger - initial API and implementation and/or initial documentation
# *******************************************************************************/ 

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})



# *******************************************************************************/
if(FORTE_MODULE_ESP32_DIGITAL_OUT)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/esp32_digital_out)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/esp32_digital_out/digital_out)
  forte_add_sourcefile_hcpp(esp32_digital_out/IOHandleESP32DigitalO)
  forte_add_sourcefile_hcpp(esp32_digital_out/IOHandleESP32PWMO)
  forte_add_sourcefile_hcpp(esp32_digital_out/IOHandleESP32ServoO)
  forte_add_sourcefile_hcpp(esp32_digital_out/IOHandleESP32LEDO_GPIO_MODE)
  forte_add_sourcefile_hcpp(esp32_digital_out/IOHandleESP32LEDO_LEDC_MODE)
endif(FORTE_MODULE_ESP32_DIGITAL_OUT)  
# *******************************************************************************/
if(FORTE_MODULE_ESP_ADDONS)
  forte_add_sourcefile_hcpp(esp32_digital_out/IOHandleESP32LEDO_STRIPS_MODE)
endif(FORTE_MODULE_ESP_ADDONS)  
# *******************************************************************************/
if(FORTE_MODULE_ESP32_DIGITAL_IN)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/esp32_digital_in)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/esp32_digital_in/digital_in)
  forte_add_sourcefile_hcpp(esp32_digital_in/button_handle)
  forte_add_sourcefile_hcpp(esp32_digital_in/IOHandleESP32Button)
  forte_add_sourcefile_hcpp(esp32_digital_in/IOHandleESP32Button_DOWN)
  forte_add_sourcefile_hcpp(esp32_digital_in/IOHandleESP32Button_UP)
  forte_add_sourcefile_hcpp(esp32_digital_in/IOHandleESP32Button_REPEAT)
  forte_add_sourcefile_hcpp(esp32_digital_in/IOHandleESP32Button_SINGLE_CLICK)
  forte_add_sourcefile_hcpp(esp32_digital_in/IOHandleESP32Button_DOUBLE_CLICK)
  forte_add_sourcefile_hcpp(esp32_digital_in/IOHandleESP32Button_LONG_PRESS_START)
  forte_add_sourcefile_hcpp(esp32_digital_in/IOHandleESP32Button_LONG_PRESS_HOLD)
endif(FORTE_MODULE_ESP32_DIGITAL_IN) 
# *******************************************************************************/
if(FORTE_MODULE_ESP32_ANALOG_IN)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/esp32_analog_in)
  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/esp32_analog_in/analog_in)
  forte_add_sourcefile_hcpp(esp32_analog_in/IOHandleESP32AnalogIN)  
endif(FORTE_MODULE_ESP32_ANALOG_IN) 
# *******************************************************************************/

# *******************************************************************************/

