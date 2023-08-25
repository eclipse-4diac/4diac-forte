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
#ifndef _X20AI4622_H_
#define _X20AI4622_H_

#include <funcbloc.h>

#include "EplCNCallback.h"
#include "EplWrapper.h"

// cppcheck-suppress noConstructor
class FORTE_X20AI4622 : public CFunctionBlock, public IEplCNCallback{
  DECLARE_FIRMWARE_FB(FORTE_X20AI4622)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
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

    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
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

    CIEC_INT &AI01(){
      return *static_cast<CIEC_INT*>(getDO(3));
    }
    ;

    CIEC_INT &AI02(){
      return *static_cast<CIEC_INT*>(getDO(4));
    }
    ;

    CIEC_INT &AI03(){
      return *static_cast<CIEC_INT*>(getDO(5));
    }
    ;

    CIEC_INT &AI04(){
      return *static_cast<CIEC_INT*>(getDO(6));
    }
    ;

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;
    static const TForteInt16 scmEIWithIndexes[];
    static const TDataIOID scmEIWith[];
    static const CStringDictionary::TStringId scmEventInputNames[];

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;
    static const TForteInt16 scmEOWithIndexes[];
    static const TDataIOID scmEOWith[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;


    void executeEvent(TEventID paEIID) override;

    // Variables for mapping between inputs and POWERLINK stack
    SEplMapping mEplMapping;CSyncObject mSync;
    bool mInitOk;
    // ...

  public:
    FUNCTION_BLOCK_CTOR(FORTE_X20AI4622){
  };

  ~FORTE_X20AI4622() override = default;

  void cnSynchCallback() override;

};

#endif //close the ifdef sequence from the beginning of the file
