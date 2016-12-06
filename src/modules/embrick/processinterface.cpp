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

#include "handler/bus.h"

using namespace EmBrick;

ProcessInterface::ProcessInterface(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId,
    TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId,
        paFBConnData, paFBVarsData) {
  isReady = false;
  handle = NULL;
}

ProcessInterface::~ProcessInterface() {
  deinitialise();
}

bool ProcessInterface::initialise(bool paIsInput) {
  DEVLOG_INFO("Init ProcessInterface\n");

  BusHandler &bus = BusHandler::getInstance();
  if (bus.ready())
    setup();

  return true;
}

bool ProcessInterface::deinitialise() {
  DEVLOG_INFO("DeInit\n");
  return true;
}

bool ProcessInterface::readPin() {
  if (!ready())
    return false;

  // TODO Implement for IW etc.
  IN_X() = ((BitSlaveHandle*) handle)->get();

  return true;
}

bool ProcessInterface::writePin() {
  if (!ready())
    return false;

  // TODO Implement for QW etc.
  ((BitSlaveHandle*) handle)->set(OUT_X());

  return true;
}

bool ProcessInterface::ready() {
  if (!isReady) {
    BusHandler &bus = BusHandler::getInstance();
    if (bus.ready())
      setup();
  }

  return isReady;
}

void ProcessInterface::setup() {
  BusHandler &bus = BusHandler::getInstance();

  // TODO Replace with dynamic port mapping
  handle = bus.getSlave(2)->getOutputHandle(4);

  isReady = true;
}
