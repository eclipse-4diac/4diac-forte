/*******************************************************************************
 * Copyright (c) 2011 ACIN
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *     - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#ifndef _GEN_E_MUX_H_
#define _GEN_E_MUX_H_

#include <genfb.h>

class GEN_E_MUX : public CGenFunctionBlock<CFunctionBlock>{
  DECLARE_GENERIC_FIRMWARE_FB(GEN_E_MUX)

  private:
    static const CStringDictionary::TStringId scm_anDataOutputNames[], scm_aunDODataTypeIds[];

    static const TEventID scm_nEventEOID = 0;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    CStringDictionary::TStringId *m_anEventInputNames;

    virtual void executeEvent(int paEIID);
    virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec);

    CIEC_UINT& K(){
      return *static_cast<CIEC_UINT*>(getDO(0));
    }

    GEN_E_MUX(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    virtual ~GEN_E_MUX();

};
#endif //_GEN_E_MUX_H_
