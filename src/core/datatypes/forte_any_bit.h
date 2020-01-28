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
#include <limits>

/*!\ingroup COREDTS IIEC_ANY_BIT represents any bit data types according to IEC 61131.
 */
class CIEC_ANY_BIT : public CIEC_ANY_ELEMENTARY{
  DECLARE_FIRMWARE_DATATYPE(ANY_BIT)

  public:
    virtual ~CIEC_ANY_BIT(){
    }

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
    class PARTIAL_ACCESS : public TBase{
    private:
      typedef typename TBase::TValueType TBaseType; // Shortcut for the value type used by the access object
                                                    // evaluates to TForteByte, TForteWord or TForteDWord
      typedef typename TObject::TValueType TObjectType; // Shortcut for the value type used by the data-object
                                                        // evaluates to TForteWord, TForteDWord or TForteLWord
      static const size_t length = std::numeric_limits<TObjectType>::digits/std::numeric_limits<TBaseType>::digits; // amount of accessible elements
      union TFortePartial {                    // Split the data value into multiple indexable elements
        TLargestUIntValueType mLargestUInt;
        TBaseType mData[length];
      };

      TObject& dataObject; // The referenced object which data is pulled from
      const size_t index;  // Index value for accessing the split elements

      /*! \brief Method for handling endianess conversion
       *
       *  On little endian systems the index value can directly be returned.
       *  On big endian systems the order of bytes has to be flipped.
       *
       */
      static constexpr size_t endianiseIndex(const size_t paIndex){
#ifdef FORTE_BIG_ENDIAN
        return length-1-paIndex;
#else //#ifdef FORTE_BIG_ENDIAN
        return paIndex;
#ifndef FORTE_LITTLE_ENDIAN
#error Endianess not defined
#endif //#ifndef FORTE_LITTLE_ENDIAN
#endif //#ifdef FORTE_BIG_ENDIAN
      }

      /*! \brief Method to read the partial element form the referenced object
       *
       *  This is a static method since it is used during object construction
       *
       */
      static TBaseType getPartial(TObject &src, const size_t index){
        TFortePartial temp;
        temp.mLargestUInt = src.getLargestUInt();
        return temp.mData[index];
      }

      /*! \brief Method for internally setting the objects data type member value
       *
       * Reads the current value from the referenced object, changes the indexed element and writes it back to the referenced object.
       * Also sets the data type member value of this object in order to allow object reuse by the compiler
       *
       */
      PARTIAL_ACCESS<TBase, TObject, TIndex>& setPartial(TBaseType paValue){
        TFortePartial temp;
        temp.mLargestUInt = dataObject.getLargestUInt();
        temp.mData[index] = paValue;
        dataObject.setValuePartial(static_cast<TLargestUIntValueType>(temp.mLargestUInt));
        TBase::setLargestUInt(paValue); // Store to the temporary object. Otherwise this might behave incorrect on object reuse.
        return *this;
      }

      public:
        /*! \brief Constructor: Setup of byte/word/dword-access-object
         *
         *  The data member value is set based on the value of paSrc and the selected index.
         *  Out of Bounds checking is implemented on the index value as a static check; The index therefore has to be known at compile time.
         *  The index is stored after correction for endianess.
         */
        explicit PARTIAL_ACCESS(TObject &paSrc) :
          TBase(getPartial(paSrc, TIndex)), dataObject(paSrc), index(endianiseIndex(TIndex)){
          static_assert(TIndex < length, "Index for partial access out of bounds.");
        };

        /*! \brief Operator: setValue method that handles parts correctly (As long as its not a complex data structure - e.g. CIEC_STRING will not work!).
         *
         */
        virtual void setValue(const CIEC_ANY &pa_roValue){
          TBase::setValue(pa_roValue); //Extract the value using the base class' setValue method
          setPartial(static_cast<TBaseType>(this->getLargestUInt()));
        }

