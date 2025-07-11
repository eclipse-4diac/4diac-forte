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
# Conversion when bit-string variable contains data encoded in BCD x_BCD_TO_xx
# xx_TO_BCD_x
# ############################################################################

add_library(forte-modules-IEC61131-3-Conversion-BCD OBJECT
            F_BYTE_BCD_TO_USINT_fbt.h
            F_BYTE_BCD_TO_USINT_fbt.cpp
            F_WORD_BCD_TO_UINT_fbt.h
            F_WORD_BCD_TO_UINT_fbt.cpp
            F_DWORD_BCD_TO_UDINT_fbt.h
            F_DWORD_BCD_TO_UDINT_fbt.cpp
            F_USINT_TO_BCD_BYTE_fbt.h
            F_USINT_TO_BCD_BYTE_fbt.cpp
            F_UINT_TO_BCD_WORD_fbt.h
            F_UINT_TO_BCD_WORD_fbt.cpp
            F_UDINT_TO_BCD_DWORD_fbt.h
            F_UDINT_TO_BCD_DWORD_fbt.cpp
            F_LWORD_BCD_TO_ULINT_fbt.h
            F_LWORD_BCD_TO_ULINT_fbt.cpp
            F_ULINT_TO_BCD_LWORD_fbt.h
            F_ULINT_TO_BCD_LWORD_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-Conversion-BCD)
