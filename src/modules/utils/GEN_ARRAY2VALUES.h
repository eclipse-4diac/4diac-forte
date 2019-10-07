/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GbmH
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
 *******************************************************************************/
#ifndef _GEN_ARRAY2VALUES_H_
#define _GEN_ARRAY2VALUES_H_

#include <genfb.h>

class GEN_ARRAY2VALUES : public CGenFunctionBlock<CFunctionBlock> {
  DECLARE_GENERIC_FIRMWARE_FB(GEN_ARRAY2VALUES)

  private:
    CStringDictionary::TStringId *m_anDataOutputNames;
    CStringDictionary::TStringId *m_anDataOutputTypeIds;

    static const CStringDictionary::TStringId scm_anDataInputNames[];
    CStringDictionary::TStringId *m_anDataInputTypeIds;

    CIEC_ARRAY &IN_Array(){
      return *static_cast<CIEC_ARRAY *>(getDI(0));
    }
    ;

    static const TEventID scm_nEventREQID = 0;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventCNFID = 0;
    static const TForteInt16 scm_anEOWithIndexes[];
    TDataIOID *m_anEOWith;
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    //self-defined members
    size_t mDOutputs;
    CStringDictionary::TStringId mValueTypeID;

    virtual void executeEvent(int paEIID);
    virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec);

    GEN_ARRAY2VALUES(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    virtual ~GEN_ARRAY2VALUES();

  public:
};

#endif //_GEN_ARRAY2VALUES_H_

