#*******************************************************************************
# * Copyright (c) 2011 - 2014 nxtControl, ACIN, fortiss GmbH, Profactor GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Stanislav Meduna, Ingo Hegny, Alois Zoitl, Monika Wenger, Matthias Plasch  - initial API and implementation and/or initial documentation
# *******************************************************************************/

#SET(SOURCE_GROUP ${SOURCE_GROUP}\\core)

  forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_test_add_link_directories(${CMAKE_BINARY_DIR}/src/core/datatypes)
  
  forte_test_add_sourcefile_cpp(CIEC_ANY_tests.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_any.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_any_elementary.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_any_date.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_any_string.cpp)

#Unicode utilities
  forte_test_add_sourcefile_cpp(CUnicodeUtilities_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/unicode_utils.cpp)  

#BOOLEAN Datatype  
  forte_test_add_sourcefile_cpp(CIEC_BOOL_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_BOOL_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_bool.cpp)
  
#BIT-Datatypes  
  forte_test_add_sourcefile_cpp(CIEC_BYTE_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_BYTE_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_byte.cpp)
  forte_test_add_sourcefile_cpp(CIEC_WORD_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_WORD_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_word.cpp)  
  forte_test_add_sourcefile_cpp(CIEC_DWORD_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DWORD_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_dword.cpp)  
IF (FORTE_USE_64BIT_DATATYPES)
  forte_test_add_sourcefile_cpp(CIEC_LWORD_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LWORD_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_lword.cpp)  
ENDIF (FORTE_USE_64BIT_DATATYPES) 
  
#INT-Datatypes
  forte_test_add_sourcefile_cpp(CIEC_SINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_SINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_sint.cpp)  
  forte_test_add_sourcefile_cpp(CIEC_USINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_USINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_usint.cpp)  
  forte_test_add_sourcefile_cpp(CIEC_INT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_INT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_int.cpp)
  forte_test_add_sourcefile_cpp(CIEC_UINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_UINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_uint.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_dint.cpp)
  forte_test_add_sourcefile_cpp(CIEC_UDINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_UDINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_udint.cpp)
IF (FORTE_USE_64BIT_DATATYPES)
  forte_test_add_sourcefile_cpp(CIEC_LINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_lint.cpp)  
  forte_test_add_sourcefile_cpp(CIEC_ULINT_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_ULINT_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_ulint.cpp)
ENDIF (FORTE_USE_64BIT_DATATYPES) 


 
IF (FORTE_USE_REAL_DATATYPE)
  forte_test_add_sourcefile_cpp(CIEC_REAL_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_REAL_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_real.cpp)
ENDIF (FORTE_USE_REAL_DATATYPE) 
IF (FORTE_USE_64BIT_DATATYPES)
IF (FORTE_USE_LREAL_DATATYPE) 
  forte_test_add_sourcefile_cpp(CIEC_LREAL_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_LREAL_CASTS_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_lreal.cpp)  
ENDIF (FORTE_USE_LREAL_DATATYPE)  
ENDIF (FORTE_USE_64BIT_DATATYPES) 
  
  forte_test_add_sourcefile_cpp(CIEC_STRING_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_string.cpp)  
  forte_test_add_sourcefile_cpp(CIEC_WSTRING_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_wstring.cpp)  

IF (FORTE_SUPPORT_ARRAYS)
  forte_test_add_sourcefile_cpp(CIEC_ARRAY_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_array.cpp)
ENDIF(FORTE_SUPPORT_ARRAYS)

  forte_test_add_sourcefile_cpp(CIEC_TIME_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_time.cpp)  
  forte_test_add_sourcefile_cpp(CIEC_DATE_AND_TIME_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_date_and_time.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DATE_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_date.cpp)
  forte_test_add_sourcefile_cpp(CIEC_TIME_OF_DAY_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_time_of_day.cpp)
  
  forte_test_add_sourcefile_cpp(CIEC_STRUCT_test.cpp)
  forte_test_add_sourcefile_cpp(../../../src/core/datatypes/forte_struct.cpp)
  
# convert functions for TO_(W)STRING conversions
  forte_test_add_sourcefile_cpp(convert_to_STRING_test.cpp)

IF(FORTE_USE_WSTRING_DATATYPE)
  forte_test_add_sourcefile_cpp(convert_to_WSTRING_test.cpp)
ENDIF(FORTE_USE_WSTRING_DATATYPE)