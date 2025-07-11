# *******************************************************************************
# Copyright (c) 2011 - 2018, 2024 AIT, ACIN, Profactor GmbH, fortiss GmbH This
# program and the accompanying materials are made available under the terms of
# the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Thomas Strasser, Alois Zoitl, Matthias Plasch, Ben Schneider *
# - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_register_module(UTILS OFF "FORTE UTILITY FBs" FORTE_MODULE_IEC61131)

# ############################################################################
# FORTE UTILITY FBs
# ############################################################################
add_subdirectory(selection)
add_subdirectory(const)
add_subdirectory(assembling)
add_subdirectory(splitting)
add_subdirectory(timing)
add_subdirectory(signals)
add_library(forte-modules-utils OBJECT
            GetInstancePath_fbt.h
            GetInstancePath_fbt.cpp
            GetInstancePathAndName_fbt.h
            GetInstancePathAndName_fbt.cpp
            OUT_ANY_CONSOLE_fbt.h
            OUT_ANY_CONSOLE_fbt.cpp
            GEN_CSV_WRITER_fbt.h
            GEN_CSV_WRITER_fbt.cpp
            GEN_ARRAY2VALUES_fbt.h
            GEN_ARRAY2VALUES_fbt.cpp
            GEN_VALUES2ARRAY_fbt.h
            GEN_VALUES2ARRAY_fbt.cpp
            GEN_ARRAY2ARRAY_fbt.h
            GEN_ARRAY2ARRAY_fbt.cpp
            GET_AT_INDEX_fbt.h
            GET_AT_INDEX_fbt.cpp
            SET_AT_INDEX_fbt.h
            SET_AT_INDEX_fbt.cpp
            FB_RANDOM_fbt.h
            FB_RANDOM_fbt.cpp
            GET_STRUCT_VALUE_fbt.h
            GET_STRUCT_VALUE_fbt.cpp
            SET_STRUCT_VALUE_fbt.h
            SET_STRUCT_VALUE_fbt.cpp
            STEST_END_fbt.h
            STEST_END_fbt.cpp
            TEST_CONDITION_fbt.h
            TEST_CONDITION_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-modules-utils)
