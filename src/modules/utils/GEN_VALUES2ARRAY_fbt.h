/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#ifndef _GEN_VALUES2ARRAY_H_
#define _GEN_VALUES2ARRAY_H_

#include <genfb.h>

class GEN_VALUES2ARRAY : public CGenFunctionBlock<CFunctionBlock>{
  DECLARE_GENERIC_FIRMWARE_FB(GEN_VALUES2ARRAY)

  private:
    CStringDictionary::TStringId *mDataInputNames;
    CStringDictionary::TStringId *mDataInputTypeIds;

    static const CStringDictionary::TStringId scmDataOutputNames[];
    CStringDictionary::TStringId *mDataOutputTypeIds;

    CIEC_ARRAY &OUT_Array(){
      return *static_cast<CIEC_ARRAY *>(getDO(0));
    }

    static const TEventID scmEventREQID = 0;
    static const CStringDictionary::TStringId scmEventInputNames[];

    static const TEventID scmEventCNFID = 0;
    static const CStringDictionary::TStringId scmEventOutputNames[];

    //self-defined members
    unsigned int mDInputs;
    CStringDictionary::TStringId m_ValueTypeID;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEI) override;
    void writeOutputData(TEventID paEO) override;

    bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

    GEN_VALUES2ARRAY(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    ~GEN_VALUES2ARRAY() override;

};

#endif //_GEN_VALUES2ARRAY_H_

