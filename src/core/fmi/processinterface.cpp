/*******************************************************************************
 * Copyright (c) 2016 -2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

const char * const CFMUProcessInterface::scmOK = "OK";
const char * const CFMUProcessInterface::scmNOTINITIALIZED = "Not initialized";
const char * const CFMUProcessInterface::scmINTERNALERROR = "Internal Error";

CFMUProcessInterface::CFMUProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mInitialized(false), mValue(0){
}

CFMUProcessInterface::CFMUProcessInterface(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CProcessInterfaceBase(paSrcRes, 0, paInstanceNameId, 0, 0), mInitialized(false), mValue(0) {
  FMU_DEBUG_LOG(GET_FMU_INSTANCE_FROM_FB(this), "Someone called the wrong constructor.\n") //Why do I need this constructor?
}

CFMUProcessInterface::~CFMUProcessInterface(){
}

void CFMUProcessInterface::setValueContainer(fmuValueContainer* paValueContainer){
  mValue = paValueContainer;
}

bool CFMUProcessInterface::initialise(bool paIsInput){
  NOT_USED(paIsInput)
  if(0 != mValue){
    STATUS() = scmOK;
    mInitialized = true;
    FMU_DEBUG_LOG(GET_FMU_INSTANCE_FROM_FB(this), "Input/Output Initialize\n")
  }
  else{
    STATUS() = scmINTERNALERROR;
    FMU_DEBUG_LOG(GET_FMU_INSTANCE_FROM_FB(this), "ERROR: Input/Output not initialized. You should set a pointer to the fmuContainer for this IO\n")
    mInitialized =  false;
  }
  return mInitialized;
}

bool CFMUProcessInterface::deinitialise(){
  STATUS() = scmOK;
  mInitialized = false;
  return true;
}

bool CFMUProcessInterface::readPin(){
  if(true == mInitialized){
    IN_X() = *mValue->getValueAsBool();
    STATUS() = scmOK;
    return true;
  }
  STATUS() = scmNOTINITIALIZED;
  return false;
}

bool CFMUProcessInterface::writePin(){
  if(true == mInitialized){
    mValue->setValue(OUT_X());
    STATUS() = scmOK;
    return true;
  }
  STATUS() = scmNOTINITIALIZED;
  return false;
}

bool CFMUProcessInterface::readWord(){
  if(true == mInitialized){
    IN_W() = static_cast<TForteWord>(*mValue->getValueAsInt());
    STATUS() = scmOK;
    return true;
  }
  STATUS() = scmNOTINITIALIZED;
  return false;
}

bool CFMUProcessInterface::writeWord(){
  if(true == mInitialized){
    mValue->setValue(OUT_W());
    STATUS() = scmOK;
    return true;
  }
  STATUS() = scmNOTINITIALIZED;
  return false;
}

