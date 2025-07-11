# *******************************************************************************
# Copyright (c) 2013 ACIN This program and the accompanying materials are made
# available under the terms of the Eclipse Public License 2.0 which is
# available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# CharacterString
# ############################################################################
add_library(forte-modules-IEC61131-3-CharacterString OBJECT
            F_LEN_fbt.h
            F_LEN_fbt.cpp
            F_LEFT_fbt.h
            F_LEFT_fbt.cpp
            F_RIGHT_fbt.h
            F_RIGHT_fbt.cpp
            F_MID_fbt.h
            F_MID_fbt.cpp
            F_CONCAT_fbt.h
            F_CONCAT_fbt.cpp
            F_INSERT_fbt.h
            F_INSERT_fbt.cpp
            F_DELETE_fbt.h
            F_DELETE_fbt.cpp
            F_REPLACE_fbt.h
            F_REPLACE_fbt.cpp
            F_FIND_fbt.h
            F_FIND_fbt.cpp
            F_CONCAT_DATE_TOD_fbt.h
            F_CONCAT_DATE_TOD_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-CharacterString)
