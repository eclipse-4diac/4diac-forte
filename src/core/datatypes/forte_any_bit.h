/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *               2020 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Monika Wenger,
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Ernst Blecha - add multibit partial access
  *******************************************************************************/
#ifndef _ANY_BIT_H_
#define _ANY_BIT_H_

#include "forte_any_elementary.h"
#include "../utils/staticassert.h"
#include <limits>

/*!\ingroup COREDTS IIEC_ANY_BIT represents any bit data types according to IEC 61131.
 */
class CIEC_ANY_BIT : public CIEC_ANY_ELEMENTARY{
  DECLARE_FIRMWARE_DATATYPE(ANY_BIT)

  public:
    virtual ~CIEC_ANY_BIT(){
    }

    template <class TBase, class TObject>
    class PARTIAL_ACCESS_TYPE : public TBase {
    protected:
      typedef typename TBase::TValueType TBaseType; // Shortcut for the value type used by the access object
                                                    // evaluates to bool, TForteByte, TForteWord or TForteDWord
      typedef typename TObject::TValueType TObjectType; // Shortcut for the value type used by the data-object
                                                        // evaluates to TForteByte, TForteWord, TForteDWord or TForteLWord
      static const size_t length = std::numeric_limits<TObjectType>::digits/std::numeric_limits<TBaseType>::digits; // amount of accessible elements

      /*! \brief Method to read the partial element form the referenced object
       *
       *  This is a static method since it is used during object construction
       *
       */
      static TBaseType getPartial(TObject &src, const size_t paIndex){
        if(paIndex >= length) {
          return 0; // FAIL SILENT
        } else if (forte::core::mpl::is_same<TBase,CIEC_BOOL>::value){
          return ( 0 != (src.getLargestUInt() & ((TLargestUIntValueType)1 << paIndex)));
        } else {
          TFortePartial temp;
          temp.mLargestUInt = src.getLargestUInt();
          return temp.mData[paIndex];
        }
      }

      /*! \brief Method for internally setting the objects data type member value
       *
       * Reads the current value from the referenced object, changes the indexed element and writes it back to the referenced object.
       * Also sets the data type member value of this object in order to allow object reuse by the compiler
       *
       */
      PARTIAL_ACCESS_TYPE<TBase, TObject>& setPartial(TBaseType paValue){
        if(index < length) {
          TFortePartial temp;
          temp.mLargestUInt = dataObject.getLargestUInt();
          if (forte::core::mpl::is_same<TBase,CIEC_BOOL>::value){
            if(paValue) {
              temp.mLargestUInt |=  ((TLargestUIntValueType)1 << index);
            } else {
              temp.mLargestUInt &= ~((TLargestUIntValueType)1 << index);
            }
          } else {
            temp.mData[index] = paValue;
          }
          dataObject.setValuePartial(static_cast<TLargestUIntValueType>(temp.mLargestUInt));
          TBase::setLargestUInt(paValue); // Store to the temporary object. Otherwise this might behave incorrect on object reuse.
        }
        return *this;
      }

      /*! \brief Set method for data type member value
       *
       *  The data type value is set through the objects setPartial method;
       *  This derived method handles chaining access elements.
       *
       */
      virtual void setValuePartial(TLargestUIntValueType paValue) {
        setPartial(static_cast<TBaseType>(paValue));
      }

    private:
      union TFortePartial {                    // Split the data value into multiple indexable elements
        TLargestUIntValueType mLargestUInt;
        TBaseType mData[length];
      };

      TObject& dataObject; // The referenced object which data is pulled from
      const size_t index;  // Index value for accessing the split elements
      bool accessedOutOfBounds; // Flag to indicate Out of Bounds access

      /*! \brief Method for handling endianess conversion
       *
       *  On little endian systems the index value can directly be returned.
       *  On big endian systems the order of bytes has to be flipped.
       *
       */
      static size_t endianiseIndex(const size_t paIndex){
#ifdef FORTE_BIG_ENDIAN
          return (forte::core::mpl::is_same<TBase,CIEC_BOOL>::value) ? paIndex : length-1-paIndex; // Within bool-data endianess is implicitly correct
#else //#ifdef FORTE_BIG_ENDIAN
          return paIndex;
#ifndef FORTE_LITTLE_ENDIAN
#error Endianess not defined
#endif //#ifndef FORTE_LITTLE_ENDIAN
#endif //#ifdef FORTE_BIG_ENDIAN
      }

