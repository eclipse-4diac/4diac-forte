/*******************************************************************************
 * Copyright (c) 2020 Johannes Kepler University
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
#ifndef _GEN_STRUCT_DEMUX_H_
#define _GEN_STRUCT_DEMUX_H_

#include <genfb.h>
#include <stdio.h>

class GEN_STRUCT_DEMUX : public CGenFunctionBlock<CFunctionBlock> {
    DECLARE_GENERIC_FIRMWARE_FB(GEN_STRUCT_DEMUX)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];

    static const TEventID scm_nEventREQID = 0;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventCNFID = 0;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    void executeEvent(int paEIID);
    virtual bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec);

    CIEC_STRUCT& st_IN() {
       return *static_cast<CIEC_STRUCT*>(getDI(0));
    }

    //copy constructor, declared but not defined to avoid copying it
    GEN_STRUCT_DEMUX(const GEN_STRUCT_DEMUX &paOther);

  public:
    GEN_STRUCT_DEMUX(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    virtual ~GEN_STRUCT_DEMUX();

};

#endif //_GEN_STRUCT_DEMUX_H_
