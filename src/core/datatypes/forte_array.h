/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, nxtControl GmbH, fortiss GmbH
 *               2022 Primetals Technologies Austria GmbH
 *               2022 Martin Erich Jobst
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
 *    Martin Melik Merkumians, Martin Jobst - added ctors for copying one type
 *      to another and iterators
 *******************************************************************************/
#ifndef _FORTE_ARRAY_H_
#define _FORTE_ARRAY_H_

#include "forte_array_common.h"
#include <stdexcept>
#include <devlog.h>

#ifdef FORTE_SUPPORT_ARRAYS

template <typename T, intmax_t lowerBound, intmax_t upperBound>
class CIEC_ARRAY_FIXED;

template <typename T>
class CIEC_ARRAY_VARIABLE;

/*!\ingroup COREDTS CIEC_ARRAY represents the array data type according to IEC 61131.
 */
template<typename T = CIEC_ANY>
class CIEC_ARRAY : public CIEC_ARRAY_COMMON<T> {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename CIEC_ARRAY_COMMON<T>::value_type;
    using pointer = typename CIEC_ARRAY_COMMON<T>::pointer;
    using const_pointer = typename CIEC_ARRAY_COMMON<T>::const_pointer;
    using reference = typename CIEC_ARRAY_COMMON<T>::reference;
    using const_reference = typename CIEC_ARRAY_COMMON<T>::const_reference;
    using iterator = typename CIEC_ARRAY_COMMON<T>::iterator;
    using const_iterator = typename CIEC_ARRAY_COMMON<T>::const_iterator;

    CIEC_ARRAY(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType) {
      setup(paLength, paArrayType);
    }

    CIEC_ARRAY(const CIEC_ARRAY &paValue) {
      size_t nSize = paValue.size();

      if (0 != nSize) {
        CIEC_ARRAY_COMMON<T>::setGenData(reinterpret_cast<TForteByte *>(new CArraySpecs(nSize)));
        paValue.getReferenceElement()->clone(reinterpret_cast<TForteByte *>(getReferenceElement()));

        CIEC_ANY *destArray = getArray();
        const CIEC_ANY *srcArray = paValue.getArray();

        for (size_t i = 0; i < nSize; ++i) {
          // as we destArray is already the target place we don't need to store the resulting pointer
          srcArray[i].clone(reinterpret_cast<TForteByte *>(&(destArray[i]))); // clone is faster than the CTypeLib call
        }
      }
    }

    template<typename U>
    CIEC_ARRAY(const CIEC_ARRAY_VARIABLE<U> &paSource) {
      if (paSource.getUpperBound() > 0) {
        setup(static_cast<TForteUInt16>(paSource.getUpperBound()), paSource[paSource.getLowerBound()].getTypeNameID());
        for (intmax_t i = paSource.getLowerBound() < 0 ? 0 : paSource.getLowerBound(); i < paSource.getUpperBound(); ++i) {
          static_cast<T&>((*this)[i]) = paSource[i];
        }
      }
    }

    // Plain old arrays start implicitly at 0 and their size is their upperBound
    template<typename U, intmax_t lowerBound, intmax_t upperBound>
    CIEC_ARRAY(const CIEC_ARRAY_FIXED<U, lowerBound, upperBound> &paSource) {
      if (paSource.getUpperBound() > 0) {
        setup(static_cast<TForteUInt16>(paSource.getUpperBound()), paSource[paSource.getLowerBound()].getTypeNameID());
        for (intmax_t i = paSource.getLowerBound() < 0 ? 0 : paSource.getLowerBound(); i < paSource.getUpperBound(); ++i)
        {
          static_cast<T&>((*this)[i]) = paSource[i];
        }
      }
    }

    ~CIEC_ARRAY() override {
      clear();
    }

    //!Function to configure the array if it is created via the typelib
    void setup(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType) {
      if (0 != paLength) {
        clear();

        CIEC_ARRAY_COMMON<T>::setGenData(reinterpret_cast<TForteByte *>(new CArraySpecs(paLength)));

        CIEC_ANY *destArray = getArray();

        // The reference element is used
        // - to initialize the elements not set by fromString or deserialize
        // - to get the element id, even if the array has a zero size (not enabled yet, open to discussion)
        CIEC_ANY *refElement = getReferenceElement();

        if (CTypeLib::createDataTypeInstance(paArrayType, reinterpret_cast<TForteByte *>(refElement))) {
          for (size_t i = 0; i < paLength; ++i) {
            // as we acDataBuf is already the target place we don't need to store the resulting pointer
            refElement->clone(reinterpret_cast<TForteByte *>(&(destArray[i]))); // clone is faster than the CTypeLib call
          }
        } else { // datatype not found, clear everything
          clear();
        }
      }
    }

