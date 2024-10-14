#*******************************************************************************
# Copyright (c) 2024 HR Agrartechnik GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Franz Höpfinger - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#############################################################################
# timing
#############################################################################

forte_add_sourcefile_hcpp(F_NOW_fct)
forte_add_sourcefile_hcpp(F_NOW_MONOTONIC_fct)
forte_add_sourcefile_hcpp(E_STOPWATCH_fbt)
forte_add_sourcefile_hcpp(TIMESTAMP_NS_fct)
