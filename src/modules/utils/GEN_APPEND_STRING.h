/*******************************************************************************
 * Copyright (c) 2012 - 2013 Profactor GmbH, ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch, Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _GEN_APPEND_STRING_H_
#define _GEN_APPEND_STRING_H_

#include <funcbloc.h>

class GEN_APPEND_STRING : public CFunctionBlock{
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
    int m_nDInputs;

    CStringDictionary::TStringId m_nConfiguredFBTypeNameId;

    virtual void executeEvent(int pa_nEIID);

    GEN_APPEND_STRING(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
    virtual ~GEN_APPEND_STRING();
  public:

    CStringDictionary::TStringId getFBTypeId(void) const{
      return m_nConfiguredFBTypeNameId;
    }

    bool configureFB(const char *pa_acConfigString);
};
#endif //close the ifdef sequence from the beginning of the file
