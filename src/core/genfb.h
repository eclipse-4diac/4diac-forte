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

#include <cstddef>
#include <memory>
#include "basicfb.h"
#include "eventconn.h"
#include "funcbloc.h"

USE_STRING_ID(ANY);

template<class T>
class CGenFunctionBlock : public T {
    static_assert(std::is_base_of_v<CFunctionBlock, T>, "T must be a subclass of CFunctionBlock");

  public:
    /*!\brief The getFBType method is used by the Query command to get the instances correct type name (eg.
     * "CLIENT_3_2")
     * \return pointer to typename string
     */
    CStringDictionary::TStringId getFBTypeId() const override {
      return mConfiguredFBTypeNameId;
    }

    bool configureFB(const char *paConfigString) override;

    CIEC_ANY *getDI(TPortId) override {
      return nullptr;
    }

    CIEC_ANY *getDO(TPortId) override {
      return nullptr;
    }

  protected:
    template<typename... Args>
    CGenFunctionBlock(forte::core::CFBContainer &paContainer,
                      const CStringDictionary::TStringId paInstanceNameId,
                      Args &&...args) :
        T(paContainer, mGenInterfaceSpec, paInstanceNameId, std::forward<Args>(args)...),
        mAdapters(nullptr),
        mConfiguredFBTypeNameId(CStringDictionary::scmInvalidStringId),
        mGenInterfaceSpec() {
    }

    template<typename... Args>
    CGenFunctionBlock(forte::core::CFBContainer &paContainer,
                      const SFBInterfaceSpec &paInterfaceSpec,
                      const CStringDictionary::TStringId paInstanceNameId,
                      Args &&...args) :
        T(paContainer, mGenInterfaceSpec, paInstanceNameId, std::forward<Args>(args)...),
        mAdapters(nullptr),
        mConfiguredFBTypeNameId(CStringDictionary::scmInvalidStringId),
        mGenInterfaceSpec(paInterfaceSpec) {
    }

    ~CGenFunctionBlock() override;

    bool initialize() override;

    void setInitialValues() override;

    template<typename... Args>
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

    template<typename... Args>
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

    template<typename... Args>
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

    static void generateGenericInterfacePointNameArray(const char *const paPrefix,
                                                       CStringDictionary::TStringId *paNamesArayStart,
                                                       size_t paNumGenericDataPoints);

    void setupFBInterface();

    virtual void createGenInputData() {
      // per default we are not creating any pins
    }

    virtual void createGenOutputData() {
      // per default we are not creating any pins
    }

    void createGenEOCons() {
      if (getGenEONums()) {
        mGenEOConns.reserve(getGenEONums());
        for (size_t i = getGenEOOffset(); i < getGenInterfaceSpec().mNumEOs; ++i) {
          mGenEOConns.emplace_back(*this, i);
        }
      }
    }

    void createGenDICons() {
      if (getGenDINums()) {
        mGenDIConns = std::make_unique<CDataConnection *[]>(getGenDINums());
      }
    }

    void createGenDOCons() {
      if (getGenDONums()) {
        mGenDOConns.reserve(getGenDONums());
        for (size_t i = getGenDOOffset(); i < getGenInterfaceSpec().mNumDOs; ++i) {
          mGenDOConns.emplace_back(*this, i, *getDO(i));
        }
      }
    }

    size_t getGenEONums() {
      return getGenInterfaceSpec().mNumEOs - getGenEOOffset();
    }

    virtual size_t getGenEOOffset() {
      return 0;
    }

    size_t getGenDINums() {
      return getGenInterfaceSpec().mNumDIs - getGenDIOffset();
    }

    virtual size_t getGenDIOffset() {
      return 0;
    }

    size_t getGenDONums() {
      return getGenInterfaceSpec().mNumDOs - getGenDOOffset();
    }

    virtual size_t getGenDOOffset() {
      return 0;
    }

    CEventConnection *getEOConUnchecked(TPortId paEONum) override {
      return &mGenEOConns[paEONum - getGenEOOffset()];
    }

    CDataConnection *getDOConUnchecked(TPortId paDONum) override {
      return &mGenDOConns[paDONum - getGenDOOffset()];
    }

    CDataConnection **getDIConUnchecked(TPortId paDINum) override {
      return &mGenDIConns[paDINum - getGenDIOffset()];
    }

    CAdapter *getAdapterUnchecked(TPortId paAdapterNum) override {
      return mAdapters[paAdapterNum];
    }

    SFBInterfaceSpec &getGenInterfaceSpec() {
      return mGenInterfaceSpec;
    }

    std::vector<CEventConnection> mGenEOConns;
    std::unique_ptr<CDataConnection *[]> mGenDIConns;
    std::vector<CGenDataConnection> mGenDOConns;

    std::unique_ptr<CAdapter *[]>
        mAdapters; //!< A list of pointers to the adapters. This allows to implement a general getAdapter().

  private:
    void freeFBInterfaceData();

