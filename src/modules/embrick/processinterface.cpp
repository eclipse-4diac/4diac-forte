/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "processinterface.h"
#include <io/mapper.h>

namespace EmBrick {

ProcessInterface::ProcessInterface(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId,
    TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId,
        paFBConnData, paFBVarsData), IOObserver() {
  isReady = false;
  isInput = false;
}

ProcessInterface::~ProcessInterface() {
  deinitialise();
}

bool ProcessInterface::initialise(bool paIsInput) {
  isInput = paIsInput;

  // Register interface
  IOMapper::getInstance().registerObserver(getInstanceName(), this);

  return isReady;
}

bool ProcessInterface::deinitialise() {
  // Deregister interface
  IOMapper::getInstance().deregisterObserver(this);

  return !isReady;
}

bool ProcessInterface::readPin() {
  if (!isReady)
    return false;

  // TODO Implement for IW etc.
  IN_X() = ((BitSlaveHandle*) handle)->get();

  return true;
}

bool ProcessInterface::writePin() {
  if (!isReady)
    return false;

  // TODO Implement for QW etc.
  ((BitSlaveHandle*) handle)->set(OUT_X());

  return true;
}

bool ProcessInterface::onChange() {
  QO() = readPin();

  return true;
}

void ProcessInterface::onHandle(IOHandle* handle) {
  IOObserver::onHandle(handle);

  if (isInput) {
    setEventChainExecutor(m_poInvokingExecEnv);
  }

  QO() = true;
  isReady = true;
}

void ProcessInterface::dropHandle() {
  IOObserver::dropHandle();

  QO() = false;
  isReady = false;
}

}