    /*! \brief Get the array's size
     *
     *   With this command the size of the array can be evaluated.
     *
     *   \param - No parameters necessary.
     *   \return - the length of the array.
     */
    [[nodiscard]] size_t size() const override {
      return (nullptr != getSpecs()) ? getSpecs()->mLength : static_cast<size_t>(0);
    }

    [[nodiscard]] intmax_t getLowerBound() const override {
      return 0;
    }

    [[nodiscard]] intmax_t getUpperBound() const override {
      return static_cast<intmax_t>(size() - 1);
    }

    [[nodiscard]] iterator begin() override {
      return static_cast<iterator>(getArray());
    }

    [[nodiscard]] iterator end() override {
      return static_cast<iterator>(getArray() + getUpperBound() + 1);
    }

    [[nodiscard]] const_iterator begin() const override {
      return static_cast<const_iterator>(getArray());
    }

    [[nodiscard]] const_iterator end() const override {
      return static_cast<const_iterator>(getArray() + getUpperBound() + 1);
    }

    /*! \brief Operator: CIEC_INT data type [intmax_t] data type
     *
     *   This command implements the index operator for a C++ intmax_t index.
     *   \param paIndex  Index
     *   \return Can be the following response:
     *     - Pointer to given object.
     *     - nullptr if outside the array range
     */
    [[nodiscard]] reference at(intmax_t paIndex) override {
      if (!(paIndex >= 0 && static_cast<size_t>(paIndex) < size())) {
        std::__throw_out_of_range_fmt(__N("array::at: __n (which is %zu) "
                                          ">= _Nm (which is %zu)"),
                                      paIndex, size());
      } 
      return begin()[paIndex];
    }

    [[nodiscard]] const_reference at(intmax_t paIndex) const override {
      if (!(paIndex >= 0 && static_cast<size_t>(paIndex) < size()))
      {
        std::__throw_out_of_range_fmt(__N("array::at: __n (which is %zu) "
                                          ">= _Nm (which is %zu)"),
                                      paIndex, size());
      }
      return begin()[paIndex];
    }

    [[nodiscard]] reference operator[](intmax_t paIndex) override {
      return at(paIndex);
    }

    [[nodiscard]] const_reference operator[](intmax_t paIndex) const override {
      return at(paIndex);
    }

    CIEC_ARRAY<T>& operator =(const CIEC_ARRAY_COMMON<T> &paSource) override {
      const intmax_t sourceLowerBound = paSource.getLowerBound();
      const intmax_t sourceUpperBound = paSource.getUpperBound();
      const intmax_t lowerBoundOffset = getLowerBound() - sourceLowerBound;
      const intmax_t upperBoundOffset = getUpperBound() - sourceUpperBound;
  
      auto sourceIteratorBegin = paSource.begin();
      auto sourceIteratorEnd = paSource.end();
      auto targetIteratorBegin = begin();
      if ((sourceLowerBound <= getUpperBound()) && (sourceUpperBound >= getLowerBound())) {
      // Target lowerBound is a bigger number than the source, so all elements below the target lowerbound cannot be copied, so shift start to first element which will be copied
        if (getLowerBound() >= sourceLowerBound) {
          sourceIteratorBegin += lowerBoundOffset;
        } else {
          targetIteratorBegin -= lowerBoundOffset; // lowerBoundOffset is negative here
        }

        if (getUpperBound() < sourceUpperBound) {
          sourceIteratorEnd += upperBoundOffset; // upperBoundOffset is negative here
        }

        for (auto element = sourceIteratorBegin; element != sourceIteratorEnd; ++element, ++targetIteratorBegin) {
          targetIteratorBegin->setValue(*element);
        }
      }
      return *this;
    }

    CIEC_ARRAY<T> &operator=(const CIEC_ARRAY<T> &paValue) {
      this->operator=(static_cast<CIEC_ARRAY_COMMON<T> &>(paValue));
      return *this;
    }

    CIEC_ARRAY<T> &operator=(std::initializer_list<T> init) override {
      auto copyEnd = init.end();
      if (init.size() > size()) {
        copyEnd -= init.size() - size();
        DEVLOG_WARNING("Initializer list longer than array\n");
      }
      for (auto iterSource = init.begin(), iterDest = begin(); iterSource != copyEnd; ++iterSource, ++iterDest ) {
        iterDest->setValue(*iterSource);
      }
      return *this;
    }

