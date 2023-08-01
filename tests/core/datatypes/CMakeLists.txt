#*******************************************************************************
# Copyright (c) 2011 - 2014 nxtControl, ACIN, fortiss GmbH, Profactor GmbH
#               2020 Johannes Kepler University Linz
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Stanislav Meduna, Ingo Hegny, Alois Zoitl, Monika Wenger, Matthias Plasch  - initial API and implementation and/or initial documentation
#    Ernst Blecha - add multibit partial access, add at-variable tests
# *******************************************************************************/

#SET(SOURCE_GROUP ${SOURCE_GROUP}\\core)

  forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})

  forte_test_add_sourcefile_cpp(CIEC_ANY_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_ELEMENTARY_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_MAGNITUDE_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_NUM_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_REAL_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_INT_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_SIGNED_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_UNSIGNED_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_DURATION_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_BIT_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_CHARS_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_STRING_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_CHAR_VARIANT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ANY_DATE_VARIANT_test.cpp)

#Unicode utilities
  forte_test_add_sourcefile_cpp(CUnicodeUtilities_test.cpp)

#BOOLEAN Datatype  
  forte_test_add_sourcefile_cpp(CIEC_BOOL_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_BOOL_CASTS_test.cpp)
  
#BIT-Datatypes  
  forte_test_add_sourcefile_cpp(CIEC_BYTE_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_BYTE_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_WORD_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_WORD_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DWORD_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DWORD_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LWORD_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LWORD_CASTS_test.cpp)
  forte_test_add_subdirectory(CIEC_PARTIAL)

#INT-Datatypes
  forte_test_add_sourcefile_cpp(CIEC_SINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_SINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_USINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_USINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_INT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_INT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_UINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_UINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_UDINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_UDINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ULINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ULINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_REAL_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_REAL_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LREAL_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LREAL_CASTS_test.cpp)

#ANY_CHARS-Datatypes
  forte_test_add_sourcefile_cpp(CIEC_CHAR_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_CHAR_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_WCHAR_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_WCHAR_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_STRING_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_STRING_FIXED_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_WSTRING_test.cpp)

#ARRAY-Datatypes
  forte_test_add_sourcefile_cpp(CIEC_ARRAY_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ARRAY_TEMPLATE_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_TIME_STRUCT_AND_ARRAY_test.cpp)

#ANY_DURATION and ANY_DATE-Datatypes
  forte_test_add_sourcefile_cpp(CIEC_TIME_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DATE_AND_TIME_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DATE_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_TIME_OF_DAY_test.cpp)
  
  forte_test_add_sourcefile_cpp(CIEC_LTIME_test.cpp)
  
forte_test_add_sourcefile_cpp(CIEC_STRUCT_test.cpp)
 
forte_test_add_subdirectory(convert)
