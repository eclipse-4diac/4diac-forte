# *******************************************************************************
# Copyright (c) 2012 - 2013 ACIN, 2018 TU Vienna/ACIN, 2019 TU Wien/ACIN This
# program and the accompanying materials are made available under the terms of
# the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *   - initial API and implementation and/or
# initial documentation *   Martin Melik-Merkumians - removing illegal casts *
# Martin Melik-Merkumians - adding TIME IN RESOLUTION TO FBs
# *******************************************************************************/

# ############################################################################
# TIME_TO_xxx Conversion
# ############################################################################

add_library(forte-modules-IEC61131-3-Conversion-TIME OBJECT
            F_TIME_AS_STRING_fbt.h
            F_TIME_AS_STRING_fbt.cpp
            F_TIME_IN_S_TO_LINT_fbt.h
            F_TIME_IN_S_TO_LINT_fbt.cpp
            F_TIME_IN_MS_TO_LINT_fbt.h
            F_TIME_IN_MS_TO_LINT_fbt.cpp
            F_TIME_IN_US_TO_LINT_fbt.h
            F_TIME_IN_US_TO_LINT_fbt.cpp
            F_TIME_IN_NS_TO_LINT_fbt.h
            F_TIME_IN_NS_TO_LINT_fbt.cpp
            F_TIME_IN_S_TO_DINT_fbt.h
            F_TIME_IN_S_TO_DINT_fbt.cpp
            F_TIME_IN_MS_TO_DINT_fbt.h
            F_TIME_IN_MS_TO_DINT_fbt.cpp
            F_TIME_IN_US_TO_DINT_fbt.h
            F_TIME_IN_US_TO_DINT_fbt.cpp
            F_TIME_IN_NS_TO_DINT_fbt.h
            F_TIME_IN_NS_TO_DINT_fbt.cpp
            F_TIME_IN_S_TO_ULINT_fbt.h
            F_TIME_IN_S_TO_ULINT_fbt.cpp
            F_TIME_IN_MS_TO_ULINT_fbt.h
            F_TIME_IN_MS_TO_ULINT_fbt.cpp
            F_TIME_IN_US_TO_ULINT_fbt.h
            F_TIME_IN_US_TO_ULINT_fbt.cpp
            F_TIME_IN_NS_TO_ULINT_fbt.h
            F_TIME_IN_NS_TO_ULINT_fbt.cpp
            F_TIME_IN_S_TO_UDINT_fbt.h
            F_TIME_IN_S_TO_UDINT_fbt.cpp
            F_TIME_IN_MS_TO_UDINT_fbt.h
            F_TIME_IN_MS_TO_UDINT_fbt.cpp
            F_TIME_IN_US_TO_UDINT_fbt.h
            F_TIME_IN_US_TO_UDINT_fbt.cpp
            F_TIME_IN_NS_TO_UDINT_fbt.h
            F_TIME_IN_NS_TO_UDINT_fbt.cpp
            F_TIME_IN_S_TO_LREAL_fbt.h
            F_TIME_IN_S_TO_LREAL_fbt.cpp
            F_TIME_IN_MS_TO_LREAL_fbt.h
            F_TIME_IN_MS_TO_LREAL_fbt.cpp
            F_TIME_IN_US_TO_LREAL_fbt.h
            F_TIME_IN_US_TO_LREAL_fbt.cpp
            F_TIME_IN_NS_TO_LREAL_fbt.h
            F_TIME_IN_NS_TO_LREAL_fbt.cpp
            F_TIME_AS_WSTRING_fbt.h
            F_TIME_AS_WSTRING_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-Conversion-TIME)
