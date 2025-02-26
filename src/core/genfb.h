/*******************************************************************************
 * Copyright (c) 2018, 2024 Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *    Martin Erich Jobst - add generic event accessor helpers
 *******************************************************************************/

#pragma once

#include "funcbloc.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_gen.cpp"
#endif

template <class T>
class CGenFunctionBlock : public T {
    static_assert(std::is_base_of_v<CFunctionBlock, T>, "T must be a subclass of CFunctionBlock");

  public:

    /*!\brief The getFBType method is used by the Query command to get the instances correct type name (eg. "CLIENT_3_2")
    * \return pointer to typename string
    */
    CStringDictionary::TStringId getFBTypeId() const override {
      return mConfiguredFBTypeNameId;
    }

    bool configureFB(const char *paConfigString) override;

    CIEC_ANY* getDI(TPortId paDINum) override {
      return mDIs[paDINum];
    }

    CIEC_ANY* getDO(TPortId paDONum) override {
      return mDOs[paDONum];
    }

  protected:
    template<typename... Args>
    CGenFunctionBlock(forte::core::CFBContainer &paContainer, const CStringDictionary::TStringId paInstanceNameId,
                      Args &&... args) :
            T(paContainer, mGenInterfaceSpec, paInstanceNameId, std::forward<Args>(args)...),
            mEOConns(nullptr), mDIConns(nullptr), mDOConns(nullptr), mDIs(nullptr), mDOs(nullptr),
            mAdapters(nullptr),
            mConfiguredFBTypeNameId(CStringDictionary::scmInvalidStringId),
            mGenInterfaceSpec(),
            mFBConnData(nullptr), mFBVarsData(nullptr) {
    }

    template<typename... Args>
    CGenFunctionBlock(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec,
                      const CStringDictionary::TStringId paInstanceNameId, Args &&... args) :
            T(paContainer, mGenInterfaceSpec, paInstanceNameId, std::forward<Args>(args)...),
            mEOConns(nullptr), mDIConns(nullptr), mDOConns(nullptr), mDIs(nullptr), mDOs(nullptr),
            mAdapters(nullptr),
            mConfiguredFBTypeNameId(CStringDictionary::scmInvalidStringId),
            mGenInterfaceSpec(paInterfaceSpec),
            mFBConnData(nullptr), mFBVarsData(nullptr) {
    }

    ~CGenFunctionBlock() override;

    bool initialize();

    template<typename ...Args>
    void writeArguments(Args &&...paArgs) {
      TPortId index = 0;
      (writeArgument(index++, std::forward<Args>(paArgs)), ...);
    }

    template<typename Arg>
    void writeArgument(TPortId paIndex, const Arg &paArg) {
      const SFBInterfaceSpec &interfaceSpec = T::getFBInterfaceSpec();
      if (paIndex < interfaceSpec.mNumDIs) {
        T::getDIFromPortId(paIndex)->setValue(paArg);
      } else if (paIndex < interfaceSpec.mNumDIs + interfaceSpec.mNumDIOs) {
        T::getDIOFromPortId(paIndex - interfaceSpec.mNumDIs)->setValue(paArg);
      } else if (paIndex < interfaceSpec.mNumDIs + interfaceSpec.mNumDIOs + interfaceSpec.mNumDOs) {
        T::getDOFromPortId(paIndex - interfaceSpec.mNumDIs - interfaceSpec.mNumDIOs)->setValue(paArg);
      }
    }

    template<typename ...Args>
    void writeInputArguments(Args &&...paArgs) {
      TPortId index = 0;
      (writeInputArgument(index++, std::forward<Args>(paArgs)), ...);
    }

    template<typename Arg>
    void writeInputArgument(TPortId paIndex, const Arg &paArg) {
      const SFBInterfaceSpec &interfaceSpec = T::getFBInterfaceSpec();
      if (paIndex < interfaceSpec.mNumDIs) {
        T::getDIFromPortId(paIndex)->setValue(paArg);
      } else if (paIndex < interfaceSpec.mNumDIs + interfaceSpec.mNumDIOs) {
        T::getDIOFromPortId(paIndex - interfaceSpec.mNumDIs)->setValue(paArg);
      } // skip DO
    }

