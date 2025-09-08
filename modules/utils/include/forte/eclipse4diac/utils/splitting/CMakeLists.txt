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
        SPLIT_BYTE_INTO_BOOLS_fct.h
        SPLIT_BYTE_INTO_QUARTERS_fct.h
        SPLIT_DWORD_INTO_BOOLS_fct.h
        SPLIT_DWORD_INTO_QUARTERS_fct.h
        SPLIT_DWORD_INTO_WORDS_fct.h
        SPLIT_LWORD_INTO_BOOLS_fct.h
        SPLIT_LWORD_INTO_QUARTERS_fct.h
        SPLIT_WORD_INTO_BOOLS_fct.h
        SPLIT_WORD_INTO_BYTES_fct.h
        SPLIT_WORD_INTO_QUARTERS_fct.h
)
