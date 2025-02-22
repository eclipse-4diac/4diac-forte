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
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)

# *******************************************************************************/
if(FORTE_MODULE_ESP32_ANALOG_IN)
  forte_add_sourcefile_hcpp(logiBUS_8_AI_fbt)  
endif(FORTE_MODULE_ESP32_ANALOG_IN) 
# *******************************************************************************/
if(FORTE_MODULE_ESP32_DIGITAL_IN)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_DOUBLE_CLICK_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_DOWN_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_END_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_LONG_PRESS_HOLD_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_LONG_PRESS_START_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_LONG_PRESS_UP_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_MULTIPLE_CLICK_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_REPEAT_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_SINGLE_CLICK_fbt)
  forte_add_sourcefile_hcpp(logiBUS_8_DI_UP_fbt)
endif(FORTE_MODULE_ESP32_DIGITAL_IN)
# *******************************************************************************/
if(FORTE_MODULE_ESP32_DIGITAL_OUT)
  # *******************************************************************************/
  forte_add_sourcefile_hcpp(logiBUS_8_DO_fbt)
  # TODO DO_PWM
  # *******************************************************************************/
  forte_add_sourcefile_hcpp(logiBUS_8_PWM_fbt)
  forte_add_sourcefile_hcpp(logiBUS_2_SERVO_fbt)
  forte_add_sourcefile_hcpp(logiBUS_1_LED_PWM_fbt)
  forte_add_sourcefile_hcpp(logiBUS_1_LED_DO_fbt)
  forte_add_sourcefile_hcpp(logiBUS_1_LED_strip_fbt)
  # *******************************************************************************/
  forte_add_sourcefile_hcpp(LED_DO_gcf)
  # *******************************************************************************/
  forte_add_sourcefile_hcpp(LED_strip_gcf)
  forte_add_sourcefile_hcpp(LED_COLOURS_gcf)
  # *******************************************************************************/
endif(FORTE_MODULE_ESP32_DIGITAL_OUT)  
# *******************************************************************************/