    template<typename ...Args>
    void readOutputArguments(Args &&...paArgs) {
      TPortId index = 0;
      (readOutputArgument(index++, std::forward<Args>(paArgs)), ...);
    }

    template<typename Arg>
    void readOutputArgument(TPortId paIndex, Arg &&paArg) {
      const SFBInterfaceSpec &interfaceSpec = T::getFBInterfaceSpec();
      if (paIndex < interfaceSpec.mNumDIs) {
        // do nothing
      } else if (paIndex < interfaceSpec.mNumDIs + interfaceSpec.mNumDIOs) {
        if constexpr (std::is_const_v<std::remove_reference_t<Arg>>) {
          DEVLOG_ERROR("[CGenFunctionBlock] Trying to pass const argument to in/out variable\n");
        } else {
          paArg.setValue(T::getDIOFromPortId(paIndex - interfaceSpec.mNumDIs)->unwrap());
        }
      } else if (paIndex < interfaceSpec.mNumDIs + interfaceSpec.mNumDIOs + interfaceSpec.mNumDOs) {
        if constexpr (std::is_const_v<std::remove_reference_t<Arg>>) {
          DEVLOG_ERROR("[CGenFunctionBlock] Trying to pass const argument to output variable\n");
        } else {
          paArg.setValue(T::getDOFromPortId(paIndex - interfaceSpec.mNumDIs - interfaceSpec.mNumDIOs)->unwrap());
        }
      }
    }

    static void generateGenericInterfacePointNameArray(const char * const paPrefix,
        CStringDictionary::TStringId* paNamesArayStart,
        size_t paNumGenericDataPoints);
    static void generateGenericDataPointArrays(const char * const paPrefix,
        CStringDictionary::TStringId* paDataTypeNamesArrayStart,
        CStringDictionary::TStringId* paNamesArrayStart, size_t paNumGenericDataPoints);

    static size_t getDataPointSpecSize(const CIEC_ANY &paValue);
    static void fillDataPointSpec(const CIEC_ANY &paValue, CStringDictionary::TStringId *&paDataTypeIds);

    static size_t calculateFBConnDataSize(const SFBInterfaceSpec &paInterfaceSpec);

    static size_t calculateFBVarsDataSize(const SFBInterfaceSpec &paInterfaceSpec);

    void setupFBInterface();

    void freeFBInterfaceData();

    CEventConnection *getEOConUnchecked(TPortId paEONum) override {
      return (mEOConns + paEONum);
    }

    CDataConnection *getDOConUnchecked(TPortId paDONum) override {
      return mDOConns + paDONum;
    }

    CDataConnection **getDIConUnchecked(TPortId paDINum) override {
      return mDIConns + paDINum;
    }

    CAdapter *getAdapterUnchecked(TPortId paAdapterNum) override {
      return mAdapters[paAdapterNum];
    }

    SFBInterfaceSpec &getGenInterfaceSpec() {
      return mGenInterfaceSpec;
    }

    CEventConnection *mEOConns; //!< A list of event connections pointers storing for each event output the event connection. If the output event is not connected the pointer is nullptr.
    CDataConnection **mDIConns; //!< A list of data connections pointers storing for each data input the data connection. If the data input is not connected the pointer is nullptr.
    CDataConnection *mDOConns; //!< A list of data connections pointers storing for each data output the data connection. If the data output is not connected the pointer is nullptr.
    CIEC_ANY **mDIs; //!< A list of pointers to the data inputs. This allows to implement a general getDataInput()
    CIEC_ANY **mDOs; //!< A list of pointers to the data outputs. This allows to implement a general getDataOutput()
    CAdapter **mAdapters; //!< A list of pointers to the adapters. This allows to implement a general getAdapter().
  private:
    /*! \brief parse the config string and generate the according interface specification
     *
     * This function is to be implemented by a generic fb and should parse the given interface
     * string and generate an according interface specification. The resulting interface specification
     * has to be set in the paInterfaceSpec parameter.
     *
     * @paConfigString the string sent from the id for configuring this fb (e.g., PUBLISH_2)
     * @paInterfaceSpec the interface data structure where the resulting interface config must be stored
     * @retval true if the string could be parsed, false if the string is invalid and no interface spec could be derived from it
     */
    virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) = 0;

    void setConfiguredTypeNameId( CStringDictionary::TStringId paTypeNameId){
      mConfiguredFBTypeNameId = paTypeNameId;
    }

    void setupAdapters(TForteByte *paFBData);

    CStringDictionary::TStringId mConfiguredFBTypeNameId;
    SFBInterfaceSpec mGenInterfaceSpec;  //!< the interface spec for this specific instance of generic FB
    void *mFBConnData; //!< Connection data buffer
    void *mFBVarsData; //!< Variable data buffer
};


