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

    typedef const char* TStructInitialValues;

    CIEC_STRUCT(CStringDictionary::TStringId paTypeName, TForteUInt16 paLength, const CStringDictionary::TStringId paElementTypes[],
        const CStringDictionary::TStringId paElementNames[], TForteUInt8 paTypeID, TStructInitialValues *paInitialValues = 0);

    CIEC_STRUCT(const CIEC_STRUCT& paValue);

    virtual ~CIEC_STRUCT();

    CIEC_STRUCT& operator =(const CIEC_STRUCT &paValue){
      setValue(paValue);
      return *this;
    }

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

    void setValue(const CIEC_ANY& paValue);

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
    virtual int fromString(const char *paValue);
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function to C++ data type.
     *   \param pa_acValue       Pointer to char-array
     *   \param pa_nBufferSize   Size of the provided buffer
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    virtual int toString(char* paValue, unsigned int paBufferSize) const;

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
    CIEC_ANY *getMemberNamed(CStringDictionary::TStringId paMemberNameId);

    virtual unsigned int getToStringBufferSize() const;

    TStructInitialValues* getInitialValues() const {
      return
          (0 != getGenData()) ?
            *reinterpret_cast<TStructInitialValues**>(const_cast<TForteByte*>(getGenData() + 2 * sizeof(TForteUInt8) + sizeof(TForteUInt16)
              + sizeof(CStringDictionary::TStringId) + sizeof(CStringDictionary::TStringId*))) : 0;
      }

  protected:

    enum EASN1Tags{
      e_UNIVERSAL = 0, e_APPLICATION = 64, e_CONTEXT = 128, e_PRIVATE = 192
    };
    enum EASN1Encoding{
    e_PRIMITIVE = 0, e_CONSTRUCTED = 32
    };

    //!Function to configure the array if it is created via the typelib
    void setup(CStringDictionary::TStringId paTypeName, TForteUInt16 paLength, const CStringDictionary::TStringId paElementTypes[],
        const CStringDictionary::TStringId paElementNames[], TForteUInt8 paTypeID, TStructInitialValues *paInitialValues = 0);

  private:

    /*
     * Alignment of the struct:
     *   ASN1-StructType (8bit)
     *   Padding (8bit)
     *   StructSize (number of elements) (16bit)
     *   StructTypeNameID (32bit)
     *   elementNames (system dependent - but aligned)
     *   Data (system dependent - but aligned)
     */

    static const unsigned int scm_unMembersOffset =
        2 * sizeof(TForteUInt8) +               //ASN1Type + padding (2*8bit)
        sizeof(TForteUInt16) +                  //number of elements (1x16bit)
        sizeof(CStringDictionary::TStringId) +  //StructureType name ID   (1x32bit)
      sizeof(CStringDictionary::TStringId*) + //Pointer to const static member of specialized class, containing element names (system-dependent)
      sizeof(TStructInitialValues*); //InitialValues

     void setASN1StructType(TForteUInt16 paVal){
      TForteByte *pBuf = getGenData();
      if(0 != pBuf){
        *((TForteUInt16 *) (pBuf)) = paVal;
      }
    }

    void setStructTypeNameID(const CStringDictionary::TStringId paVal){
      TForteByte * pBuf = (getGenData() + 2 * sizeof(TForteUInt8) + sizeof(TForteUInt16));
      if(0 != pBuf){
        *((CStringDictionary::TStringId*) (pBuf)) = paVal;
      }
    }

    void setStructSize(TForteUInt16 paVal){
      TForteByte *pBuf = getGenData() + 2 * sizeof(TForteUInt8);
      if(0 != pBuf){
        *((TForteUInt16 *) (pBuf)) = paVal;
      }
    }

    void setElementNames(const CStringDictionary::TStringId* paElementNames){
      CStringDictionary::TStringId** pBuf = (reinterpret_cast<CStringDictionary::TStringId**>((getGenData() + 2 * sizeof(TForteUInt8) + sizeof(TForteUInt16) + sizeof(CStringDictionary::TStringId))));
      if(0 != getGenData()){
        *(pBuf) = (const_cast<CStringDictionary::TStringId*>((paElementNames)));
      }
    }

    void setInitialValues(TStructInitialValues* paInitialValue) {
      TStructInitialValues** pBuf = (reinterpret_cast<TStructInitialValues**>((getGenData() + 2 * sizeof(TForteUInt8) + sizeof(TForteUInt16)
        + sizeof(CStringDictionary::TStringId) + sizeof(CStringDictionary::TStringId*))));
      if(0 != getGenData()) {
        *pBuf = paInitialValue;
      }
    }

    void clear();

    void storeNewFoundElement(CStringDictionary::TStringId* paStorage, CStringDictionary::TStringId paToStore, size_t *paPosition);

    void initializeNotFoundElements(CStringDictionary::TStringId* paFoundElements, size_t paNumberOfFound);

    void getNotFoundElements(CStringDictionary::TStringId* paFoundElements, size_t paNumberOfFound, CStringDictionary::TStringId* paNotFoundElements);

    void initializeMemberWithInitialValue(CIEC_ANY* paToInitialize, size_t paPosition);

    void findNextNonBlankSpace(const char** paRunner);

    static CStringDictionary::TStringId parseNextElementId(const char *paRunner, int &paCounter);

};

#endif /*_FORTE_STRUCT_H_*/
