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
CGenFunctionBlock<T>::CGenFunctionBlock(CResource *paSrcRes, const CStringDictionary::TStringId paInstanceNameId) :
    T(paSrcRes, 0, paInstanceNameId, 0, 0),
    mConfiguredFBTypeNameId(CStringDictionary::scm_nInvalidStringId), mGenInterfaceSpec(), mFBConnData(0), mFBVarsData(0) {

    FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CFunctionBlock, T>::value), TFunctionBlock);
}

template<class T>
CGenFunctionBlock<T>::~CGenFunctionBlock(){
  if(0 != T::m_pstInterfaceSpec){
    T::freeAllData();  //clean the interface and connections first.
    delete[] mFBConnData;
    delete[] mFBVarsData;
    T::m_pstInterfaceSpec = 0; //this stops the base classes from any wrong clean-up
  }
}

template<class T>
bool CGenFunctionBlock<T>::configureFB(const char *paConfigString){
  setConfiguredTypeNameId(CStringDictionary::getInstance().insert(paConfigString));
  if(createInterfaceSpec(paConfigString, mGenInterfaceSpec)){
    mFBConnData = new TForteByte[T::genFBConnDataSize(mGenInterfaceSpec.m_nNumEOs, mGenInterfaceSpec.m_nNumDIs, mGenInterfaceSpec.m_nNumDOs)];
    mFBVarsData = new TForteByte[T::genFBVarsDataSize(mGenInterfaceSpec.m_nNumDIs, mGenInterfaceSpec.m_nNumDOs)];
    T::setupFBInterface(&mGenInterfaceSpec, mFBConnData, mFBVarsData);
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

  if(cg_nIdentifierLength >= (len + noOfDigits)){
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

