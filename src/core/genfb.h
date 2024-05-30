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
#ifndef _GENFB_H_
#define _GENFB_H_

#include "funcbloc.h"

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
    CGenFunctionBlock(forte::core::CFBContainer &paContainer, const CStringDictionary::TStringId paInstanceNameId);

    CGenFunctionBlock(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec *paInterfaceSpec,
                      const CStringDictionary::TStringId paInstanceNameId);

    ~CGenFunctionBlock() override;

    bool initialize();

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

    void setupFBInterface(const SFBInterfaceSpec *paInterfaceSpec);

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

    void setupAdapters(const SFBInterfaceSpec *paInterfaceSpec, TForteByte *paFBData);

    CStringDictionary::TStringId mConfiguredFBTypeNameId;
    SFBInterfaceSpec mGenInterfaceSpec;  //!< the interface spec for this specific instance of generic FB
    void *mFBConnData; //!< Connection data buffer
    void *mFBVarsData; //!< Variable data buffer
};

#include "genfb.tpp"

#endif /* _GENFB_H_ */
