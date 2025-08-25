# *******************************************************************************
# Copyright (c) 2023, 2024 HR Agrartechnik GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Franz Hoepfinger - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

forte_register_module(SIGNALPROCESSING OFF "SIGNALPROCESSING")

# ############################################################################
# SIGNALPROCESSING
# ############################################################################
add_library(forte-modules-signalprocessing OBJECT
            DualHysteresis_fbt.h
            DualHysteresis_fbt.cpp
            FIELDBUS_WORD_TO_PERCENT_fct.h
            FIELDBUS_WORD_TO_PERCENT_fct.cpp
            FIELDBUS_PERCENT_TO_WORD_fct.h
            FIELDBUS_PERCENT_TO_WORD_fct.cpp
            FIELDBUS_SIGNAL_gcf.h
            FIELDBUS_SIGNAL_gcf.cpp
            SCALE_fct.h
            SCALE_fct.cpp
            SCALE_LIM_fct.h
            SCALE_LIM_fct.cpp
            RampLimitFS_fbt.h
            RampLimitFS_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-signalprocessing)

add_subdirectory(distance)
