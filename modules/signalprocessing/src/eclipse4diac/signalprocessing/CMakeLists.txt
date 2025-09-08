#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

target_sources(forte-signalprocessing PRIVATE
        DualHysteresis_fbt.cpp
        FIELDBUS_PERCENT_TO_WORD_fct.cpp
        FIELDBUS_SIGNAL_gcf.cpp
        FIELDBUS_WORD_TO_PERCENT_fct.cpp
        RampLimitFS_fbt.cpp
        SCALE_fct.cpp
        SCALE_LIM_fct.cpp
)
