# *******************************************************************************
# Copyright (c) 2011 - 2014 ACIN, Profactor GmbH This program and the
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
# BitwiseOperators
# ############################################################################
add_library(forte-modules-IEC61131-3-BitwiseOperators OBJECT
            F_AND_fbt.h
            F_AND_fbt.cpp
            F_NOT_fbt.h
            F_NOT_fbt.cpp
            F_OR_fbt.h
            F_OR_fbt.cpp
            F_XOR_fbt.h
            F_XOR_fbt.cpp
            genbitbase_fbt.h
            genbitbase_fbt.cpp
            GEN_AND_fbt.h
            GEN_AND_fbt.cpp
            GEN_OR_fbt.h
            GEN_OR_fbt.cpp
            GEN_XOR_fbt.h
            GEN_XOR_fbt.cpp
            F_SHL_fbt.h
            F_SHL_fbt.cpp
            F_SHR_fbt.h
            F_SHR_fbt.cpp
            F_ROR_fbt.h
            F_ROR_fbt.cpp
            F_ROL_fbt.h
            F_ROL_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-BitwiseOperators)
