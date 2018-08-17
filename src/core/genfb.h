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
    SFBInterfaceSpecforGenerics(TForteUInt8 pa_nNumEIs,
        const CStringDictionary::TStringId * const pa_aunEINames,
        const TDataIOID * const pa_anEIWith, const TForteInt16 * const pa_anEIWithIndexes,
        TForteUInt8 pa_nNumEOs, const CStringDictionary::TStringId * const pa_aunEONames,
        const TDataIOID * const pa_anEOWith, const TForteInt16 * const pa_anEOWithIndexes,
        TForteUInt8 pa_nNumDIs, const CStringDictionary::TStringId * const pa_aunDINames,
        const CStringDictionary::TStringId * const pa_aunDIDataTypeNames, TForteUInt8 pa_nNumDOs,
        const CStringDictionary::TStringId * const pa_aunDONames,
        const CStringDictionary::TStringId * const pa_aunDODataTypeNames){
      m_nNumEIs = pa_nNumEIs;
      m_aunEINames = pa_aunEINames;
      m_anEIWith = pa_anEIWith;
      m_anEIWithIndexes = pa_anEIWithIndexes;
      m_nNumEOs = pa_nNumEOs;
      m_aunEONames = pa_aunEONames;
      m_anEOWith = pa_anEOWith;
      m_anEOWithIndexes = pa_anEOWithIndexes;
      m_nNumDIs = pa_nNumDIs;
      m_aunDINames = pa_aunDINames;
      m_aunDIDataTypeNames = pa_aunDIDataTypeNames;
      m_nNumDOs = pa_nNumDOs;
      m_aunDONames = pa_aunDONames;
      m_aunDODataTypeNames = pa_aunDODataTypeNames;
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

#include "genfb.cpp"

#endif /* _GENFB_H_ */
