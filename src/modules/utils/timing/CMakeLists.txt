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
# timing
# ############################################################################
target_sources(forte-modules-utils PRIVATE
            F_NOW_fct.h
            F_NOW_fct.cpp
            F_NOW_MONOTONIC_fct.h
            F_NOW_MONOTONIC_fct.cpp
            E_STOPWATCH_fbt.h
            E_STOPWATCH_fbt.cpp
            TIMESTAMP_NS_fct.h
            TIMESTAMP_NS_fct.cpp
)
