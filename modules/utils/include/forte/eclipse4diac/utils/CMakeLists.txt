#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

target_sources(forte-utils PUBLIC
        FILE_SET HEADERS
        FILES
        FB_RANDOM_fbt.h
        GEN_CSV_WRITER_fbt.h
        GetInstancePath_fbt.h
        GetInstancePathAndName_fbt.h
        OUT_ANY_CONSOLE_fbt.h
        STEST_END_fbt.h
        TEST_CONDITION_fbt.h
)

add_subdirectory(assembling)
add_subdirectory(const)
add_subdirectory(selection)
add_subdirectory(signals)
add_subdirectory(splitting)
add_subdirectory(timing)