template<class T>
CGenFunctionBlock<T>::~CGenFunctionBlock(){
    freeFBInterfaceData();  //clean the interface and connections first.
    mGenInterfaceSpec = {}; //this stops the base classes from any wrong clean-up
}

template<class T>
bool CGenFunctionBlock<T>::initialize() {
  setupFBInterface();
  return T::initialize();
}

template<class T>
bool CGenFunctionBlock<T>::configureFB(const char *paConfigString){
  setConfiguredTypeNameId(CStringDictionary::insert(paConfigString));
  if(createInterfaceSpec(paConfigString, mGenInterfaceSpec)){
    setupFBInterface();
    return true;
  }
  return false;
}

template<class T>
void CGenFunctionBlock<T>::generateGenericInterfacePointNameArray(const char * const paPrefix, CStringDictionary::TStringId* paNamesArayStart,
    size_t paNumGenericDataPoints) {
  size_t len = strlen(paPrefix);

  unsigned int noOfDigits = 0;
  {
    size_t tempNum = paNumGenericDataPoints;
    while(tempNum){
      tempNum /= 10;
      noOfDigits++;
    }
  }

  if(cgIdentifierLength >= (len + noOfDigits)){
    TIdentifier acBuffer;
    memcpy(acBuffer, paPrefix, len);
    for(size_t i = 0; i <= noOfDigits; i++){
      acBuffer[len + i] = '\0';
    }

    for(size_t i = 1; i <= paNumGenericDataPoints; i++) {
      if(i < 10){ //1 digit
        acBuffer[len] = static_cast<char>(0x30 + i);
      }
      else if(i < 100){ //2 digits
        if(0 == i % 10){
          acBuffer[len] = static_cast<char>(0x30 + (i % 100 / 10));
        }
        acBuffer[len + 1] = static_cast<char>(0x30 + i % 10);
      }
      else{ //3 digits
        if(0 == i % 100){
          acBuffer[len] = static_cast<char>(0x30 + (i / 100));
        }
        if(0 == i % 10){
          acBuffer[len + 1] = static_cast<char>(0x30 + (i % 100 / 10));
        }
        acBuffer[len + 2] = static_cast<char>(0x30 + i % 10);
      }
      paNamesArayStart[i - 1] = CStringDictionary::insert(acBuffer);
    }
  }
  else{
    DEVLOG_ERROR("CFunctionBlock::generateGenericInterfacePointNameArray won't be able to create all the generics since %s is too long to hold until %d", paPrefix, paNumGenericDataPoints);
  }
}

template<class T>
void CGenFunctionBlock<T>::generateGenericDataPointArrays(const char * const paPrefix, CStringDictionary::TStringId* paDataTypeNamesArrayStart,
    CStringDictionary::TStringId* paNamesArrayStart, size_t paNumGenericDataPoints) {
  generateGenericInterfacePointNameArray(paPrefix, paNamesArrayStart, paNumGenericDataPoints);

  for(size_t i = 0; i < paNumGenericDataPoints; i++){
    paDataTypeNamesArrayStart[i] = g_nStringIdANY;
  }
}

