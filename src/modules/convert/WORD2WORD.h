/*******************************************************************************
 * Copyright (c) 2006 - 2011 Profactor GmbH, ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _WORD2WORD_H_
#define _WORD2WORD_H_

#ifndef FMU

#include <funcbloc.h>

class WORD2WORD: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(WORD2WORD)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[], scm_aunDIDataTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[], scm_aunDODataTypeIds[];
  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1,1,1, 0);

  virtual void executeEvent(int pa_nEIID);

  CIEC_WORD& IN() {
    return *static_cast<CIEC_WORD*>(getDI(0));
  }

  CIEC_WORD& OUT() {
    return *static_cast<CIEC_WORD*>(getDO(0));
  }

public:
  FUNCTION_BLOCK_CTOR(WORD2WORD){};
  virtual ~WORD2WORD();

};

#else

#include <basicfb.h>
#include <forte_word.h>

class WORD2WORD: public CBasicFB{
  DECLARE_FIRMWARE_FB(WORD2WORD)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_WORD &IN() {
    return *static_cast<CIEC_WORD*>(getDI(0));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_WORD &OUT() {
    return *static_cast<CIEC_WORD*>(getDO(0));
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

   FORTE_BASIC_FB_DATA_ARRAY(1, 1, 1, 0, 0);
  void alg_REQ(void);
  static const TForteInt16 scm_nStateSTART = 0;
  static const TForteInt16 scm_nStateREQ = 1;

  void enterStateSTART(void);
  void enterStateREQ(void);

  virtual void executeEvent(int pa_nEIID);

public:
  WORD2WORD(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId,
              0, m_anFBConnData, m_anFBVarsData){
  };

  virtual ~WORD2WORD(){};

};

#endif

#endif //close the ifdef sequence from the beginning of the file

