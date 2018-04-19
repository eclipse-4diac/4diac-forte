/*******************************************************************************
  * Copyright (c) 2007 - 2013 ACIN, nxtControl GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
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
    CIEC_ARRAY(TForteUInt16 pa_nLength, CStringDictionary::TStringId pa_unArrayType);

    explicit CIEC_ARRAY(const CIEC_ARRAY& pa_roValue);

    virtual ~CIEC_ARRAY();

    //!Function to configure the array if it is created via the typelib
    void setup(TForteUInt16 pa_nLength, CStringDictionary::TStringId pa_unArrayType);

    /*! \brief Get the array's size
     *
     *   With this command the size of the array can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - the length of the array.
     */
    TForteUInt16 size() const{
      return (0 != getGenData()) ? (*((TForteUInt16 *) (getGenData()))) : static_cast<TForteUInt16>(0);
    }

    /*! \brief Operator: CIEC_INT data type [unsigned int] data type
     *
     *   This command implements the index operator for a C++ unsigned int index.
     *   \param pa_nIndex  Index
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_ANY* operator [](TForteUInt16 pa_nIndex){
      if(pa_nIndex < size()){
        return &(getArray()[pa_nIndex]);
      }
      return 0;
    }
    ;

    const CIEC_ANY* operator [](TForteUInt16 pa_nIndex) const{
      if(pa_nIndex < size()){
        return &(getArray()[pa_nIndex]);
      }
      return 0;
    }


    CIEC_ARRAY& operator =(const CIEC_ARRAY &pa_roValue){
    	setValue(pa_roValue);
        return *this;
    }

    /*! \brief Get the data type of elements inside the array
     *
     */
    CIEC_ANY::EDataTypeID getElementDataTypeID() const{
      return getArray() != 0 ? getArray()[-1].getDataTypeID() : CIEC_ANY::e_ANY;
    }
    ;

    virtual void setValue(const CIEC_ANY& pa_roValue);

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
    virtual int fromString(const char *pa_rsValue);
    /*! \brief Converts data type value to array
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC61131 conform data type (array format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param pa_rsValue  Reference to the given String
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    virtual int toString(char* pa_acValue, unsigned int pa_nBufferSize) const;

    virtual unsigned int getToStringBufferSize();

  protected:

  private:
    //This constructor is only to be used by the create instance method
    CIEC_ARRAY();

    /*! \brief CIEC_ARRAY data type member value is a array of CIEC_ANY.
     *
     *   This list contains the elements in the array of the type given by
     *   the reference pointer.
     *
     *   Note: the pointers have to be correctly aligned. To accommodate 64-bit
     *   pointers there is a 6-byte gap after the 2-byte array size.
     */
    CIEC_ANY *getArray(){
      return static_cast<CIEC_ANY *>((0 != getGenData()) ? ((CIEC_ANY *) (getGenData() + 8)) + 1 : 0);
    }

    const CIEC_ANY *getArray() const{
      return static_cast<const CIEC_ANY *>((0 != getGenData()) ? ((const CIEC_ANY *) (getGenData() + 8)) + 1 : 0);
    }

    void setSize(TForteUInt16 pa_unVal){
      TForteByte *pBuf = getGenData();
      if(0 != pBuf){
        *((TForteUInt16 *) (pBuf)) = pa_unVal;
      }
    }

    void clear();
  };

#endif /* FORTE_SUPPORT_ARRAYS */

#endif /*_FORTE_ARRAY_H_*/
