# ############################################################################
# Copyright (c) 2024 KT Elektronik GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Dirk Kaar - initial API and implementation and/or initial
# documentation
# ############################################################################

add_library(forte-modules-zephyr-handle OBJECT
            IOHandleGPIO.h IOHandleGPIO.cpp IOHandleADC.h IOHandleADC.cpp
            IOHandlePWM.h IOHandlePWM.cpp
)
target_link_libraries(forte PRIVATE forte-modules-zephyr-handle)
