/*******************************************************************************
 * Copyright (c) 2014 - 2015 fortiss GmbH, 2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger - initial API and implementation and/or initial documentation
 *   Martin Melik-Merkumians - Adds methods for IB, QB, IL, and QL
 *******************************************************************************/
#include "../../core/esfb.h"
#include "../../core/resource.h"

#ifndef PROCESSINTERFACEBASE_H_
#define PROCESSINTERFACEBASE_H_


class CProcessInterfaceBase : public CEventSourceFB{
  public:
    CProcessInterfaceBase(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId) :
          CEventSourceFB(paContainer, paInterfaceSpec, paInstanceNameId){
      setEventChainExecutor(getResource()->getResourceEventExecution());
    }

    ~CProcessInterfaceBase() override = default;

  protected:
    CIEC_BOOL &QI(){
      return *static_cast<CIEC_BOOL*>(getDI(0));
    }

    CIEC_STRING &PARAMS(){
      return *static_cast<CIEC_STRING*>(getDI(1));
    }

    CIEC_BOOL &QO(){
      return *static_cast<CIEC_BOOL*>(getDO(0));
    }

    CIEC_STRING &STATUS(){
      return *static_cast<CIEC_STRING*>(getDO(1));
    }

    CIEC_BOOL &IN_X(){
      return *static_cast<CIEC_BOOL*>(getDO(2));
    }

    CIEC_BOOL &OUT_X() {
      return *static_cast<CIEC_BOOL*>(getDI(2));
    };

    CIEC_BYTE &IN_B(){
      return *static_cast<CIEC_BYTE*>(getDO(2));
    };

    CIEC_BYTE &OUT_B() {
      return *static_cast<CIEC_BYTE*>(getDI(2));
    };

    CIEC_WORD &IN_W(){
      return *static_cast<CIEC_WORD*>(getDO(2));
    };

    CIEC_WORD &OUT_W() {
      return *static_cast<CIEC_WORD*>(getDI(2));
    };

    CIEC_DWORD &IN_D(){
      return *static_cast<CIEC_DWORD*>(getDO(2));
    }

    CIEC_DWORD &OUT_D() {
      return *static_cast<CIEC_DWORD*>(getDI(2));
    };

    CIEC_LWORD &IN_L(){
      return *static_cast<CIEC_LWORD*>(getDO(2));
    }

    CIEC_LWORD &OUT_L() {
      return *static_cast<CIEC_LWORD*>(getDI(2));
    };

    //TODO move pin checking and managing code into this class. Can be solved with recurring template pattern
};

#endif
