/*******************************************************************************
  * Copyright (c) 2010 - 2013 ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Ingo Hegny, Alois Zoitl, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_STRUCT_H_
#define _FORTE_STRUCT_H_

#include "forte_any_derived.h"

class CIEC_STRUCT : public CIEC_ANY_DERIVED{
  public:
    CIEC_STRUCT(CStringDictionary::TStringId pa_unTypeName, TForteUInt16 pa_nLength, const CStringDictionary::TStringId pa_unElementTypes[], const CStringDictionary::TStringId pa_unElementNames[], TForteUInt8 TypeID);

    CIEC_STRUCT(const CIEC_STRUCT& pa_roValue);

    virtual ~CIEC_STRUCT();

    CIEC_STRUCT& operator =(const CIEC_STRUCT &pa_roValue){
      setValue(pa_roValue);
      return *this;
    }

//new alignment:
//  ASN1-StructType (8bit)
//  padding (8bit)
//  StructSize (16bit)
//  StructTypeNameID (32bit)
//  *elementNames (system dependent - but aligned)
//  Data (system dependent - but aligned)

    /*! \brief Get the Struct's type
     *
     *   With this command the type-ID of the struct can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - the type-ID of the struct.
     */

    TForteUInt8 getASN1StructType() const{
      return (0 != getGenData()) ? (*((TForteUInt8 *) (getGenData()))) : static_cast<TForteUInt8>(0);
    }

    /*! \brief Get the Struct's size
     *
     *   With this command the size of the struct can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - the size of the struct.
     */
    TForteUInt16 getStructSize() const{
      return (0 != getGenData()) ? (*((TForteUInt16 *) (getGenData() + 2 * sizeof(TForteUInt8)))) : static_cast<TForteUInt16>(0);
    }

    /*! \brief Get the Struct's elementNames
     *
     *   retrieve array of StringIDs of element names.
     *
     *   \param - No parameters necessary.
     *   \return - pointer to array of StringIds.
     */
    const CStringDictionary::TStringId* elementNames() const{
      if(0 != getGenData()){
        CStringDictionary::TStringId** pBuf = reinterpret_cast<CStringDictionary::TStringId**>(const_cast<TForteByte*>(getGenData() + 2 * sizeof(TForteUInt8) + sizeof(TForteUInt16) + sizeof(CStringDictionary::TStringId)));
        return *pBuf;
      }
      return 0;
    }

    /*! \brief Get the Struct's type name
     *
     *   With this command the type name of the struct can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - StringId of Struct's type name.
     */
    CStringDictionary::TStringId getStructTypeNameID() const{
      return (0 != getGenData()) ? (*((CStringDictionary::TStringId *) (getGenData() + 2 * sizeof(TForteUInt8) + sizeof(TForteUInt16)))) : 0;
    }

    void setValue(const CIEC_ANY& pa_roValue);

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_STRUCT;
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
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function to C++ data type.
     *   \param pa_acValue       Pointer to char-array
     *   \param pa_nBufferSize   Size of the provided buffer
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    virtual int toString(char* pa_pacValue, unsigned int pa_nBufferSize) const;

    CIEC_ANY *getMembers(){
      return (CIEC_ANY *) ((0 != getGenData()) ? (getGenData() + scm_unMembersOffset) : 0);
    }

    const CIEC_ANY *getMembers() const{
      return (CIEC_ANY *) ((0 != getGenData()) ? (getGenData() + scm_unMembersOffset) : 0);
    }

    /*! \brief Get the struct's member var with the given name id
     *
     * \param pa_unMemberNameId the string id of the member name
     * \return on a valid member name id a pointer to the member var otherwise 0
     */
    CIEC_ANY *getMemberNamed(CStringDictionary::TStringId pa_unMemberNameId);

  protected:

    //!Function to configure the array if it is created via the typelib
    void setup(CStringDictionary::TStringId pa_unTypeName, TForteUInt16 pa_nLength, const CStringDictionary::TStringId pa_unElementTypes[], const CStringDictionary::TStringId pa_unElementNames[], TForteUInt8 pa_TypeID);

//TODO: remove? already defined in de-/serializer
    enum EASN1Tags{
      e_UNIVERSAL = 0, e_APPLICATION = 64, e_CONTEXT = 128, e_PRIVATE = 192
    };
    enum EASN1Encoding{
      e_PRIMITIVE = 0, e_CONSTRUCTED = 32
    };

  private:
    static const unsigned int scm_unMembersOffset =
        2 * sizeof(TForteUInt8) +               //ASN1Type + padding (2*8bit)
        sizeof(TForteUInt16) +                  //number of elements (1x16bit)
        sizeof(CStringDictionary::TStringId) +  //Datatype name ID   (1x32bit)
        sizeof(CStringDictionary::TStringId*);  //Pointer to const static member of specialized class, containing element names (system-dependent)

     void setASN1StructType(TForteUInt16 pa_unVal){
      TForteByte *pBuf = getGenData();
      if(0 != pBuf){
        *((TForteUInt16 *) (pBuf)) = pa_unVal;
      }
    }

    void setStructTypeNameID(const CStringDictionary::TStringId pa_unVal){
      TForteByte * pBuf = (getGenData() + 2 * sizeof(TForteUInt8) + sizeof(TForteUInt16));
      if(0 != pBuf){
        *((CStringDictionary::TStringId*) (pBuf)) = pa_unVal;
      }
    }

    void setStructSize(TForteUInt16 pa_unVal){
      TForteByte *pBuf = getGenData() + 2 * sizeof(TForteUInt8);
      if(0 != pBuf){
        *((TForteUInt16 *) (pBuf)) = pa_unVal;
      }
    }

    void setElementNames(const CStringDictionary::TStringId* pa_unElementNames){
      CStringDictionary::TStringId** pBuf = (reinterpret_cast<CStringDictionary::TStringId**>((getGenData() + 2 * sizeof(TForteUInt8) + sizeof(TForteUInt16) + sizeof(CStringDictionary::TStringId))));
      if(0 != getGenData()){
        *(pBuf) = (const_cast<CStringDictionary::TStringId*>((pa_unElementNames)));
      }
    }

    void clear();

    static CStringDictionary::TStringId parseNextElementId(const char *pa_pcRunner, int &pa_nCounter);

};

#endif /*_FORTE_STRUCT_H_*/
