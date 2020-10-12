/*******************************************************************************
 * Copyright (c) 2020 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ernst Blecha - initial implementation
 *******************************************************************************/
#ifndef _FORTE_ARRAY_AT_H_
#define _FORTE_ARRAY_AT_H_

#ifdef FORTE_USE_64BIT_DATATYPES
#include <forte_lword.h>
#else
#include <forte_dword.h>
#endif
#include "../utils/staticassert.h"



template <class TBase, class TObject, const size_t startIndex, const size_t stopIndex>
class ARRAY_AT {

private:
  typedef typename TBase::TValueType TBaseType; // Shortcut for the value type used by the access object
                                                // evaluates to bool, TForteByte, TForteWord or TForteDWord
  typedef typename TObject::TValueType TObjectType; // Shortcut for the value type used by the data-object
                                                    // evaluates to TForteByte, TForteWord, TForteDWord or TForteLWord
  TObject& dataObject; // The referenced object which data is pulled from
  TObject dummy;       // Dummy object for out of bounds data
    bool accessedOutOfBounds; // Flag to indicate Out of Bounds access

  template <class TO> class ARRAY_AT_ACCESSOR_BASE {
  protected:
    TO& dataObject; // The referenced object which data is pulled from
    TO& dummy;      // Dummy object for out of bounds data
  public:
    ARRAY_AT_ACCESSOR_BASE<TO>(TO& paDataObject, TO& paDummy): dataObject(paDataObject), dummy(paDummy) {
          FORTE_STATIC_ASSERT(
            (forte::core::mpl::is_same<TO, CIEC_BYTE>::value || forte::core::mpl::is_same<TO, CIEC_WORD>::value
              || forte::core::mpl::is_same<TO, CIEC_DWORD>::value || forte::core::mpl::is_same<TO, CIEC_LWORD>::value),
            TO_has_to_be_one_of_CIEC_BYTE_CIEC_WORD_CIEC_DWORD_or_CIEC_LWORD);
    }
  };

