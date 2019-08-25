/*******************************************************************************
 * Copyright (c) 2009 - 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _FB_SR_H_
#define _FB_SR_H_

#include <funcbloc.h>

class FB_SR: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FB_SR)

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

  FORTE_FB_DATA_ARRAY(1,2,1, 0);

  void executeEvent(int pa_nEIID);

  CIEC_BOOL& S1() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  }
  CIEC_BOOL& R() {
    return *static_cast<CIEC_BOOL*>(getDI(1));
  }

  CIEC_BOOL& Q1() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  }

public:
  FUNCTION_BLOCK_CTOR(FB_SR){};
  virtual ~FB_SR(){};

};

#endif //close the ifdef sequence from the beginning of the file

