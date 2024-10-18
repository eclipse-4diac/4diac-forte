#*******************************************************************************
# Copyright (c) 2023, 2024 HR Agrartechnik GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors: 
#     Franz Hoepfinger - initial API and implementation and/or initial documentation
# *******************************************************************************/ 
 
forte_add_module(SIGNALPROCESSING OFF "SIGNALPROCESSING")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#######################################################
# SIGNALPROCESSING
#######################################################


forte_add_sourcefile_hcpp(DualHysteresis_fbt)
forte_add_sourcefile_hcpp(FIELDBUS_WORD_TO_PERCENT_fct)
forte_add_sourcefile_hcpp(FIELDBUS_PERCENT_TO_WORD_fct)
forte_add_sourcefile_hcpp(FIELDBUS_SIGNAL_gcf)
forte_add_sourcefile_hcpp(SCALE_fct)
forte_add_sourcefile_hcpp(SCALE_LIM_fct)
forte_add_sourcefile_hcpp(RampLimitFS_fbt)
forte_add_subdirectory(distance)