/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/

#ifndef _GEN_ADD_H_
#define _GEN_ADD_H_

#include <genfb.h>

class GEN_ADD: public CGenFunctionBlock<CFunctionBlock> {
DECLARE_GENERIC_FIRMWARE_FB(GEN_ADD)

private:
  CStringDictionary::TStringId *m_anDataInputNames;
  CStringDictionary::TStringId *m_anDataInputTypeIds;

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ANY_MAGNITUDE &st_OUT() {
    return *static_cast<CIEC_ANY_MAGNITUDE*>(getDO(0));
  }

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  TDataIOID *m_anEIWith;
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  //self-defined members
  unsigned int m_nDInputs;

  virtual void executeEvent(int paEIID);
  virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec);

  GEN_ADD(const CStringDictionary::TStringId pa_nInstanceNameId,
      CResource *pa_poSrcRes);
  virtual ~GEN_ADD();

public:

  template<typename T> void calculateValue() {
    T oIn;
    T oOut;

    for (unsigned int nInputIndex = 0; nInputIndex < m_nDInputs; nInputIndex++) {

      oIn.saveAssign(*static_cast<T*>(getDI(nInputIndex)));

      if (0 == nInputIndex) {
        st_OUT().saveAssign(oIn);
      } else {
        oOut.saveAssign(st_OUT());
        st_OUT().saveAssign(ADD(oOut, oIn));
      }
    }
  }
};

#endif // _GEN_ADD_H_

