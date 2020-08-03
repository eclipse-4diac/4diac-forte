/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
      if(this != &paValue) {
        setValue(paValue);
      }
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
      return (0 != getSpecs()) ? getSpecs()->mASN1Type : static_cast<TForteUInt8>(0);
    }

    /*! \brief Get the Struct's size
     *
     *   With this command the size of the struct can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - the size of the struct.
     */
    TForteUInt16 getStructSize() const{
      return (0 != getSpecs()) ? getSpecs()->mNumberOfElements : static_cast<TForteUInt16>(0);
    }

    /*! \brief Get the Struct's elementNames
     *
     *   retrieve array of StringIDs of element names.
     *
     *   \param - No parameters necessary.
     *   \return - pointer to array of StringIds.
     */
    const CStringDictionary::TStringId* elementNames() const{
      return (0 != getSpecs()) ? getSpecs()->mElementNames : 0;
    }

    /*! \brief Get the Struct's type name
     *
     *   With this command the type name of the struct can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - StringId of Struct's type name.
     */
    CStringDictionary::TStringId getStructTypeNameID() const{
      return (0 != getSpecs()) ? getSpecs()->mStructureTypeID : 0;
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
     *   \param paValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    virtual int fromString(const char *paValue);
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function to C++ data type.
     *   \param paValue       Pointer to char-array
     *   \param paBufferSize   Size of the provided buffer
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;

    CIEC_ANY *getMembers(){
      return (0 != getSpecs()) ? getSpecs()->mMembers : static_cast<CIEC_ANY *>(0);
    }

    const CIEC_ANY *getMembers() const{
      return (0 != getSpecs()) ? getSpecs()->mMembers : static_cast<CIEC_ANY *>(0);
    }
    /*! \brief Get the struct's member var with the given name id
     *
     * \param pa_unMemberNameId the string id of the member name
     * \return on a valid member name id a pointer to the member var otherwise 0
     */
    CIEC_ANY *getMemberNamed(CStringDictionary::TStringId paMemberNameId);

    virtual size_t getToStringBufferSize() const;

  protected:
    /*! \brief helper method for accessing a member by index
     *
     * Mainly used for the generated accessor-functions.
     * Therefore the index is not range checked!
     *
     * \param paMemberIndex index into the member array
     * \return pointer to the member var
     */
    CIEC_ANY* getMember(size_t paMemberIndex){
      return &getMembers()[paMemberIndex];
    }

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

    class CStructSpecs {
      public:
        CStructSpecs(CStringDictionary::TStringId paTypeName, TForteUInt16 paLength, const CStringDictionary::TStringId paElementNames[], TForteUInt8 paTypeID) :
            mASN1Type(paTypeID), mNumberOfElements(paLength), mStructureTypeID(paTypeName), mElementNames(paElementNames) {
          mMembers = new CIEC_ANY[paLength];
        }

        ~CStructSpecs() {
          delete[] mMembers;
        }

        TForteUInt8 mASN1Type;
        TForteUInt16 mNumberOfElements;
        CStringDictionary::TStringId mStructureTypeID;
        const CStringDictionary::TStringId *mElementNames;
        CIEC_ANY *mMembers;
      private:
        //!declared but undefined copy constructor as we don't want these specs to be directly copied.
        CStructSpecs(const CStructSpecs&);
    };

    void clear();

    static void findNextNonBlankSpace(const char** paRunner);

    bool initializeFromString(int *paLength, CIEC_ANY *paMember, const char** paRunner, bool* paErrorOcurred);

    const CStructSpecs* getSpecs() const {
      return reinterpret_cast<const CStructSpecs*>(getGenData());
    }

    CStructSpecs* getSpecs() {
      return reinterpret_cast<CStructSpecs*>(getGenData());
    }

    static CStringDictionary::TStringId parseNextElementId(const char *paRunner, int &paCounter);

};

#endif /*_FORTE_STRUCT_H_*/
