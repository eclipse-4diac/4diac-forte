#*******************************************************************************
# Copyright (c) 2013 ACIN
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
# CharacterString
#############################################################################
forte_add_sourcefile_hcpp(F_LEN_fbt)
forte_add_sourcefile_hcpp(F_LEFT_fbt)
forte_add_sourcefile_hcpp(F_RIGHT_fbt)
forte_add_sourcefile_hcpp(F_MID_fbt)
forte_add_sourcefile_hcpp(F_CONCAT_fbt)
forte_add_sourcefile_hcpp(F_INSERT_fbt)
forte_add_sourcefile_hcpp(F_DELETE_fbt)
forte_add_sourcefile_hcpp(F_REPLACE_fbt)
forte_add_sourcefile_hcpp(F_FIND_fbt)

forte_add_sourcefile_hcpp(F_CONCAT_DATE_TOD_fbt)
