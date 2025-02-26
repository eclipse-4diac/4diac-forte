###############################################################################
# Copyright (c) 2021, 2022 Jonathan Lainer
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Jonathan Lainer - Initial contribution.
###############################################################################

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)

SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(FORTE_BUILD_EXECUTABLE OFF)
SET(FORTE_BUILD_STATIC_LIBRARY ON)

# NOTE: This path should work fine for a linux system installation.
#       If your toolchain is not installed to this path or 
#       you are not using linux, change the path accordingly.
SET(TOOLCHAIN_BIN "/usr/bin" CACHE PATH "Toolchain Path")

SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN}/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN}/arm-none-eabi-g++)

# NOTE: Check that the float-abi matches your application!
SET(CMAKE_C_FLAGS "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -specs=nosys.specs")
SET(CMAKE_CXX_FLAGS "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -specs=nosys.specs")

SET(FORTE_ARCHITECTURE "FreeRTOSLwIP")

SET(TARGET_PROJECT_ROOT "/please/change/me" CACHE PATH "Project Path")

SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Core/Inc)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/LWIP/Target)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Inc)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Drivers/CMSIS/Include)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Drivers/CMSIS/Device/ST/STM32H7xx/Include)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/include)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/portable)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Middlewares/Third_Party/LwIP/system)
SET_PROPERTY(GLOBAL APPEND PROPERTY FORTE_INCLUDE_DIRECTORIES ${TARGET_PROJECT_ROOT}/Middlewares/Third_Party/LwIP/src/include)

# NOTE: If you use a board other than the eLITe-board, change the definition to match your processor.
ADD_DEFINITIONS(-DSTM32H743xx)
