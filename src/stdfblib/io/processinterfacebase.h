/*******************************************************************************
 * Copyright (c) 2014 - 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <esfb.h>
#include <resource.h>

#ifndef PROCESSINTERFACEBASE_H_
#define PROCESSINTERFACEBASE_H_


class CProcessInterfaceBase : public CEventSourceFB{
  public:
    CProcessInterfaceBase(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
          CEventSourceFB(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData){
  setEventChainExecutor(paSrcRes->getResourceEventExecution());
    }

    virtual ~CProcessInterfaceBase(){
    }

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

    CIEC_WORD &IN_W(){
      return *static_cast<CIEC_WORD*>(getDO(2));
    }

    CIEC_WORD &OUT_W() {
      return *static_cast<CIEC_WORD*>(getDI(2));
    };

    CIEC_DWORD &IN_D(){
      return *static_cast<CIEC_DWORD*>(getDO(2));
    }

    CIEC_DWORD &OUT_D() {
      return *static_cast<CIEC_DWORD*>(getDI(2));
    };



    //TODO move pin checking and managing code into this class. Can be solved with recurring template pattern
};

#endif
