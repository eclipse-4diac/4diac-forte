# *******************************************************************************
# Copyright (c) 2025 HR Agrartechnik GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Franz Höpfinger - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# signals
# ############################################################################
add_library(forte-modules-utils-signals OBJECT
            E_BLINK_fbt.h E_BLINK_fbt.cpp E_BLINK_TRAIN_fbt.h
            E_BLINK_TRAIN_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-utils-signals)