        /*! \brief Operator: Assignment operator with elementary type as its input
         *
         */
        PARTIAL_ACCESS<TBase, TObject, TIndex>& operator =(TBaseType paValue){
          return setPartial(paValue); // No need for conversion, TBaseType can be directly assigned
        }

        /*! \brief Operator: Assignment operator with CIEC_XXXX as its input
         *
         */
        PARTIAL_ACCESS<TBase, TObject, TIndex>& operator =(const TBase &paValue){
          return setPartial(paValue); // This does conversion from TBase to TBaseType implicitly
        }

      protected:
        /*! \brief Set method for data type member value
         *
         *  The data type value is set through the objects setPartial method;
         *  This derived method handles chaining access elements.
         *
         */
        virtual void setValuePartial(TLargestUIntValueType paValue) {
          setPartial(static_cast<TBaseType>(paValue));
        }

    };

    template <class TBOOL, class TObject, size_t TIndex> // the template parameter TBOOL allows use of CIEC_BOOL "before" it is fully evaluated
    class PARTIAL_ACCESS_BIT : public TBOOL{
    private:
      typedef typename TObject::TValueType TObjectType; // Shortcut for the value type used by the data-object
                                                        // evaluates to TForteByte, TForteWord, TForteDWord or TForteLWord
      static const size_t length = std::numeric_limits<TObjectType>::digits; // amount of accessible bits

      TObject& dataObject; // The referenced object which data is pulled from

      /*! \brief Method to read the partial element form the referenced object
       *
       *  This is a static method since it is used during object construction
       *
       */
      static bool getPartial(TObject &src, const size_t index){
        return ( 0 != (src.getLargestUInt() & ((TLargestUIntValueType)1 << index)));
      }

      /*! \brief Method for internally setting the objects data type member value
       *
       * Reads the current value from the referenced object, changes the indexed element and writes it back to the referenced object.
       * Also sets the data type member value of this object in order to allow object reuse by the compiler
       *
       */
      PARTIAL_ACCESS_BIT<TBOOL, TObject, TIndex>& setPartial(bool paValue) {
        TLargestUIntValueType temp = dataObject.getLargestUInt();
        if(paValue) {
          temp |=  ((TLargestUIntValueType)1 << TIndex);
        } else {
          temp &= ~((TLargestUIntValueType)1 << TIndex);
        }
        dataObject.setValuePartial(static_cast<TLargestUIntValueType>(temp));
        this->setLargestUInt(false != paValue); // Store to the temporary object. Otherwise this might behave incorrect on object reuse.
        return *this;
      }

      public:
        /*! \brief Constructor: Setup of bit-access-object
         *
         *  The data member value is set based on the value of paSrc and the selected index.
         *  Out of Bounds checking is implemented on the index value as a static check; The index therefore has to be known at compile time.
         *  As bit-alignment is not affected by endianess the index can directly be used without conversion.
         *
         */
        explicit PARTIAL_ACCESS_BIT(TObject &paSrc) :
          TBOOL(getPartial(paSrc, TIndex)), dataObject(paSrc){
          static_assert(TIndex < length, "Index for partial access out of bounds.");
        }

        /*! \brief Operator: setValue method that handles parts correctly (As long as its not a complex data structure - e.g. CIEC_STRING will not work!).
         *
         */
        virtual void setValue(const CIEC_ANY &pa_roValue){
          TBOOL::setValue(pa_roValue);
          setPartial(static_cast<bool>(false != this->getLargestUInt()));
        }

        /*! \brief Operator: Assignment operator with elementary bool as its input
         *
         */
        PARTIAL_ACCESS_BIT<TBOOL, TObject, TIndex>& operator =(bool paValue){
          return setPartial(paValue); // No need for conversion, TBaseType can be directly assigned
        }

        /*! \brief Operator: Assignment operator with CIEC_BOOL as its input
         *
         */
        PARTIAL_ACCESS_BIT<TBOOL, TObject, TIndex>& operator =(const TBOOL &paValue){
          return setPartial(paValue); // This does conversion from CIEC_BOOL to bool implicitly
        }

    };

};

#endif /*_MANY_BIT_H_*/