    /*! \brief parse the config string and generate the according interface specification
     *
     * This function is to be implemented by a generic fb and should parse the given interface
     * string and generate an according interface specification. The resulting interface specification
     * has to be set in the paInterfaceSpec parameter.
     *
     * @paConfigString the string sent from the id for configuring this fb (e.g., PUBLISH_2)
     * @paInterfaceSpec the interface data structure where the resulting interface config must be stored
     * @retval true if the string could be parsed, false if the string is invalid and no interface spec could be derived
     * from it
     */
    virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) = 0;

    void setConfiguredTypeNameId(CStringDictionary::TStringId paTypeNameId) {
      mConfiguredFBTypeNameId = paTypeNameId;
    }

    void setupAdapters();

    CStringDictionary::TStringId mConfiguredFBTypeNameId;
    SFBInterfaceSpec mGenInterfaceSpec; //!< the interface spec for this specific instance of generic FB
};

template<class T>
CGenFunctionBlock<T>::~CGenFunctionBlock() {
  freeFBInterfaceData(); // clean the interface and connections first.
  mGenInterfaceSpec = {}; // this stops the base classes from any wrong clean-up
}

template<class T>
bool CGenFunctionBlock<T>::initialize() {
  return T::initialize();
}

template<class T>
bool CGenFunctionBlock<T>::configureFB(const char *paConfigString) {
  setConfiguredTypeNameId(CStringDictionary::insert(paConfigString));
  if (createInterfaceSpec(paConfigString, mGenInterfaceSpec)) {
    setupFBInterface();
    return true;
  }
  return false;
}

template<class T>
void CGenFunctionBlock<T>::setInitialValues() {
  T::setInitialValues();
  const SFBInterfaceSpec &interfaceSpec = T::getFBInterfaceSpec();

  for (TPortId i = 0; i < interfaceSpec.mNumDIs; ++i) {
    getDI(i)->reset();
  }

  for (TPortId i = 0; i < interfaceSpec.mNumDOs; ++i) {
    getDO(i)->reset();
  }

  if constexpr (std::is_base_of_v<CBasicFB, T>) {
    if (T::cmVarInternals) {
      for (TPortId i = 0; i < T::cmVarInternals->mNumIntVars; ++i) {
        T::getVarInternal(i)->reset();
      }
    }
  }
}

template<class T>
void CGenFunctionBlock<T>::generateGenericInterfacePointNameArray(const char *const paPrefix,
                                                                  CStringDictionary::TStringId *paNamesArayStart,
                                                                  size_t paNumGenericDataPoints) {
  size_t len = strlen(paPrefix);

  unsigned int noOfDigits = 0;
  {
    size_t tempNum = paNumGenericDataPoints;
    while (tempNum) {
      tempNum /= 10;
      noOfDigits++;
    }
  }

  if (cgIdentifierLength >= (len + noOfDigits)) {
    TIdentifier acBuffer;
    memcpy(acBuffer, paPrefix, len);
    for (size_t i = 0; i <= noOfDigits; i++) {
      acBuffer[len + i] = '\0';
    }

    for (size_t i = 1; i <= paNumGenericDataPoints; i++) {
      if (i < 10) { // 1 digit
        acBuffer[len] = static_cast<char>(0x30 + i);
      } else if (i < 100) { // 2 digits
        if (0 == i % 10) {
          acBuffer[len] = static_cast<char>(0x30 + (i % 100 / 10));
        }
        acBuffer[len + 1] = static_cast<char>(0x30 + i % 10);
      } else { // 3 digits
        if (0 == i % 100) {
          acBuffer[len] = static_cast<char>(0x30 + (i / 100));
        }
        if (0 == i % 10) {
          acBuffer[len + 1] = static_cast<char>(0x30 + (i % 100 / 10));
        }
        acBuffer[len + 2] = static_cast<char>(0x30 + i % 10);
      }
      paNamesArayStart[i - 1] = CStringDictionary::insert(acBuffer);
    }
  } else {
    DEVLOG_ERROR("CFunctionBlock::generateGenericInterfacePointNameArray won't be able to create all the generics "
                 "since %s is too long to hold until %d",
                 paPrefix, paNumGenericDataPoints);
  }
}

template<class T>
void CGenFunctionBlock<T>::setupFBInterface() {

  createGenEOCons();

  createGenInputData();

  createGenDICons();

  createGenOutputData();

  createGenDOCons();

  setupAdapters();

  T::setupInputConnectionTrackingData();

#ifdef FORTE_SUPPORT_MONITORING
  T::setupEventMonitoringData();
#endif
}

template<class T>
void CGenFunctionBlock<T>::freeFBInterfaceData() {
  if (nullptr != mAdapters) {
    for (TPortId i = 0; i < T::getFBInterfaceSpec().mNumAdapters; ++i) {
      T::destroyAdapter(mAdapters[i]);
    }
  }

#ifdef FORTE_SUPPORT_MONITORING
  T::freeEventMonitoringData();
#endif // FORTE_SUPPORT_MONITORING
}

template<class T>
void CGenFunctionBlock<T>::setupAdapters() {
  size_t numAdpaters = T::getFBInterfaceSpec().mNumAdapters;
  if (numAdpaters) {
    mAdapters = std::unique_ptr<CAdapter *[]>(new CAdapter *[numAdpaters]);
    for (TPortId i = 0; i < numAdpaters; ++i) {
      mAdapters[i] =
          T::createAdapter(T::getFBInterfaceSpec().mAdapterInstanceDefinition[i], static_cast<TForteUInt8>(i));
    }
  }
}
