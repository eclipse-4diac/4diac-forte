################################################################################
# Copyright (c) 2010 - 2014 Profactor GmbH, ACIN, fortiss GmbH
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Michael Hofmann, Alois Zoitl, Monika Wenger, Gerhard Ebenhofer
#       - initial implementation and rework communication infrastructure
################################################################################


#############################################################################
# Core Datatypes
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_subdirectory(convert)

forte_add_sourcefile_hcpp(forte_any_elementary       forte_int   forte_sint)
forte_add_sourcefile_hcpp(forte_dint    forte_udint forte_uint)
forte_add_sourcefile_hcpp(forte_usint   forte_dword forte_word)
forte_add_sourcefile_hcpp(forte_byte    forte_bool  forte_string)
forte_add_sourcefile_hcpp(forte_time  forte_any)
forte_add_sourcefile_hcpp(forte_any_string)
forte_add_sourcefile_hcpp(forte_any_num)
forte_add_sourcefile_hcpp(forte_any_magnitude)
forte_add_sourcefile_hcpp(forte_any_bit)
forte_add_sourcefile_hcpp(forte_any_int)
forte_add_sourcefile_hcpp(forte_any_real)

# include STRUCT Datatype-Support
forte_add_sourcefile_hcpp(forte_struct)

set(FORTE_USE_64BIT_DATATYPES ON CACHE BOOL "Enable Forte 64-Bit Datatypes")
mark_as_advanced(FORTE_USE_64BIT_DATATYPES)
if(FORTE_USE_64BIT_DATATYPES)
  forte_add_sourcefile_hcpp(forte_lint        forte_ulint)
  forte_add_sourcefile_hcpp(forte_time_of_day forte_date_and_time)
  forte_add_sourcefile_hcpp(forte_any_date forte_date        forte_lword)
  forte_add_definition("-DFORTE_USE_64BIT_DATATYPES")
endif(FORTE_USE_64BIT_DATATYPES)

set(FORTE_USE_REAL_DATATYPE ON CACHE BOOL "Enable Forte Real Datatypes")
mark_as_advanced(FORTE_USE_REAL_DATATYPE)
if(FORTE_USE_REAL_DATATYPE)
  forte_add_sourcefile_hcpp(forte_real)
  forte_add_definition("-DFORTE_USE_REAL_DATATYPE")
endif(FORTE_USE_REAL_DATATYPE)

if(FORTE_USE_64BIT_DATATYPES AND FORTE_USE_REAL_DATATYPE)
  forte_add_sourcefile_hcpp(forte_lreal)
  forte_add_definition("-DFORTE_USE_LREAL_DATATYPE")
  set(FORTE_USE_LREAL_DATATYPE ON CACHE INTERNAL "Enable Forte Real Datatypes")
endif(FORTE_USE_64BIT_DATATYPES AND FORTE_USE_REAL_DATATYPE)

set(FORTE_SUPPORT_ARRAYS ON CACHE BOOL "Enable Forte Array Datatypes")
mark_as_advanced(FORTE_SUPPORT_ARRAYS)
if(FORTE_SUPPORT_ARRAYS)
  forte_add_sourcefile_hcpp(forte_array)
  forte_add_definition("-DFORTE_SUPPORT_ARRAYS")
endif(FORTE_SUPPORT_ARRAYS)


set(FORTE_USE_WSTRING_DATATYPE ON CACHE BOOL "Enable Forte WSTRING Datatypes")
mark_as_advanced(FORTE_USE_WSTRING_DATATYPE)
if(FORTE_USE_WSTRING_DATATYPE)
  forte_add_sourcefile_hcpp(forte_wstring)
  forte_add_definition("-DFORTE_USE_WSTRING_DATATYPE")
endif(FORTE_USE_WSTRING_DATATYPE)


set(FORTE_UNICODE_SUPPORT ON CACHE BOOL "Enable unicode support for STRING and WSTRING, WSTRING needs unicode support enabled!")
mark_as_advanced(FORTE_UNICODE_SUPPORT)
if(FORTE_UNICODE_SUPPORT)
  forte_add_sourcefile_hcpp(unicode_utils)
  forte_add_definition("-DFORTE_UNICODE_SUPPORT")
elseif(FORTE_USE_WSTRING_DATATYPE)
   message(FATAL_ERROR "In order to use WSTRING you need unicode support enabled")
endif(FORTE_UNICODE_SUPPORT)

set(FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES ON CACHE BOOL "Enable Forte Custom Serializeable Datatypes")
mark_as_advanced(FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES)
if(FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES)
  forte_add_definition("-DFORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES")
endif(FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES)

