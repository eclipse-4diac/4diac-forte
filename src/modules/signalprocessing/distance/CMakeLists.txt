# *******************************************************************************
# Copyright (c) 2024 HR Agrartechnik GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Franz Höpfinger - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# Distance
# ############################################################################
add_library(forte-modules-signalprocessing-distance OBJECT
            RangeBasedPulse_fbt.h RangeBasedPulse_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-signalprocessing-distance)
