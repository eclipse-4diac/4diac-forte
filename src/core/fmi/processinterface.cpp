
/*******************************************************************************
 * Copyright (c) 2016 -2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "processinterface.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "processinterface_gen.cpp"
#endif
#include <iostream>
#include "fmuInstance.h"

DEFINE_FIRMWARE_FB(CFMUProcessInterface, g_nStringIdCFMUProcessInterface)


const char * const CFMUProcessInterface::scmOK = "OK";
const char * const CFMUProcessInterface::scmNOTINITIALIZED = "Not initialized";
const char * const CFMUProcessInterface::scmINTERNALERROR = "Internal Error";

CFMUProcessInterface::CFMUProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), m_bInitialized(false), m_value(0){
}

CFMUProcessInterface::CFMUProcessInterface(const CStringDictionary::TStringId paInstanceNameId, CResource *pa_poSrcRes) :
    CProcessInterfaceBase(pa_poSrcRes, 0, paInstanceNameId, 0, 0), m_bInitialized(false), m_value(0) {
  FMU_DEBUG_LOG(GET_FMU_INSTANCE_FROM_FB(this), "Someone called the wrong constructor.\n") //Why do I need this constructor?
}

CFMUProcessInterface::~CFMUProcessInterface(){
}

void CFMUProcessInterface::setValueContainer(fmuValueContainer* pa_valueContainer){
  m_value = pa_valueContainer;
}

bool CFMUProcessInterface::initialise(bool paIsInput){
  NOT_USED(paIsInput)
  if(0 != m_value){
    STATUS() = scmOK;
    m_bInitialized = true;
    FMU_DEBUG_LOG(GET_FMU_INSTANCE_FROM_FB(this), "Input/Output Initialize\n")
  }
  else{
    STATUS() = scmINTERNALERROR;
    FMU_DEBUG_LOG(GET_FMU_INSTANCE_FROM_FB(this), "ERROR: Input/Output not initialized. You should set a pointer to the fmuContainer for this IO\n")
    m_bInitialized =  false;
  }
  return m_bInitialized;
}

bool CFMUProcessInterface::deinitialise(){
  STATUS() = scmOK;
  m_bInitialized = false;
  return true;
}

bool CFMUProcessInterface::readPin(){
  if(true == m_bInitialized){
    IN_X() = *m_value->getValueAsBool();
    STATUS() = scmOK;
    return true;
  }
  STATUS() = scmNOTINITIALIZED;
  return false;
}

bool CFMUProcessInterface::writePin(){
  if(true == m_bInitialized){
    m_value->setValue(OUT_X());
    STATUS() = scmOK;
    return true;
  }
  STATUS() = scmNOTINITIALIZED;
  return false;
}

bool CFMUProcessInterface::readWord(){
  if(true == m_bInitialized){
    IN_W() = static_cast<TForteWord>(*m_value->getValueAsInt());
    STATUS() = scmOK;
    return true;
  }
  STATUS() = scmNOTINITIALIZED;
  return false;
}

bool CFMUProcessInterface::writeWord(){
  if(true == m_bInitialized){
    m_value->setValue(OUT_W());
    STATUS() = scmOK;
    return true;
  }
  STATUS() = scmNOTINITIALIZED;
  return false;
}

