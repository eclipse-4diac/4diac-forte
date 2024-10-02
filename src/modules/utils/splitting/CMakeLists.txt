#*******************************************************************************
# Copyright (c) 2023 HR Agrartechnik GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors: 
#     Franz Hoepfinger - initial API and implementation and/or initial documentation
# *******************************************************************************/ 
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#######################################################
# assembling
#######################################################

forte_add_sourcefile_hcpp(SPLIT_BYTE_INTO_BOOLS_fct)
forte_add_sourcefile_hcpp(SPLIT_BYTE_INTO_QUARTERS_fct)
forte_add_sourcefile_hcpp(SPLIT_DWORD_INTO_BOOLS_fct)
forte_add_sourcefile_hcpp(SPLIT_DWORD_INTO_QUARTERS_fct)
forte_add_sourcefile_hcpp(SPLIT_DWORD_INTO_WORDS_fct)
forte_add_sourcefile_hcpp(SPLIT_LWORD_INTO_BOOLS_fct)
forte_add_sourcefile_hcpp(SPLIT_LWORD_INTO_QUARTERS_fct)
forte_add_sourcefile_hcpp(SPLIT_WORD_INTO_BOOLS_fct)
forte_add_sourcefile_hcpp(SPLIT_WORD_INTO_BYTES_fct)
forte_add_sourcefile_hcpp(SPLIT_WORD_INTO_QUARTERS_fct)