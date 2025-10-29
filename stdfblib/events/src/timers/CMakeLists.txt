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

target_sources(forte-events PRIVATE
        E_PULSE_fbt.cpp
        E_TOF_fbt.cpp
        E_TONOF_fbt.cpp
        E_TON_fbt.cpp
        E_TP_fbt.cpp
)