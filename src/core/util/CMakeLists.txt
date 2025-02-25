#*******************************************************************************
# Copyright (c) 2011 - 2015 ACIN, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl
# *    - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# core/util
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_h(singlet.h criticalregion.h)
forte_add_sourcefile_h(fortearray.h inplace_vector.h)
forte_add_sourcefile_h(ringbuf.h)

forte_add_sourcefile_hcpp(string_utils parameterParser configFileParser mixedStorage ifSpecBuilder)
