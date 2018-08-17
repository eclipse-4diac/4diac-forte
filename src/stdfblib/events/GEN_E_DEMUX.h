/*******************************************************************************
 * Copyright (c) 2011 ACIN
 *                      2018 Johannes Kepler University
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl
 *     - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#ifndef _GEN_E_DEMUX_H_
#define _GEN_E_DEMUX_H_

#include <genfb.h>

class GEN_E_DEMUX : public CGenFunctionBlock<CFunctionBlock>{
  DECLARE_GENERIC_FIRMWARE_FB(GEN_E_DEMUX)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[], scm_aunDIDataTypeIds[];

    static const TEventID scm_nEventEIID = 0;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    CStringDictionary::TStringId *m_anEventOutputNames;

    virtual void executeEvent(int pa_nEIID);
    virtual SFBInterfaceSpecforGenerics *createInterfaceSpec(const char *paConfigString);

    CIEC_UINT& K(){
      return *static_cast<CIEC_UINT*>(getDI(0));
    }

    GEN_E_DEMUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
    virtual ~GEN_E_DEMUX();
  public:

};
#endif //close the ifdef sequence from the beginning of the file
