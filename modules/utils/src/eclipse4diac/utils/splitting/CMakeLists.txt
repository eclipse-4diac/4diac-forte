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

target_sources(forte-utils PRIVATE
        SPLIT_BYTE_INTO_BOOLS_fct.cpp
        SPLIT_BYTE_INTO_QUARTERS_fct.cpp
        SPLIT_DWORD_INTO_BOOLS_fct.cpp
        SPLIT_DWORD_INTO_QUARTERS_fct.cpp
        SPLIT_DWORD_INTO_WORDS_fct.cpp
        SPLIT_LWORD_INTO_BOOLS_fct.cpp
        SPLIT_LWORD_INTO_QUARTERS_fct.cpp
        SPLIT_WORD_INTO_BOOLS_fct.cpp
        SPLIT_WORD_INTO_BYTES_fct.cpp
        SPLIT_WORD_INTO_QUARTERS_fct.cpp
)
