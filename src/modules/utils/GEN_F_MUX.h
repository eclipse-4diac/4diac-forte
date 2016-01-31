/*******************************************************************************
 * Copyright (c) 2012 - 2015 Profactor GmbH, ACIN, fortiss Gmbh
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch, Monika Wenger, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _GEN_F_MUX_H_
#define _GEN_F_MUX_H_

#include <funcbloc.h>
#include <forte_any.h>

class GEN_F_MUX : public CFunctionBlock{
  DECLARE_GENERIC_FIRMWARE_FB(GEN_F_MUX)

  private:
	//we know for sure that there is one output event
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const TEventID scm_nEventEOID = 0;

    CStringDictionary::TStringId *m_anEventInputNames;
	CStringDictionary::TStringId *m_anDataOutputNames;
	CStringDictionary::TStringId *m_anDataInputNames;
	CStringDictionary::TStringId *m_anDataOutputTypeIds;
	CStringDictionary::TStringId *m_anDataInputTypeIds;

	TForteInt16 *m_anEIWithIndexes;
	TDataIOID *m_anEIWith;

	TForteInt16 *m_anEOWithIndexes;
	TDataIOID *m_anEOWith;

	//self-defined members
	unsigned int m_nEInputs;
	unsigned int m_nEOutputs;
	unsigned int m_nDInputs;
	unsigned int m_nDOutputs;

    CStringDictionary::TStringId m_nConfiguredFBTypeNameId;

    virtual void executeEvent(int pa_nEIID);

    GEN_F_MUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
    virtual ~GEN_F_MUX();
  public:

    CStringDictionary::TStringId getFBTypeId(void) const{
      return m_nConfiguredFBTypeNameId;
    }

    bool configureFB(const char *pa_acConfigString);
};
#endif //close the ifdef sequence from the beginning of the file
