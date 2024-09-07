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
forte_add_sourcefile_hcpp(FB_CTD_DINT_fbt)
forte_add_sourcefile_hcpp(FB_CTD_UDINT_fbt)
forte_add_sourcefile_hcpp(FB_CTD_fbt)
forte_add_sourcefile_hcpp(FB_CTUD_DINT_fbt)
forte_add_sourcefile_hcpp(FB_CTU_UDINT_fbt)
forte_add_sourcefile_hcpp(FB_CTU_fbt)
forte_add_sourcefile_hcpp(FB_CTU_DINT_fbt)
forte_add_sourcefile_hcpp(FB_CTUD_fbt)
forte_add_sourcefile_hcpp(FB_CTD_LINT_fbt)
forte_add_sourcefile_hcpp(FB_CTD_ULINT_fbt)
forte_add_sourcefile_hcpp(FB_CTU_LINT_fbt)
forte_add_sourcefile_hcpp(FB_CTU_ULINT_fbt)
forte_add_sourcefile_hcpp(FB_CTUD_LINT_fbt)
forte_add_sourcefile_hcpp(FB_CTUD_ULINT_fbt)
