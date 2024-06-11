/*******************************************************************************
 * Copyright (c) 2018 Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_gen.cpp"
#endif

template<class T>
CGenFunctionBlock<T>::CGenFunctionBlock(forte::core::CFBContainer &paContainer, const CStringDictionary::TStringId paInstanceNameId) :
        T(paContainer, nullptr, paInstanceNameId),
        mEOConns(nullptr), mDIConns(nullptr), mDOConns(nullptr), mDIs(nullptr), mDOs(nullptr),
        mAdapters(nullptr),
        mConfiguredFBTypeNameId(CStringDictionary::scmInvalidStringId),
        mGenInterfaceSpec(),
        mFBConnData(nullptr), mFBVarsData(nullptr)
{
}

template<class T>
CGenFunctionBlock<T>::CGenFunctionBlock(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec *paInterfaceSpec,
                                        const CStringDictionary::TStringId paInstanceNameId) :
        T(paContainer, paInterfaceSpec, paInstanceNameId),
        mEOConns(nullptr), mDIConns(nullptr), mDOConns(nullptr), mDIs(nullptr), mDOs(nullptr),
        mAdapters(nullptr),
        mConfiguredFBTypeNameId(CStringDictionary::scmInvalidStringId),
        mGenInterfaceSpec(),
        mFBConnData(nullptr), mFBVarsData(nullptr)
{
}

template<class T>
CGenFunctionBlock<T>::~CGenFunctionBlock(){
  if(nullptr != T::mInterfaceSpec){
    freeFBInterfaceData();  //clean the interface and connections first.
    T::mInterfaceSpec = nullptr; //this stops the base classes from any wrong clean-up
  }
}

template<class T>
bool CGenFunctionBlock<T>::initialize() {
  setupFBInterface(T::mInterfaceSpec);
  return true;
}

template<class T>
bool CGenFunctionBlock<T>::configureFB(const char *paConfigString){
  setConfiguredTypeNameId(CStringDictionary::getInstance().insert(paConfigString));
  if(createInterfaceSpec(paConfigString, mGenInterfaceSpec)){
    setupFBInterface(&mGenInterfaceSpec);
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
      paNamesArayStart[i - 1] = CStringDictionary::getInstance().insert(acBuffer);
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
void CGenFunctionBlock<T>::setupFBInterface(const SFBInterfaceSpec *paInterfaceSpec) {
  freeFBInterfaceData();

  T::mInterfaceSpec = const_cast<SFBInterfaceSpec *>(paInterfaceSpec);

  if (nullptr != paInterfaceSpec) {
    size_t connDataSize = calculateFBConnDataSize(*paInterfaceSpec);
    size_t varsDataSize = calculateFBVarsDataSize(*paInterfaceSpec);
    mFBConnData = connDataSize ? operator new(connDataSize) : nullptr;
    mFBVarsData = varsDataSize ? operator new(varsDataSize) : nullptr;

    auto *connData = reinterpret_cast<TForteByte *>(mFBConnData);
    auto *varsData = reinterpret_cast<TForteByte *>(mFBVarsData);

    TPortId i;
    if (T::mInterfaceSpec->mNumEOs) {
      mEOConns = reinterpret_cast<CEventConnection *>(connData);

      for (i = 0; i < T::mInterfaceSpec->mNumEOs; ++i) {
        //create an event connection for each event output and initialize its source port
        new(connData)CEventConnection(this, i);
        connData += sizeof(CEventConnection);
      }
    } else {
      mEOConns = nullptr;
    }

    const CStringDictionary::TStringId *pnDataIds;
    if (T::mInterfaceSpec->mNumDIs) {
      mDIConns = reinterpret_cast<TDataConnectionPtr *>(connData);
      connData += sizeof(TDataConnectionPtr) * T::mInterfaceSpec->mNumDIs;

      mDIs = reinterpret_cast<CIEC_ANY **>(varsData);
      varsData += T::mInterfaceSpec->mNumDIs * sizeof(CIEC_ANY *);

      pnDataIds = paInterfaceSpec->mDIDataTypeNames;
      for (i = 0; i < T::mInterfaceSpec->mNumDIs; ++i) {
        mDIs[i] = T::createDataPoint(pnDataIds, varsData);
        mDIConns[i] = nullptr;
      }
    } else {
      mDIConns = nullptr;
      mDIs = nullptr;
    }

    if (T::mInterfaceSpec->mNumDOs) {
      //let mDOConns point to the first data output connection
      mDOConns = reinterpret_cast<CDataConnection *>(connData);

      mDOs = reinterpret_cast<CIEC_ANY **>(varsData);
      varsData += T::mInterfaceSpec->mNumDOs * sizeof(CIEC_ANY *);

      pnDataIds = paInterfaceSpec->mDODataTypeNames;
      for (i = 0; i < T::mInterfaceSpec->mNumDOs; ++i) {
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
    if (T::mInterfaceSpec->mNumAdapters) {
      setupAdapters(paInterfaceSpec, varsData);
    }

#ifdef FORTE_SUPPORT_MONITORING
    T::setupEventMonitoringData();
#endif
  }
}

template<class T>
void CGenFunctionBlock<T>::freeFBInterfaceData(){
  if(nullptr != T::mInterfaceSpec){
    if(nullptr != mEOConns) {
      std::destroy_n(mEOConns, T::mInterfaceSpec->mNumEOs);
    }

    if(nullptr != mDOConns) {
      for (TPortId i = 0; i < T::mInterfaceSpec->mNumDOs; ++i) {
        if(CIEC_ANY* value = mDOConns[i].getValue(); nullptr != value) {
          std::destroy_at(value);
        }
      }
      std::destroy_n(mDOConns, T::mInterfaceSpec->mNumDOs);
    }

    if(nullptr != mDIs) {
      for (TPortId i = 0; i < T::mInterfaceSpec->mNumDIs; ++i) {
        if(CIEC_ANY* value = mDIs[i]; nullptr != value) {
          std::destroy_at(value);
        }
      }
    }

    if(nullptr != mDOs) {
      for (TPortId i = 0; i < T::mInterfaceSpec->mNumDOs; ++i) {
        if(CIEC_ANY* value = mDOs[i]; nullptr != value) {
          std::destroy_at(value);
        }
      }
    }

    if(nullptr != mAdapters) {
      for (TPortId i = 0; i < T::mInterfaceSpec->mNumAdapters; ++i) {
        T::destroyAdapter(mAdapters[i]);
      }
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
void CGenFunctionBlock<T>::setupAdapters(const SFBInterfaceSpec *paInterfaceSpec, TForteByte *paFBData){
  if((nullptr != paInterfaceSpec) && (nullptr != paFBData) && (paInterfaceSpec->mNumAdapters)) {
    mAdapters = reinterpret_cast<TAdapterPtr *>(paFBData);
    for(TPortId i = 0; i < paInterfaceSpec->mNumAdapters; ++i) {
      mAdapters[i] = T::createAdapter(paInterfaceSpec->mAdapterInstanceDefinition[i], static_cast<TForteUInt8>(i));
    }
  }
}

