/*******************************************************************************
 * Copyright (c) 2012 - 2013 Profactor GmbH, ACIN
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#ifndef _GEN_APPEND_STRING_H_
#define _GEN_APPEND_STRING_H_

#include <genfb.h>

class GEN_APPEND_STRING : public CGenFunctionBlock<CFunctionBlock> {
  DECLARE_GENERIC_FIRMWARE_FB(GEN_APPEND_STRING)

  private:
  //we know for sure that there is one output event and one input event
    static const TEventID scm_nEventCNFID = 0;
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const TEventID scm_nEventREQID = 0;
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    CStringDictionary::TStringId *m_anDataInputNames;
    CStringDictionary::TStringId *m_anDataInputTypeIds;

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];

    static const TForteInt16 scm_anEIWithIndexes[];
    TDataIOID *m_anEIWith;

    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];

  //maximum string buffer size for conversions into string values
  static const TForteInt16 scm_maxStringBufSize;
    //self-defined members
    size_t mDInputs;

     virtual void executeEvent(int paEIID);
     virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec);

    GEN_APPEND_STRING(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    virtual ~GEN_APPEND_STRING();
  public:

};
#endif //_GEN_APPEND_STRING_H_