    /*! \brief Get the data type of elements inside the array
     *
     */
    [[nodiscard]] CIEC_ANY::EDataTypeID getElementDataTypeID() const override {
      return getReferenceElement() != 0 ? getReferenceElement()->getDataTypeID() : CIEC_ANY::e_ANY;
    }

    void setValue(const CIEC_ANY &paValue) override {
      if (paValue.getDataTypeID() == CIEC_ANY::e_ARRAY)
      {
        // TODO maybe check if array data is of same type or castable
        const CIEC_ANY *poSrcArray = static_cast<const CIEC_ARRAY &>(paValue).getArray();

        size_t unSize = (size() < static_cast<const CIEC_ARRAY &>(paValue).size()) ? size() : static_cast<const CIEC_ARRAY &>(paValue).size();

        for (intmax_t i = 0; i < static_cast<intmax_t>(unSize); ++i, ++poSrcArray) {
          (*this)[i].setValue(*poSrcArray);
        }
      }
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
    [[nodiscard]] int fromString(const char *paValue) override {
      int nRetVal = -1;
      const char *pcRunner = paValue;
      if('[' == paValue[0]){
    
        CIEC_ANY *poBufVal = nullptr;
        intmax_t i = 0;
        size_t unArraySize = size();
        int nValueLen;
    
        while(('\0' != *pcRunner) && (']' != *pcRunner)) {
          pcRunner++;
          findNextNonBlankSpace(&pcRunner);
    
          initializeFromString(unArraySize, &nValueLen, i, pcRunner, &poBufVal);
    
          while(' ' == pcRunner[nValueLen]){
            nValueLen++;
          }
          if((0 < nValueLen) && ((',' == pcRunner[nValueLen]) || (']' == pcRunner[nValueLen]))){
            pcRunner += nValueLen;
          }
          else{
            //we have an error or the end bracket
            break;
          }
          i++;
        }
        if(*pcRunner == ']'){
          //arrays have to and on a closing bracket
          nRetVal = static_cast<int>(pcRunner - paValue + 1); //+1 from the closing bracket
          // For the rest of the array size copy the default element
          for(; i < static_cast<intmax_t>(unArraySize); ++i) {
            (*this)[i].setValue(*(getReferenceElement()));
          }
        }
        delete poBufVal;
      }
      return nRetVal;
    }
    /*! \brief Converts data type value to array
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC61131 conform data type (array format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param pa_rsValue  Reference to the given String
     *   \return number of bytes used in the buffer
     *           -1 on error
     */
    [[nodiscard]] int toString(char* paValue, size_t paBufferSize) const override {
      int nBytesUsed = -1;

      if (paBufferSize) {
        *paValue = '[';
        paValue++;
        paBufferSize--;
        nBytesUsed = 1;
        size_t unSize = size();
        const CIEC_ANY *poArray = getArray();
        for (size_t i = 0; i < unSize; ++i, ++poArray) {
          int nUsedBytesByElement = poArray->toString(paValue, paBufferSize);
          if (-1 == nUsedBytesByElement) {
            return -1;
          }
          paBufferSize -= static_cast<size_t>(nUsedBytesByElement);
          paValue += nUsedBytesByElement;
          if (!paBufferSize) {
            return -1;
          }

          nBytesUsed += nUsedBytesByElement;

          if (i != unSize - 1) {
            *paValue = ',';
            paValue++;

            ++nBytesUsed;
            paBufferSize--;
          }
        }
        if (paBufferSize < 2) {
          return -1;
        }
        *paValue = ']';
        paValue[1] = '\0';
        nBytesUsed++;
      }

      return nBytesUsed;
    }

    [[nodiscard]] size_t getToStringBufferSize() const override {
      size_t retVal = 3; // 2 bytes for the open and closing brackets and one for the '\0'
      size_t nSize = size();
      retVal += (nSize > 1) ? (nSize - 1) : 0; // for the commas between the elements

      const CIEC_ANY *members = getArray();
      if (0 != members) {
        switch (getElementDataTypeID()) { // in these cases, the length of the elements are not always the same
        case CIEC_ANY::e_WSTRING:
        case CIEC_ANY::e_STRING: // quotes or double quotes are already counted in ANY_STRING
        case CIEC_ANY::e_ARRAY:
        case CIEC_ANY::e_STRUCT:
          for (size_t i = 0; i < nSize; i++) {
            retVal += (members[i].getToStringBufferSize() - 1); //-1 since the \0 of each element is counted as just one at the beginning
          }
          break;
        default:
          retVal += nSize * (getReferenceElement()->getToStringBufferSize() - 1); //-1 since the \0 of each element is counted as just one at the beginning
          break;
        }
      }

      return retVal;
    }
  
  protected:
    CIEC_ARRAY() = default;

  private:
    //This constructor is only to be used by the create instance method

    class CArraySpecs {
      public:
        explicit CArraySpecs(size_t paLength) :
            mLength(paLength) {
          mArray = new CIEC_ANY[paLength + 1];  //store an additional reference element in the first place
        }

        ~CArraySpecs() {
          delete[] mArray;
        }

        size_t mLength;

        [[nodiscard]] CIEC_ANY *getRefElement(){
          return mArray;
        }

        [[nodiscard]] const CIEC_ANY *getRefElement() const{
          return mArray;
        }

        [[nodiscard]] CIEC_ANY *getArrayContent(){
          return &(mArray[1]);          //the array content starts from the first entry in our array
        }

        [[nodiscard]] const CIEC_ANY *getArrayContent() const {
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
    [[nodiscard]] CIEC_ANY *getArray() {
      return (nullptr != getSpecs()) ? getSpecs()->getArrayContent() : static_cast<CIEC_ANY *>(nullptr);
    }

    [[nodiscard]] const CIEC_ANY *getArray() const{
      return (nullptr != getSpecs()) ? getSpecs()->getArrayContent() : static_cast<const CIEC_ANY *>(nullptr);
    }

    [[nodiscard]] CIEC_ANY *getReferenceElement() {
      return (nullptr != getSpecs()) ? getSpecs()->getRefElement() : static_cast<CIEC_ANY *>(nullptr);
    }

    [[nodiscard]] const CIEC_ANY *getReferenceElement() const {
      return (nullptr != getSpecs()) ? getSpecs()->getRefElement() : static_cast<CIEC_ANY *>(nullptr);
    }

    [[nodiscard]] const CArraySpecs* getSpecs() const {
      return reinterpret_cast<const CArraySpecs *>(CIEC_ARRAY_COMMON<T>::getGenData());
    }

    [[nodiscard]] CArraySpecs* getSpecs() {
      return reinterpret_cast<CArraySpecs *>(CIEC_ARRAY_COMMON<T>::getGenData());
    }

    void clear() {
      if (CIEC_ARRAY_COMMON<T>::getGenData()) {
        delete getSpecs();
        CIEC_ARRAY_COMMON<T>::setGenData(nullptr);
      }
    }

    void initializeFromString(size_t paArraySize, int *paValueLen, intmax_t paPosition, const char *paSrcString, CIEC_ANY **paBufVal) {
      if (paPosition < static_cast<intmax_t>(paArraySize) ){
        *paValueLen = ((*this)[paPosition]).fromString(paSrcString);
      } else {
        if (nullptr == *paBufVal) {
          *paBufVal = (getReferenceElement())->clone(nullptr);
        }
        *paValueLen = (*paBufVal)->fromString(paSrcString);
      }
    }

    static void findNextNonBlankSpace(const char** paRunner) {
      while (' ' == **paRunner) {
        (*paRunner)++;
      }
    }
};

class CIEC_ARRAY_TYPELIB : public CIEC_ARRAY<CIEC_ANY> {
  public:
    using CIEC_ARRAY<CIEC_ANY>::operator=;

    CIEC_ARRAY_TYPELIB() = default;

    CIEC_ARRAY_TYPELIB(const CIEC_ARRAY_TYPELIB&) = default;

    CIEC_ARRAY_TYPELIB(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType) {
      setup(paLength, paArrayType);
    }

    static CIEC_ANY *createDataType(TForteByte *pa_acDataBuf) {
      return (nullptr != pa_acDataBuf) ? new (pa_acDataBuf) CIEC_ARRAY_TYPELIB() : new CIEC_ARRAY_TYPELIB;
    };
    
    const static CTypeLib::CDataTypeEntry csm_oFirmwareDataTypeEntry_ARRAY_TYPELIB;
    [[nodiscard]] CIEC_ANY* clone(TForteByte *pa_acDataBuf) const override {
      FORTE_STATIC_ASSERT((sizeof(CIEC_ANY) == sizeof(CIEC_ARRAY_TYPELIB)), DataTypeNotTheSameSizeAsANY);
      return (nullptr != pa_acDataBuf) ? new (pa_acDataBuf) CIEC_ARRAY_TYPELIB(*this) : new CIEC_ARRAY_TYPELIB(*this);
    }

    [[nodiscard]] CStringDictionary::TStringId getTypeNameID() const override {
      return CIEC_ARRAY_TYPELIB::csm_oFirmwareDataTypeEntry_ARRAY_TYPELIB.getTypeNameId();
    }
    static int dummyInit();
};

#endif /* FORTE_SUPPORT_ARRAYS */

#endif /*_FORTE_ARRAY_H_*/
