/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _X20DO9322_H_
#define _X20DO9322_H_

#include <funcbloc.h>

#include "EplCNCallback.h"
#include "EplWrapper.h"

// cppcheck-suppress noConstructor
class FORTE_X20DO9322 : public CFunctionBlock, public IEplCNCallback{
  DECLARE_FIRMWARE_FB(FORTE_X20DO9322)

  private:
    static const CStringDictionary::TStringId scm_anDataInputNames[];
    static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }
    ;

    CIEC_USINT &CNID(){
      return *static_cast<CIEC_USINT*>(getDI(1));
    }
    ;

    CIEC_UINT &MODID(){
      return *static_cast<CIEC_UINT*>(getDI(2));
    }
    ;

    CIEC_BOOL &DO01(){
      return *static_cast<CIEC_BOOL*>(getDI(3));
    }
    ;

    CIEC_BOOL &DO02(){
      return *static_cast<CIEC_BOOL*>(getDI(4));
    }
    ;

    CIEC_BOOL &DO03(){
      return *static_cast<CIEC_BOOL*>(getDI(5));
    }
    ;

    CIEC_BOOL &DO04(){
      return *static_cast<CIEC_BOOL*>(getDI(6));
    }
    ;

    CIEC_BOOL &DO05(){
      return *static_cast<CIEC_BOOL*>(getDI(7));
    }
    ;

    CIEC_BOOL &DO06(){
      return *static_cast<CIEC_BOOL*>(getDI(8));
    }
    ;

    CIEC_BOOL &DO07(){
      return *static_cast<CIEC_BOOL*>(getDI(9));
    }
    ;

    CIEC_BOOL &DO08(){
      return *static_cast<CIEC_BOOL*>(getDI(10));
    }
    ;

    CIEC_BOOL &DO09(){
      return *static_cast<CIEC_BOOL*>(getDI(11));
    }
    ;

    CIEC_BOOL &DO10(){
      return *static_cast<CIEC_BOOL*>(getDI(12));
    }
    ;

    CIEC_BOOL &DO11(){
      return *static_cast<CIEC_BOOL*>(getDI(13));
    }
    ;

    CIEC_BOOL &DO12(){
      return *static_cast<CIEC_BOOL*>(getDI(14));
    }
    ;

    static const CStringDictionary::TStringId scm_anDataOutputNames[];
    static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }
    ;

    CIEC_USINT &CNIDO(){
      return *static_cast<CIEC_USINT*>(getDO(1));
    }
    ;

    CIEC_STRING &STATUS(){
      return *static_cast<CIEC_STRING*>(getDO(2));
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

    FORTE_FB_DATA_ARRAY(2, 15, 3, 0)
    ;

    void executeEvent(int pa_nEIID);

    // Variables for mapping between inputs and POWERLINK stack
    SEplMapping m_oEplMapping;CSyncObject m_oSync;
    bool m_bInitOk;
    // ...

  public:
    FUNCTION_BLOCK_CTOR(FORTE_X20DO9322){
  };

  virtual ~FORTE_X20DO9322(){};

  void cnSynchCallback();

};

#endif //close the ifdef sequence from the beginning of the file
