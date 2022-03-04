/*******************************************************************************
 * Copyright (c) 2020 Johannes Kepler University Linz
 *               2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ernst Blecha - initial implementation
 *    Martin Melik Merkumians - changes multibit access index from template
 *      parameter to call argument
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
      if(index < 8) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL>(index);
      }
      return ARRAY_AT_ACCESSOR_BASE<TO>::dummy.template partial<CIEC_BOOL>(0);
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
      if(index < 16) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL>(index);
      }
      ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_BYTE> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
      return accessor.getAccessor(index);
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
      if(index < 32) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL>(index);
      }
      ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_WORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
      return accessor.getAccessor(index);
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
      if(index < 64) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BOOL>(index);
      }
      ARRAY_AT_ACCESSOR<TB, TO, CIEC_BOOL, CIEC_DWORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
      return accessor.getAccessor(index);
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
      if(index < 2){
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE>(index);
      }
      return ARRAY_AT_ACCESSOR_BASE<TO>::dummy.template partial<CIEC_BYTE>(0);
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
      if(index < 4) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE>(index);
      }
      ARRAY_AT_ACCESSOR<TB, TO, CIEC_BYTE, CIEC_WORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
      return accessor.getAccessor(index);
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
      if(index < 8) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_BYTE>(index);
      }
      ARRAY_AT_ACCESSOR<TB, TO, CIEC_BYTE, CIEC_DWORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
      return accessor.getAccessor(index);
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
      if(index < 2) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_WORD>(index);
      }
      return ARRAY_AT_ACCESSOR_BASE<TO>::dummy.template partial<CIEC_WORD>(0);
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
      if(index < 4) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_WORD>(index);
      }
      ARRAY_AT_ACCESSOR<TB, TO, CIEC_WORD, CIEC_DWORD> accessor(ARRAY_AT_ACCESSOR_BASE<TO>::dataObject, ARRAY_AT_ACCESSOR_BASE<TO>::dummy);
      return accessor.getAccessor(index);
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
      if(index < 2) {
        return ARRAY_AT_ACCESSOR_BASE<TO>::dataObject.template partial<CIEC_DWORD>(index);
      }
      return ARRAY_AT_ACCESSOR_BASE<TO>::dummy.template partial<CIEC_DWORD>(0);
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