    template<class TB, class TO, class TData, class TStorage> class ARRAY_AT_ACCESSOR : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    /*! \brief Default implementation of the ARRAY_AT_ACCESSOR-class;
     *         throws a compile-time-error to inform the user that something went terribly wrong.
     *
     */
    ARRAY_AT_ACCESSOR<TB, TO, TData, TStorage>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) {
      // this static assert should never be compiled; if this happens an unknown type of accessor was requested
      // if this was just a plain "static_assert(false)" it would still trigger if the template was never instantiated
      // as BOOL->BYTE is already handled by a specialization the statement will evaluate to false
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TData, CIEC_BOOL>::value && forte::core::mpl::is_same<TStorage, CIEC_BYTE>::value),
            Invalid_accessor_type_requested);
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_BOOL, CIEC_BYTE> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_BYTE>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a BOOL within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT(
            (forte::core::mpl::is_same<TO, CIEC_BYTE>::value || forte::core::mpl::is_same<TO, CIEC_WORD>::value
              || forte::core::mpl::is_same<TO, CIEC_DWORD>::value || forte::core::mpl::is_same<TO, CIEC_LWORD>::value),
            TO_has_to_be_one_of_CIEC_BYTE_CIEC_WORD_CIEC_DWORD_or_CIEC_LWORD)

          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_BOOL>::value), TB_has_to_be_CIEC_BOOL);
      switch(index) {
        case  0: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,0>();
        case  1: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,1>();
        case  2: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,2>();
        case  3: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,3>();
        case  4: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,4>();
        case  5: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,5>();
        case  6: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,6>();
        case  7: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,7>();
        default: return ARRAY_AT_ACCESSOR_BASE<TO>::dummy.template partial<CIEC_BOOL,0>();
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_BOOL, CIEC_WORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_WORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a BOOL within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT(
            (forte::core::mpl::is_same<TO, CIEC_WORD>::value || forte::core::mpl::is_same<TO, CIEC_DWORD>::value
              || forte::core::mpl::is_same<TO, CIEC_LWORD>::value), TO_has_to_be_one_of_CIEC_WORD_CIEC_DWORD_or_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_BOOL>::value), TB_has_to_be_CIEC_BOOL);
      switch(index) {
        case  8: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,8>();
        case  9: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,9>();
        case 10: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,10>();
        case 11: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,11>();
        case 12: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,12>();
        case 13: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,13>();
        case 14: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,14>();
        case 15: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,15>();
        default:
          ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_BYTE> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
          return accessor.getAccessor(index);
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_BOOL, CIEC_DWORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_DWORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a BOOL within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TO, CIEC_DWORD>::value || forte::core::mpl::is_same<TO, CIEC_LWORD>::value),
            TO_has_to_be_one_of_CIEC_DWORD_or_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_BOOL>::value), TB_has_to_be_CIEC_BOOL);
      switch(index) {
        case 16: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,16>();
        case 17: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,17>();
        case 18: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,18>();
        case 19: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,19>();
        case 20: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,20>();
        case 21: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,21>();
        case 22: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,22>();
        case 23: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,23>();
        case 24: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,24>();
        case 25: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,25>();
        case 26: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,26>();
        case 27: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,27>();
        case 28: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,28>();
        case 29: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,29>();
        case 30: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,30>();
        case 31: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,31>();
        default:
          ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_WORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
          return accessor.getAccessor(index);
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_BOOL, CIEC_LWORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_LWORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a BOOL within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TO, CIEC_LWORD>::value), TO_has_to_be_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_BOOL>::value), TB_has_to_be_CIEC_BOOL);
      switch(index) {
        case 32: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,32>();
        case 33: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,33>();
        case 34: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,34>();
        case 35: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,35>();
        case 36: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,36>();
        case 37: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,37>();
        case 38: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,38>();
        case 39: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,39>();
        case 40: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,40>();
        case 41: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,41>();
        case 42: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,42>();
        case 43: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,43>();
        case 44: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,44>();
        case 45: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,45>();
        case 46: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,46>();
        case 47: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,47>();
        case 48: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,48>();
        case 49: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,49>();
        case 50: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,50>();
        case 51: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,51>();
        case 52: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,52>();
        case 53: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,53>();
        case 54: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,54>();
        case 55: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,55>();
        case 56: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,56>();
        case 57: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,57>();
        case 58: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,58>();
        case 59: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,59>();
        case 60: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,60>();
        case 61: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,61>();
        case 62: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,62>();
        case 63: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL,63>();
        default:
          ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_DWORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
          return accessor.getAccessor(index);
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_BYTE, CIEC_WORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_BYTE, CIEC_WORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a BYTE within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT(
            (forte::core::mpl::is_same<TO, CIEC_WORD>::value || forte::core::mpl::is_same<TO, CIEC_DWORD>::value
              || forte::core::mpl::is_same<TO, CIEC_LWORD>::value), TO_has_to_be_one_of_CIEC_BYTE_CIEC_WORD_CIEC_DWORD_or_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_BYTE>::value), TB_has_to_be_CIEC_BYTE);
          switch(index){
        case  0: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE,0>();
        case  1: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE,1>();
        default: return ARRAY_AT_ACCESSOR_BASE<TO>::dummy.template partial<CIEC_BYTE, 0>();
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_BYTE, CIEC_DWORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_BYTE, CIEC_DWORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a BYTE within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TO, CIEC_DWORD>::value || forte::core::mpl::is_same<TO, CIEC_LWORD>::value),
            TO_has_to_be_one_of_CIEC_DWORD_or_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_BYTE>::value), TB_has_to_be_CIEC_BYTE);
      switch(index) {
        case  2: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE,2>();
        case  3: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE,3>();
        default:
          ARRAY_AT_ACCESSOR<TB, TO, CIEC_BYTE, CIEC_WORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
          return accessor.getAccessor(index);
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_BYTE, CIEC_LWORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_BYTE, CIEC_LWORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a BYTE within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TO, CIEC_LWORD>::value), TO_has_to_be_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_BYTE>::value), TB_has_to_be_CIEC_BYTE);
      switch(index) {
        case  4: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE,4>();
        case  5: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE,5>();
        case  6: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE,6>();
        case  7: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE,7>();
        default:
          ARRAY_AT_ACCESSOR<TB, TO, CIEC_BYTE, CIEC_DWORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
          return accessor.getAccessor(index);
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_WORD, CIEC_DWORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_WORD, CIEC_DWORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a WORD within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TO, CIEC_DWORD>::value || forte::core::mpl::is_same<TO, CIEC_LWORD>::value),
            TO_has_to_be_one_of_CIEC_DWORD_or_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_WORD>::value), TB_has_to_be_CIEC_WORD);
      switch(index) {
        case  0: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_WORD,0>();
        case  1: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_WORD,1>();
        default: return ARRAY_AT_ACCESSOR_BASE<TO>::dummy.template partial<CIEC_WORD,0>();
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_WORD, CIEC_LWORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_WORD, CIEC_LWORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a WORD within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TO, CIEC_LWORD>::value), TO_has_to_be_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_WORD>::value), TB_has_to_be_CIEC_WORD);
      switch(index) {
        case  2: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_WORD,2>();
        case  3: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_WORD,3>();
        default:
          ARRAY_AT_ACCESSOR<TB, TO, CIEC_WORD, CIEC_DWORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
          return accessor.getAccessor(index);
      }
    }
  };

  template <class TB, class TO> class ARRAY_AT_ACCESSOR <TB, TO, CIEC_DWORD, CIEC_LWORD> : public ARRAY_AT_ACCESSOR_BASE<TO> {
  public:
    ARRAY_AT_ACCESSOR<TB, TO, CIEC_DWORD, CIEC_LWORD>(TO& paDataObject, TO& paDummy) : ARRAY_AT_ACCESSOR_BASE<TO>(paDataObject, paDummy) { }
    /*! \brief Implementation of the getAccessor-Method for accessing a DWORD within ANY_BIT-types
     *
     */
    inline CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TB, TO> getAccessor(size_t index) {
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TO, CIEC_LWORD>::value), TO_has_to_be_CIEC_LWORD);
          FORTE_STATIC_ASSERT((forte::core::mpl::is_same<TB, CIEC_DWORD>::value), TB_has_to_be_CIEC_DWORD);
      switch(index) {
        case  0: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_DWORD,0>();
        case  1: return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_DWORD,1>();
        default: return ARRAY_AT_ACCESSOR_BASE<TO>::dummy.template partial<CIEC_DWORD,0>();
      }
    }
  };

