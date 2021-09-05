/*******************************************************************************
 * Copyright (c)2018 Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _GENBITBASE_H_
#define _GENBITBASE_H_

#include "../genfb.h"

class CGenBitBase : public CGenFunctionBlock<CFunctionBlock> {

  protected:
    CGenBitBase(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    virtual ~CGenBitBase();

    CIEC_ANY_BIT &st_OUT(){
      return *static_cast<CIEC_ANY_BIT*>(getDO(0));
    }

    static const TEventID scm_nEventREQID = 0;
    static const TEventID scm_nEventCNFID = 0;

  private:
    CStringDictionary::TStringId *mDataInputNames;
    CStringDictionary::TStringId *mDataInputTypeIds;

    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];

    static const TForteInt16 scmEIWithIndexes[];
    TDataIOID *mEIWith;
    static const CStringDictionary::TStringId scmEventInputNames[];

    static const TForteInt16 scmEOWithIndexes[];
    static const TDataIOID scmEOWith[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec);
};

#endif /* _GENBITBASE_H_ */
