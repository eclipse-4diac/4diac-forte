/*******************************************************************************
 * Copyright (c) 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _SET_VALUE_H_
#define _SET_VALUE_H_

#include <mtypes.h>                     /* M1 include files    */
#include <funcbloc.h>
#include <forte_int.h>
#include <forte_sint.h>
#include <forte_array.h>

// cppcheck-suppress noConstructor
class FORTE_SET_VALUE : public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_SET_VALUE)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_INT &MOD_NB(){
      return *static_cast<CIEC_INT*>(getDI(0));
    }
    ;

    CIEC_INT &CHAN_NB(){
      return *static_cast<CIEC_INT*>(getDI(1));
    }
    ;

    CIEC_SINT &VALUE(){
      return *static_cast<CIEC_SINT*>(getDI(2));
    }
    ;

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_INT &RET_CODE(){
      return *static_cast<CIEC_INT*>(getDO(0));
    }
    ;

    static const TEventID scm_nEventINITID = 0;
    static const TEventID scm_nEventREQID = 1;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const TDataIOID scm_anEIWith[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventINITOID = 0;
    static const TEventID scm_nEventCNFID = 1;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    static const SFBInterfaceSpec scm_stFBInterfaceSpec;

    FORTE_FB_DATA_ARRAY(2, 3, 1, 0)
    ;

    void executeEvent(int pa_nEIID);

    void *driverId;

    bool init();
    SINT32 write();

  public:
    FUNCTION_BLOCK_CTOR(FORTE_SET_VALUE){
  };

  virtual ~FORTE_SET_VALUE(){};

};

#endif //close the ifdef sequence from the beginning of the file