template<class T>
size_t CGenFunctionBlock<T>::getDataPointSpecSize(const CIEC_ANY &paValue) {
  CIEC_ANY::EDataTypeID dataTypeId = paValue.getDataTypeID();
  if(dataTypeId == CIEC_ANY::e_ARRAY) {
    const CIEC_ARRAY &arrayValue = static_cast<const CIEC_ARRAY&>(paValue);
    if(arrayValue.size() > 0) {
      return 3 + getDataPointSpecSize(arrayValue[arrayValue.getLowerBound()]);
    }
    return 4;
  }
  return 1;
}

template<class T>
void CGenFunctionBlock<T>::fillDataPointSpec(const CIEC_ANY &paValue, CStringDictionary::TStringId *&paDataTypeIds) {
  *(paDataTypeIds++) = paValue.getTypeNameID();
  CIEC_ANY::EDataTypeID dataTypeId = paValue.getDataTypeID();
  if(dataTypeId == CIEC_ANY::e_ARRAY) {
    const CIEC_ARRAY &arrayValue = static_cast<const CIEC_ARRAY&>(paValue);
    *(paDataTypeIds++) = static_cast<CStringDictionary::TStringId>(arrayValue.getLowerBound());
    *(paDataTypeIds++) = static_cast<CStringDictionary::TStringId>(arrayValue.getUpperBound());
    if(arrayValue.size() > 0) {
      fillDataPointSpec(arrayValue[arrayValue.getLowerBound()], paDataTypeIds);
    } else {
      *(paDataTypeIds++) = arrayValue.getElementTypeNameID();
    }
  }
}

template<class T>
size_t CGenFunctionBlock<T>::calculateFBConnDataSize(const SFBInterfaceSpec &paInterfaceSpec) {
  return sizeof(CEventConnection) * paInterfaceSpec.mNumEOs +
         sizeof(TDataConnectionPtr) * paInterfaceSpec.mNumDIs +
         sizeof(CDataConnection) * paInterfaceSpec.mNumDOs;
}

template<class T>
size_t CGenFunctionBlock<T>::calculateFBVarsDataSize(const SFBInterfaceSpec &paInterfaceSpec) {
  size_t result = 0;
  const CStringDictionary::TStringId *pnDataIds;

  result += paInterfaceSpec.mNumDIs * sizeof(CIEC_ANY *);
  pnDataIds = paInterfaceSpec.mDIDataTypeNames;
  for (TPortId i = 0; i < paInterfaceSpec.mNumDIs; ++i) {
    result += T::getDataPointSize(pnDataIds);
  }

  result += paInterfaceSpec.mNumDOs * sizeof(CIEC_ANY *);
  pnDataIds = paInterfaceSpec.mDODataTypeNames;
  for (TPortId i = 0; i < paInterfaceSpec.mNumDOs; ++i) {
    result += T::getDataPointSize(pnDataIds) * 2; // * 2 for connection buffer value
  }

  result += paInterfaceSpec.mNumAdapters * sizeof(TAdapterPtr);
  return result;
}

