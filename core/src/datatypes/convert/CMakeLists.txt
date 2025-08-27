# ############################################################################
# Copyright (c) 2010 - 2014 Profactor GmbH, ACIN, fortiss GmbH
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Michael Hofmann, Alois Zoitl, Monika Wenger, Gerhard Ebenhofer
# - initial implementation and rework communication infrastructure
# ############################################################################
# ############################################################################
# Core Datatypes
# ############################################################################

target_sources(forte-core PRIVATE
        DateAndTimeToConvertFunctions.cpp
        LDateAndTimeToConvertFunctions.cpp
)
