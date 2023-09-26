/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN
 *               2023 Martin Erich Jobst
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
 *    Martin Jobst - add equals function
 *                 - refactor struct memory layout
 *******************************************************************************/
#ifndef _FORTE_STRUCT_H_
#define _FORTE_STRUCT_H_

#include "forte_any_derived.h"

class CIEC_STRUCT : public CIEC_ANY_DERIVED{
  public:
    //! Indicator for invalid array member index positions
    static constexpr size_t csmNIndex = -1;

    CIEC_STRUCT() = default;

    ~CIEC_STRUCT() override = default;

    /*! \brief Get the Struct's type
     *
     *   With this command the type-ID of the struct can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - the type-ID of the struct.
     */
    virtual TForteUInt8 getASN1StructType() const {
      return e_APPLICATION + e_CONSTRUCTED + 1;
    }

    /*! \brief Get the Struct's size
     *
     *   With this command the size of the struct can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - the size of the struct.
     */
    virtual size_t getStructSize() const = 0;

    /*! \brief Get the Struct's elementNames
     *
     *   retrieve array of StringIDs of element names.
     *
     *   \param - No parameters necessary.
     *   \return - pointer to array of StringIds.
     */
    virtual const CStringDictionary::TStringId* elementNames() const = 0;

    /*! \brief Get the Struct's type name
     *
     *   With this command the type name of the struct can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - StringId of Struct's type name.
     */
    virtual CStringDictionary::TStringId getStructTypeNameID() const = 0;

    void setValue(const CIEC_ANY& paValue) override;

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
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
    int fromString(const char *paValue) override;

    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function to C++ data type.
     *   \param paValue       Pointer to char-array
     *   \param paBufferSize   Size of the provided buffer
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    int toString(char* paValue, size_t paBufferSize) const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override;

    /*! \brief helper method for accessing a member by index
     *
     * Mainly used for the generated accessor-functions.
     * Therefore the index is not range checked!
     *
     * \param paMemberIndex index into the member array
     * \return pointer to the member var
     */
    virtual CIEC_ANY *getMember(size_t paMemberIndex) = 0;

    virtual const CIEC_ANY *getMember(size_t paMemberIndex) const = 0;

    /*! \brief Get the struct's member var with the given name id
     *
     * \param paMemberNameId the string id of the member name
     * \return on a valid member name id a pointer to the member var otherwise 0
     */
    CIEC_ANY *getMemberNamed(CStringDictionary::TStringId paMemberNameId);

    /*! \brief Get the struct's member var with the given name
     *
     * \param paMemberName name of the member to be checked for
     * \return on a valid member name id a pointer to the member var otherwise 0
     */
    CIEC_ANY* getMemberNamed(const char * paMemberName);

    size_t getMemberIndex(CStringDictionary::TStringId paMemberNameId);

  protected:
    enum EASN1Tags {
      e_UNIVERSAL = 0, e_APPLICATION = 64, e_CONTEXT = 128, e_PRIVATE = 192
    };
    enum EASN1Encoding {
      e_PRIMITIVE = 0, e_CONSTRUCTED = 32
    };

    CIEC_STRUCT(const CIEC_STRUCT &) {};

    CIEC_STRUCT(CIEC_STRUCT &&) {};

    CIEC_STRUCT &operator=(const CIEC_STRUCT &) {
      return *this;
    }

    CIEC_STRUCT &operator=(CIEC_STRUCT &&) {
      return *this;
    };

private:
    static void findNextNonBlankSpace(const char** paRunner);

    int initializeFromString(const char *paValue);

    static CStringDictionary::TStringId parseNextElementId(const char *&paRunner);
};

#endif /*_FORTE_STRUCT_H_*/