template<class T>
void CGenFunctionBlock<T>::setupFBInterface() {
  freeFBInterfaceData();

  size_t connDataSize = calculateFBConnDataSize(T::getFBInterfaceSpec());
  size_t varsDataSize = calculateFBVarsDataSize(T::getFBInterfaceSpec());
  mFBConnData = connDataSize ? operator new(connDataSize) : nullptr;
  mFBVarsData = varsDataSize ? operator new(varsDataSize) : nullptr;

  auto *connData = reinterpret_cast<TForteByte *>(mFBConnData);
  auto *varsData = reinterpret_cast<TForteByte *>(mFBVarsData);

  TPortId i;
  if (T::getFBInterfaceSpec().mNumEOs) {
    mEOConns = reinterpret_cast<CEventConnection *>(connData);

    for (i = 0; i < T::getFBInterfaceSpec().mNumEOs; ++i) {
      //create an event connection for each event output and initialize its source port
      new(connData)CEventConnection(this, i);
      connData += sizeof(CEventConnection);
    }
  } else {
    mEOConns = nullptr;
  }

  const CStringDictionary::TStringId *pnDataIds;
  if (T::getFBInterfaceSpec().mNumDIs) {
    mDIConns = reinterpret_cast<TDataConnectionPtr *>(connData);
    connData += sizeof(TDataConnectionPtr) * T::getFBInterfaceSpec().mNumDIs;

    mDIs = reinterpret_cast<CIEC_ANY **>(varsData);
    varsData += T::getFBInterfaceSpec().mNumDIs * sizeof(CIEC_ANY *);

    pnDataIds = T::getFBInterfaceSpec().mDIDataTypeNames;
    for (i = 0; i < T::getFBInterfaceSpec().mNumDIs; ++i) {
      mDIs[i] = T::createDataPoint(pnDataIds, varsData);
      mDIConns[i] = nullptr;
    }
  } else {
    mDIConns = nullptr;
    mDIs = nullptr;
  }

  if (T::getFBInterfaceSpec().mNumDOs) {
    //let mDOConns point to the first data output connection
    mDOConns = reinterpret_cast<CDataConnection *>(connData);

    mDOs = reinterpret_cast<CIEC_ANY **>(varsData);
    varsData += T::getFBInterfaceSpec().mNumDOs * sizeof(CIEC_ANY *);

    pnDataIds = T::getFBInterfaceSpec().mDODataTypeNames;
    for (i = 0; i < T::getFBInterfaceSpec().mNumDOs; ++i) {
      mDOs[i] = T::createDataPoint(pnDataIds, varsData);
      CIEC_ANY* connVar = mDOs[i]->clone(varsData);
      varsData += connVar->getSizeof();
      new(connData)CDataConnection(this, i, connVar);
      connData += sizeof(CDataConnection);
    }
  } else {
    mDOConns = nullptr;
    mDOs = nullptr;
  }
  if (T::getFBInterfaceSpec().mNumAdapters) {
    setupAdapters(varsData);
  }

  T::setupInputConnectionTrackingData();

#ifdef FORTE_SUPPORT_MONITORING
  T::setupEventMonitoringData();
#endif
}

template<class T>
void CGenFunctionBlock<T>::freeFBInterfaceData(){
  if(nullptr != mEOConns) {
    std::destroy_n(mEOConns, T::getFBInterfaceSpec().mNumEOs);
  }

  if(nullptr != mDOConns) {
    for (TPortId i = 0; i < T::getFBInterfaceSpec().mNumDOs; ++i) {
      if(CIEC_ANY* value = mDOConns[i].getValue(); nullptr != value) {
        std::destroy_at(value);
      }
    }
    std::destroy_n(mDOConns, T::getFBInterfaceSpec().mNumDOs);
  }

  if(nullptr != mDIs) {
    for (TPortId i = 0; i < T::getFBInterfaceSpec().mNumDIs; ++i) {
      if(CIEC_ANY* value = mDIs[i]; nullptr != value) {
        std::destroy_at(value);
      }
    }
  }

  if(nullptr != mDOs) {
    for (TPortId i = 0; i < T::getFBInterfaceSpec().mNumDOs; ++i) {
      if(CIEC_ANY* value = mDOs[i]; nullptr != value) {
        std::destroy_at(value);
      }
    }
  }

  if(nullptr != mAdapters) {
    for (TPortId i = 0; i < T::getFBInterfaceSpec().mNumAdapters; ++i) {
      T::destroyAdapter(mAdapters[i]);
    }
  }

  operator delete(mFBConnData);
  mFBConnData = nullptr;
  operator delete(mFBVarsData);
  mFBVarsData = nullptr;

#ifdef FORTE_SUPPORT_MONITORING
  T::freeEventMonitoringData();
#endif //FORTE_SUPPORT_MONITORING
}

template<class T>
void CGenFunctionBlock<T>::setupAdapters(TForteByte *paFBData){
  if((nullptr != paFBData) && (T::getFBInterfaceSpec().mNumAdapters)) {
    mAdapters = reinterpret_cast<TAdapterPtr *>(paFBData);
    for(TPortId i = 0; i < T::getFBInterfaceSpec().mNumAdapters; ++i) {
      mAdapters[i] = T::createAdapter(T::getFBInterfaceSpec().mAdapterInstanceDefinition[i], static_cast<TForteUInt8>(i));
    }
  }
}
