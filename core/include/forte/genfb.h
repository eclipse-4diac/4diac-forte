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
#include "forte/basicfb.h"
#include "forte/eventconn.h"
#include "forte/funcbloc.h"

namespace forte {
  template<class T>
  class CGenFunctionBlock : public T {
      static_assert(std::is_base_of_v<CFunctionBlock, T>, "T must be a subclass of CFunctionBlock");

    public:
      /*!\brief The getFBType method is used by the Query command to get the instances correct type name (eg.
       * "CLIENT_3_2")
       * \return pointer to typename string
       */
      StringId getFBTypeId() const override {
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
      CGenFunctionBlock(CFBContainer &paContainer, const StringId paInstanceNameId, Args &&...args) :
          T(paContainer, mGenInterfaceSpec, paInstanceNameId, std::forward<Args>(args)...) {
      }

      template<typename... Args>
      CGenFunctionBlock(CFBContainer &paContainer,
                        const SFBInterfaceSpec &paInterfaceSpec,
                        const StringId paInstanceNameId,
                        Args &&...args) :
          T(paContainer, mGenInterfaceSpec, paInstanceNameId, std::forward<Args>(args)...),
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
        if (paIndex < interfaceSpec.getNumDIs()) {
          T::getDIFromPortId(paIndex)->setValue(paArg);
        } else if (paIndex < interfaceSpec.getNumDIs() + interfaceSpec.getNumDIOs()) {
          T::getDIOFromPortId(paIndex - interfaceSpec.getNumDIs())->setValue(paArg);
        } else if (paIndex < interfaceSpec.getNumDIs() + interfaceSpec.getNumDIOs() + interfaceSpec.getNumDOs()) {
          T::getDOFromPortId(paIndex - interfaceSpec.getNumDIs() - interfaceSpec.getNumDIOs())->setValue(paArg);
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
        if (paIndex < interfaceSpec.getNumDIs()) {
          T::getDIFromPortId(paIndex)->setValue(paArg);
        } else if (paIndex < interfaceSpec.getNumDIs() + interfaceSpec.getNumDIOs()) {
          T::getDIOFromPortId(paIndex - interfaceSpec.getNumDIs())->setValue(paArg);
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
        if (paIndex < interfaceSpec.getNumDIs()) {
          // do nothing
        } else if (paIndex < interfaceSpec.getNumDIs() + interfaceSpec.getNumDIOs()) {
          if constexpr (std::is_const_v<std::remove_reference_t<Arg>>) {
            DEVLOG_ERROR("[CGenFunctionBlock] Trying to pass const argument to in/out variable\n");
          } else {
            paArg.setValue(T::getDIOFromPortId(paIndex - interfaceSpec.getNumDIs())->unwrap());
          }
        } else if (paIndex < interfaceSpec.getNumDIs() + interfaceSpec.getNumDIOs() + interfaceSpec.getNumDOs()) {
          if constexpr (std::is_const_v<std::remove_reference_t<Arg>>) {
            DEVLOG_ERROR("[CGenFunctionBlock] Trying to pass const argument to output variable\n");
          } else {
            paArg.setValue(
                T::getDOFromPortId(paIndex - interfaceSpec.getNumDIs() - interfaceSpec.getNumDIOs())->unwrap());
          }
        }
      }

      static void generateGenericInterfacePointNameArray(const char *const paPrefix,
                                                         std::vector<StringId> &paNamesArayStart,
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
          for (size_t i = getGenEOOffset(); i < getGenInterfaceSpec().getNumEOs(); ++i) {
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
          for (size_t i = getGenDOOffset(); i < getGenInterfaceSpec().getNumDOs(); ++i) {
            mGenDOConns.emplace_back(*this, i, *getDO(i));
          }
        }
      }

      virtual void createAdapters() {
        // per default we are not creating any adapters
      }

      size_t getGenEONums() {
        return getGenInterfaceSpec().getNumEOs() - getGenEOOffset();
      }

      virtual size_t getGenEOOffset() {
        return 0;
      }

      size_t getGenDINums() {
        return getGenInterfaceSpec().getNumDIs() - getGenDIOffset();
      }

      virtual size_t getGenDIOffset() {
        return 0;
      }

      size_t getGenDONums() {
        return getGenInterfaceSpec().getNumDOs() - getGenDOOffset();
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

      SFBInterfaceSpec &getGenInterfaceSpec() {
        return mGenInterfaceSpec;
      }

      std::vector<CEventConnection> mGenEOConns;
      std::unique_ptr<CDataConnection *[]> mGenDIConns;
      std::vector<CGenDataConnection> mGenDOConns;

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
       * @retval true if the string could be parsed, false if the string is invalid and no interface spec could be
       * derived from it
       */
      virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) = 0;

      void setConfiguredTypeNameId(StringId paTypeNameId) {
        mConfiguredFBTypeNameId = paTypeNameId;
      }

      StringId mConfiguredFBTypeNameId;
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
    setConfiguredTypeNameId(StringId::insert(paConfigString));
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

    for (TPortId i = 0; i < interfaceSpec.getNumDIs(); ++i) {
      getDI(i)->reset();
    }

    for (TPortId i = 0; i < interfaceSpec.getNumDOs(); ++i) {
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
                                                                    std::vector<StringId> &paNamesArayStart,
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
      paNamesArayStart.reserve(paNamesArayStart.size() + paNumGenericDataPoints);

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
        paNamesArayStart.emplace_back(StringId::insert(acBuffer));
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

    createAdapters();

    T::setupInputConnectionTrackingData();

    T::setupEventMonitoringData();
  }

  template<class T>
  void CGenFunctionBlock<T>::freeFBInterfaceData() {
    T::freeEventMonitoringData();
  }
} // namespace forte