    public:
      /*! \brief Constructor: Setup of byte/word/dword-access-object
       *
       *  The data member value is set based on the value of paSrc and the selected index.
       *  Out of Bounds checking is implemented as an additional runtime check.
       *  The index is stored after correction for endianess.
       */
      explicit PARTIAL_ACCESS_TYPE(TObject& paSrc, const size_t paIndex) :
          TBase(getPartial(paSrc,endianiseIndex(paIndex))), dataObject(paSrc), index(endianiseIndex(paIndex)),
          accessedOutOfBounds((paIndex >= length)){
          FORTE_STATIC_ASSERT(
            (forte::core::mpl::is_same<TObject, CIEC_BYTE>::value || forte::core::mpl::is_same<TObject, CIEC_WORD>::value
              || forte::core::mpl::is_same<TObject, CIEC_DWORD>::value || forte::core::mpl::is_same<TObject, CIEC_LWORD>::value),
            TObject_has_to_be_one_of_CIEC_BYTE_CIEC_WORD_CIEC_DWORD_or_CIEC_LWORD);
          FORTE_STATIC_ASSERT(
            (forte::core::mpl::is_same<TBase, CIEC_BOOL>::value || forte::core::mpl::is_same<TBase, CIEC_BYTE>::value
              || forte::core::mpl::is_same<TBase, CIEC_WORD>::value || forte::core::mpl::is_same<TBase, CIEC_DWORD>::value),
            TBase_has_to_be_one_of_CIEC_BYTE_CIEC_WORD_CIEC_DWORD_or_CIEC_LWORD);
          FORTE_STATIC_ASSERT((std::numeric_limits<TObjectType>::digits > std::numeric_limits<TBaseType>::digits),
            Partial_access_is_only_possible_if_accessed_element_is_smaller_than_the_source);
      };

      /*! \brief read the state of the Out of Bounds flag
       *
       */
      bool getAccessedOutOfBounds() {
        return accessedOutOfBounds;
      }

      /*! \brief Operator: Assignment operator with elementary type as its input
       *
       */
      PARTIAL_ACCESS_TYPE<TBase, TObject>& operator=(TBaseType paValue){
        return setPartial(paValue); // No need for conversion, TBaseType can be directly assigned
      }

      /*! \brief Operator: Assignment operator with CIEC_XXXX as its input
       *
       */
      PARTIAL_ACCESS_TYPE<TBase, TObject>& operator =(const TBase &paValue){
        return setPartial(paValue); // This does conversion from TBase to TBaseType implicitly
      }

      /*! \brief Operator: Assignment operator with elementary type as its input
       *
       */
      PARTIAL_ACCESS_TYPE<TBase, TObject>& operator=(PARTIAL_ACCESS_TYPE<TBase, TObject> paValue){
        return setPartial((TBase)paValue); // No need for conversion, TBaseType can be directly assigned
      }

      /*! \brief Operator: setValue method that handles parts correctly (As long as its not a complex data structure - e.g. CIEC_STRING will not work!).
       *
       */
      virtual void setValue(const CIEC_ANY &pa_roValue){
        TBase::setValue(pa_roValue); //Extract the value using the base class' setValue method
        if (forte::core::mpl::is_same<TBase,CIEC_BOOL>::value){
          setPartial(static_cast<TBaseType>(false != this->getLargestUInt()));
        } else {
          setPartial(static_cast<TBaseType>(this->getLargestUInt()));
        }
      }

    };

  protected:
    CIEC_ANY_BIT() :
        CIEC_ANY_ELEMENTARY(){
    }

    /*! \brief Set method for data type member value
     *
     *  The data type value is set using th setLargestUInt method;
     *  This method needs to be derived in case of chaining multiple access elements after one another (e.g. [LWORD].D<1>().W<1>().B<1>().X<1>()).
     *
     */
    virtual void setValuePartial(TLargestUIntValueType paValue) {
      this->setLargestUInt(paValue);
    }

    template <class TBase, class TObject, size_t TIndex>
    class PARTIAL_ACCESS : public CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TBase, TObject>{
    public:
        /*! \brief Constructor: Setup of byte/word/dword-access-object
         *
         *  The data member value is set based on the value of paSrc and the selected index.
         *  Out of Bounds checking is implemented on the index value as a static check; The index therefore has to be known at compile time.
         *  The index is stored after correction for endianess.
         */
        explicit PARTIAL_ACCESS(TObject &paSrc) :
          PARTIAL_ACCESS_TYPE<TBase, TObject>(paSrc, TIndex){
          FORTE_STATIC_ASSERT((TIndex<CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE < TBase, TObject>::length), Index_for_partial_access_out_of_bounds);
        };

        /*! \brief Operator: Assignment operator with elementary type as its input
         *
         */
        PARTIAL_ACCESS<TBase, TObject, TIndex>& operator=(typename CIEC_ANY_BIT::PARTIAL_ACCESS_TYPE<TBase,TObject>::TBaseType paValue){
          this->setPartial(paValue); // No need for conversion, TBaseType can be directly assigned
          return *this;
        }

        /*! \brief Operator: Assignment operator with CIEC_XXXX as its input
         *
         */
        PARTIAL_ACCESS<TBase, TObject, TIndex>& operator =(const TBase &paValue){
          this->setPartial(paValue); // This does conversion from TBase to TBaseType implicitly
          return *this;
        }
    };

};

#endif /*_MANY_BIT_H_*/
