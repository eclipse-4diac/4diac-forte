# *******************************************************************************
# Copyright (c) 2011 - 2013 ACIN, Profacor GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger, Matthias Plasch *   - initial API and
# implementation and/or initial documentation
# *******************************************************************************/

# ############################################################################
# Conversion
# ############################################################################
add_subdirectory(BOOL)
add_subdirectory(BYTE)
add_subdirectory(DINT)
add_subdirectory(DWORD)
add_subdirectory(INT)
add_subdirectory(SINT)
add_subdirectory(STRING)
add_subdirectory(TIME)
add_subdirectory(UDINT)
add_subdirectory(UINT)
add_subdirectory(USINT)
add_subdirectory(WORD)
add_subdirectory(BCD)
add_subdirectory(WSTRING)
add_subdirectory(REAL)
add_subdirectory(LREAL)
add_subdirectory(ULINT)
add_subdirectory(LWORD)
add_subdirectory(LINT)
add_subdirectory(DT)
target_sources(forte-modules-IEC61131-3 PRIVATE
            F_ANY_AS_STRING_fbt.h F_ANY_AS_STRING_fbt.cpp
)