public:
  /*! \brief Constructor: Setup of array-access-object
   *
   *  The data member value is set based on the value of paSrc
   *  The valid index range is given by startIndex and stopIndex (both included)
   */
  ARRAY_AT(TObject &paSrc) :
        dataObject(paSrc), accessedOutOfBounds(false) {
    const size_t length = std::numeric_limits<typename TObject::TValueType>::digits/std::numeric_limits<typename TBase::TValueType>::digits;
      FORTE_STATIC_ASSERT((length > stopIndex), Invalid_index_range);
      FORTE_STATIC_ASSERT((startIndex >= 0), Invalid_index_range);
      FORTE_STATIC_ASSERT(
        (forte::core::mpl::is_same<TObject, CIEC_BYTE>::value || forte::core::mpl::is_same<TObject, CIEC_WORD>::value
          || forte::core::mpl::is_same<TObject, CIEC_DWORD>::value || forte::core::mpl::is_same<TObject, CIEC_LWORD>::value),
        TObject_has_to_be_one_of_CIEC_BYTE_CIEC_WORD_CIEC_DWORD_or_CIEC_LWORD);
      FORTE_STATIC_ASSERT(
        (forte::core::mpl::is_same<TBase, CIEC_BOOL>::value || forte::core::mpl::is_same<TBase, CIEC_BYTE>::value
          || forte::core::mpl::is_same<TBase, CIEC_WORD>::value || forte::core::mpl::is_same<TBase, CIEC_DWORD>::value),
        TBase_has_to_be_one_of_CIEC_BYTE_CIEC_WORD_CIEC_DWORD_or_CIEC_LWORD);
      FORTE_STATIC_ASSERT((std::numeric_limits<TObjectType>::digits > std::numeric_limits<TBaseType>::digits),
        Partial_acces_is_only_possible_if_accessed_element_is_smaller_than_the_source);
  };

  /*! \brief Operator: create accessor for the given index
   *
   */
  CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TBase, TObject> operator[] (const size_t index) {
    ARRAY_AT_ACCESSOR<TBase, TObject, TBase, TObject> accessor(dataObject, dummy);
    if (index >= startIndex && index <= stopIndex) {
      return accessor.getAccessor(index);
    } else {
      accessedOutOfBounds =  true;
      return accessor.getAccessor(-1); // FAIL SILENT: return a dummy-object (attention: value is undefined!)
    }
  }

  /*! \brief read the state of the Out of Bounds flag
   *
   */
    bool getAccessedOutOfBounds() const {
    return accessedOutOfBounds;
  }

};

#endif /*_FORTE_ARRAY_AT_H_*/
