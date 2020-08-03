/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, nxtControl GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Stansilav Meduna, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FORTE_ARRAY_H_
#define _FORTE_ARRAY_H_

#include "forte_any_derived.h"

#ifdef FORTE_SUPPORT_ARRAYS

/*!\ingroup COREDTS CIEC_ARRAY represents the array data type according to IEC 61131.
 */
class CIEC_ARRAY : public CIEC_ANY_DERIVED{
  DECLARE_FIRMWARE_DATATYPE(ARRAY)
  public:
    CIEC_ARRAY(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType);

    explicit CIEC_ARRAY(const CIEC_ARRAY& paValue);

    virtual ~CIEC_ARRAY();

    //!Function to configure the array if it is created via the typelib
    void setup(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType);

    /*! \brief Get the array's size
     *
     *   With this command the size of the array can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - the length of the array.
     */
    TForteUInt16 size() const{
      return (0 != getSpecs()) ? getSpecs()->mLength : static_cast<TForteUInt16>(0);
    }

    /*! \brief Operator: CIEC_INT data type [unsigned int] data type
     *
     *   This command implements the index operator for a C++ unsigned int index.
     *   \param paIndex  Index
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_ANY* operator [](TForteUInt16 paIndex){
      return (paIndex < size()) ? &(getArray()[paIndex]) : 0;
    }
    ;

    const CIEC_ANY* operator [](TForteUInt16 paIndex) const{
      return (paIndex < size()) ? &(getArray()[paIndex]) : 0;
    }


    CIEC_ARRAY& operator =(const CIEC_ARRAY &paValue){
      if( this != &paValue) {
        setValue(paValue);
      }
      return *this;
    }

    /*! \brief Get the data type of elements inside the array
     *
     */
    CIEC_ANY::EDataTypeID getElementDataTypeID() const{
      return getReferenceElement() != 0 ? getReferenceElement()->getDataTypeID() : CIEC_ANY::e_ANY;
    }
    ;

    virtual void setValue(const CIEC_ANY& paValue);

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_ARRAY;
    }

    /*! \brief Converts array value to data type value
     *
     *   This command implements a conversion function from IEC61131
     *   data type (array format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   \param pa_pacValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    virtual int fromString(const char *paValue);
    /*! \brief Converts data type value to array
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC61131 conform data type (array format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param pa_rsValue  Reference to the given String
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;

    virtual size_t getToStringBufferSize() const;

  private:
    //This constructor is only to be used by the create instance method
    CIEC_ARRAY();

    class CArraySpecs {
      public:
        explicit CArraySpecs(TForteUInt16 paLength) :
            mLength(paLength) {
          mArray = new CIEC_ANY[paLength + 1];  //store an additional reference element in the first place
        }

        ~CArraySpecs() {
          delete[] mArray;
        }

        TForteUInt16 mLength;

        CIEC_ANY *getRefElement(){
          return mArray;
        }

        const CIEC_ANY *getRefElement() const{
          return mArray;
        }

        CIEC_ANY *getArrayContent(){
          return &(mArray[1]);          //the array content starts from the first entry in our array
        }

        const CIEC_ANY *getArrayContent() const {
          return &(mArray[1]);          //the array content starts from the first entry in our array
        }

      private:
        CIEC_ANY *mArray;
    };

    /*! \brief CIEC_ARRAY data type member value is a array of CIEC_ANY.
     *
     *   This list contains the elements in the array of the type given by
     *   the reference pointer.
     *
     */
    CIEC_ANY *getArray(){
      return (0 != getSpecs()) ? getSpecs()->getArrayContent() : static_cast<CIEC_ANY *>(0);
    }

    const CIEC_ANY *getArray() const{
      return (0 != getSpecs()) ? getSpecs()->getArrayContent() : static_cast<const CIEC_ANY *>(0);
    }

    CIEC_ANY *getReferenceElement() {
      return (0 != getSpecs()) ? getSpecs()->getRefElement() : static_cast<CIEC_ANY *>(0);
    }

    const CIEC_ANY *getReferenceElement() const {
      return (0 != getSpecs()) ? getSpecs()->getRefElement() : static_cast<CIEC_ANY *>(0);
    }

    const CArraySpecs* getSpecs() const {
      return reinterpret_cast<const CArraySpecs*>(getGenData());
    }

    CArraySpecs* getSpecs() {
      return reinterpret_cast<CArraySpecs*>(getGenData());
    }

    void clear();

    void initializeFromString(TForteUInt16 paArraySize, int* paValueLen, TForteUInt16 paPosition, const char* paSrcString, CIEC_ANY ** paBufVal);

    static void findNextNonBlankSpace(const char** paRunner);
  };

#endif /* FORTE_SUPPORT_ARRAYS */

#endif /*_FORTE_ARRAY_H_*/
