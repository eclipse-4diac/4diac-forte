/*******************************************************************************
 * Copyright (c) 2014 - 2015 Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _GEN_ARRAY2ARRAY_H_
#define _GEN_ARRAY2ARRAY_H_

#include <funcbloc.h>
#include <forte_array.h>

class GEN_ARRAY2ARRAY: public CFunctionBlock{
	DECLARE_GENERIC_FIRMWARE_FB(GEN_ARRAY2ARRAY)

private:
	static const CStringDictionary::TStringId scm_anDataInputNames[];
	CStringDictionary::TStringId *m_anDataInputTypeIds;

	CIEC_ARRAY &IN_Array() {
		return *static_cast<CIEC_ARRAY*>(getDI(0));
	};

	static const CStringDictionary::TStringId scm_anDataOutputNames[];
	CStringDictionary::TStringId *m_anDataOutputTypeIds;

	CIEC_ARRAY &OUT_Array() {
		return *static_cast<CIEC_ARRAY*>(getDO(0));
	};

	static const TEventID scm_nEventREQID = 0;
	static const TForteInt16 scm_anEIWithIndexes[];
	static const TDataIOID scm_anEIWith[];
	static const CStringDictionary::TStringId scm_anEventInputNames[];

	static const TEventID scm_nEventCNFID = 0;
	static const TForteInt16 scm_anEOWithIndexes[];
	static const TDataIOID scm_anEOWith[];
	static const CStringDictionary::TStringId scm_anEventOutputNames[];

	static const SFBInterfaceSpec scm_stFBInterfaceSpec;

	//self-defined members
	CStringDictionary::TStringId m_ValueTypeID;
	unsigned int m_nArrayLength;
	CStringDictionary::TStringId m_nConfiguredFBTypeNameId;
	virtual void executeEvent(int pa_nEIID);

	GEN_ARRAY2ARRAY(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
	virtual ~GEN_ARRAY2ARRAY();;

public:
	CStringDictionary::TStringId getFBTypeId(void) const{
		return m_nConfiguredFBTypeNameId;
	}

	bool configureFB(const char *pa_acConfigString);
};

#endif //close the ifdef sequence from the beginning of the file

