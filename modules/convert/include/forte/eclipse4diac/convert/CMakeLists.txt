#############################################################################
# Copyright (c) 2025 Martin Erich Jobst, Andreas Demmler Fahrzeugbau
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
#    Franz Höpfinger
#      - add providers
#############################################################################

# This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604012115!

target_sources(forte-convert PUBLIC
        FILE_SET HEADERS
        FILES
        GEN_ARRAY2ARRAY_fbt.h
        GEN_ARRAY2VALUES_fbt.h
        GEN_STRUCT_DEMUX_fbt.h
        GEN_STRUCT_MUX_fbt.h
        GEN_VALUES2ARRAY_fbt.h
        GET_AT_INDEX_fbt.h
        GET_STRUCT_VALUE_fbt.h
        SET_AT_INDEX_fbt.h
        SET_STRUCT_VALUE_fbt.h
)
add_subdirectory(providers)