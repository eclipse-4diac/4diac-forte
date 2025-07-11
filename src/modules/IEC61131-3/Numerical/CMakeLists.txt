# *******************************************************************************
# Copyright (c) 2011 - 2013 ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# Numerical
# ############################################################################

add_library(forte-modules-IEC61131-3-Numerical OBJECT
            F_ABS_fbt.h
            F_ABS_fbt.cpp
            F_SQRT_fbt.h
            F_SQRT_fbt.cpp
            F_LN_fbt.h
            F_LN_fbt.cpp
            F_LOG_fbt.h
            F_LOG_fbt.cpp
            F_SIN_fbt.h
            F_SIN_fbt.cpp
            F_COS_fbt.h
            F_COS_fbt.cpp
            F_TAN_fbt.h
            F_TAN_fbt.cpp
            F_ASIN_fbt.h
            F_ASIN_fbt.cpp
            F_ACOS_fbt.h
            F_ACOS_fbt.cpp
            F_ATAN_fbt.h
            F_ATAN_fbt.cpp
            F_EXP_fbt.h
            F_EXP_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-IEC61131-3-Numerical)
