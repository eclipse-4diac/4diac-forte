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

forte_add_sourcefile_hcpp(forte_any)
  forte_add_sourcefile_hcpp(forte_any_elementary)
  forte_add_sourcefile_h(forte_any_magnitude.h)
  forte_add_sourcefile_h(forte_any_num.h)
    forte_add_sourcefile_hcpp(forte_any_real)
      forte_add_sourcefile_hcpp(forte_real)
      forte_add_sourcefile_hcpp(forte_lreal)
    forte_add_sourcefile_h(forte_any_int.h)
      forte_add_sourcefile_h(forte_any_unsigned.h)
        forte_add_sourcefile_hcpp(forte_usint)
        forte_add_sourcefile_hcpp(forte_uint)
        forte_add_sourcefile_hcpp(forte_udint)
        forte_add_sourcefile_hcpp(forte_ulint)
      forte_add_sourcefile_h(forte_any_signed.h)
        forte_add_sourcefile_hcpp(forte_sint)
        forte_add_sourcefile_hcpp(forte_int)
        forte_add_sourcefile_hcpp(forte_dint)
        forte_add_sourcefile_hcpp(forte_lint)
    forte_add_sourcefile_hcpp(forte_any_duration)
      forte_add_sourcefile_hcpp(forte_time)
      forte_add_sourcefile_hcpp(forte_ltime)
  forte_add_sourcefile_h(forte_any_bit.h)
    forte_add_sourcefile_hcpp(forte_bool)
    forte_add_sourcefile_hcpp(forte_byte)
    forte_add_sourcefile_hcpp(forte_word)
    forte_add_sourcefile_hcpp(forte_dword)
    forte_add_sourcefile_hcpp(forte_lword)
  forte_add_sourcefile_h(forte_any_chars.h)
    forte_add_sourcefile_hcpp(forte_any_string)
      forte_add_sourcefile_hcpp(forte_string)
      forte_add_sourcefile_h(forte_string_fixed.h)
    forte_add_sourcefile_h(forte_any_char.h)
      forte_add_sourcefile_hcpp(forte_char)
      forte_add_sourcefile_hcpp(forte_wchar)
  forte_add_sourcefile_hcpp(forte_any_date)
    forte_add_sourcefile_hcpp(forte_date_and_time)
    forte_add_sourcefile_hcpp(forte_date)
    forte_add_sourcefile_hcpp(forte_time_of_day)
    forte_add_sourcefile_hcpp(forte_ldate_and_time)
    forte_add_sourcefile_hcpp(forte_ltime_of_day)
    forte_add_sourcefile_hcpp(forte_ldate)

forte_add_sourcefile_hcpp(forte_state)

forte_add_sourcefile_hcpp(forte_any_variant)
  forte_add_sourcefile_hcpp(forte_any_elementary_variant)
  forte_add_sourcefile_hcpp(forte_any_magnitude_variant)
    forte_add_sourcefile_hcpp(forte_any_num_variant)
      forte_add_sourcefile_hcpp(forte_any_real_variant)
      forte_add_sourcefile_hcpp(forte_any_int_variant)
        forte_add_sourcefile_hcpp(forte_any_signed_variant)
        forte_add_sourcefile_hcpp(forte_any_unsigned_variant)
    forte_add_sourcefile_hcpp(forte_any_duration_variant)
  forte_add_sourcefile_hcpp(forte_any_bit_variant)
  forte_add_sourcefile_hcpp(forte_any_chars_variant)
    forte_add_sourcefile_hcpp(forte_any_string_variant)
    forte_add_sourcefile_hcpp(forte_any_char_variant)
  forte_add_sourcefile_hcpp(forte_any_date_variant)

forte_add_sourcefile_h(forte_any_unique_ptr.h)

forte_add_sourcefile_h(forte_any_bit_not_decorator.h)
forte_add_sourcefile_h(forte_any_bit_partial.h)


# include STRUCT Datatype-Support
forte_add_sourcefile_hcpp(forte_struct)

forte_add_sourcefile_hcpp(forte_array)
forte_add_sourcefile_h(forte_array_common.h)
forte_add_sourcefile_h(forte_array_variable.h)
forte_add_sourcefile_h(forte_array_fixed.h)
forte_add_sourcefile_hcpp(forte_array_dynamic)


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

