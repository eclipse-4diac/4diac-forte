/*******************************************************************************
 * Copyright (c) 2018 Johannes Kepler University
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _GENFB_H_
#define _GENFB_H_

#include "funcbloc.h"

class SFBInterfaceSpecforGenerics : public SFBInterfaceSpec{
  public:
    SFBInterfaceSpecforGenerics(TForteUInt8 paNumEIs,
        const CStringDictionary::TStringId * const paEINames,
        const TDataIOID * const paEIWith, const TForteInt16 * const paEIWithIndexes,
        TForteUInt8 paNumEOs, const CStringDictionary::TStringId * const paEONames,
        const TDataIOID * const paEOWith, const TForteInt16 * const paEOWithIndexes,
        TForteUInt8 paNumDIs, const CStringDictionary::TStringId * const paDINames,
        const CStringDictionary::TStringId * const paDIDataTypeNames, TForteUInt8 paNumDOs,
        const CStringDictionary::TStringId * const paDONames,
        const CStringDictionary::TStringId * const paDODataTypeNames){
      m_nNumEIs = paNumEIs;
      m_aunEINames = paEINames;
      m_anEIWith = paEIWith;
      m_anEIWithIndexes = paEIWithIndexes;
      m_nNumEOs = paNumEOs;
      m_aunEONames = paEONames;
      m_anEOWith = paEOWith;
      m_anEOWithIndexes = paEOWithIndexes;
      m_nNumDIs = paNumDIs;
      m_aunDINames = paDINames;
      m_aunDIDataTypeNames = paDIDataTypeNames;
      m_nNumDOs = paNumDOs;
      m_aunDONames = paDONames;
      m_aunDODataTypeNames = paDODataTypeNames;
      m_nNumAdapters = 0;
      m_pstAdapterInstanceDefinition = 0;
    }
  private:
    SFBInterfaceSpecforGenerics();
};

template <class T>
class CGenFunctionBlock : public T {
  public:

    /*!\brief The getFBType method is used by the Query command to get the instances correct type name (eg. "CLIENT_3_2")
    * \return pointer to typename string
    */
    CStringDictionary::TStringId getFBTypeId(void) const {
      return mConfiguredFBTypeNameId;
    }

    bool configureFB(const char *paConfigString);

  protected:
    CGenFunctionBlock(CResource *paSrcRes, const CStringDictionary::TStringId paInstanceNameId);
    virtual ~CGenFunctionBlock();


    static void generateGenericInterfacePointNameArray(const char * const paPrefix,
        CStringDictionary::TStringId* paNamesArayStart, unsigned int paNumGenericDataPoints);
    static void generateGenericDataPointArrays(const char * const paPrefix,
        CStringDictionary::TStringId* paDataTypeNamesArrayStart,
        CStringDictionary::TStringId* paNamesArrayStart, unsigned int paNumGenericDataPoints);

  private:
    virtual SFBInterfaceSpecforGenerics *createInterfaceSpec(const char *paConfigString) = 0;

    void setConfiguredTypeNameId( CStringDictionary::TStringId paTypeNameId){
      mConfiguredFBTypeNameId = paTypeNameId;
    }

    CStringDictionary::TStringId mConfiguredFBTypeNameId;
    TForteByte *mFBConnData;
    TForteByte *mFBVarsData;
};

#include "genfb.tpp"

#endif /* _GENFB_H_ */
