#*******************************************************************************
# Copyright (c) 2011 - 2012 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Monika Wenger
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#############################################################################
# Counters
#############################################################################
forte_add_sourcefile_hcpp(FB_CTD_DINT)
forte_add_sourcefile_hcpp(FB_CTD_UDINT)
forte_add_sourcefile_hcpp(FB_CTD)
forte_add_sourcefile_hcpp(FB_CTUD_DINT)
forte_add_sourcefile_hcpp(FB_CTU_UDINT)
forte_add_sourcefile_hcpp(FB_CTU)
forte_add_sourcefile_hcpp(FB_CTU_DINT)
forte_add_sourcefile_hcpp(FB_CTUD)
forte_add_sourcefile_hcpp(FB_CTD_LINT)
forte_add_sourcefile_hcpp(FB_CTD_ULINT)
forte_add_sourcefile_hcpp(FB_CTU_LINT)
forte_add_sourcefile_hcpp(FB_CTU_ULINT)
forte_add_sourcefile_hcpp(FB_CTUD_LINT)
forte_add_sourcefile_hcpp(FB_CTUD_ULINT)
