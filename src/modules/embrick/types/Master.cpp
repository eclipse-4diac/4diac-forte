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

#include "Master.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Master_gen.cpp"
#endif

namespace EmBrick {
namespace FunctionBlocks {

DEFINE_FIRMWARE_FB(Master, g_nStringIdEBMaster)

const CStringDictionary::TStringId Master::scm_anDataInputNames[] = {
    g_nStringIdQI, g_nStringIdBusInterface, g_nStringIdBusSelectPin,
    g_nStringIdBusInitSpeed, g_nStringIdBusLoopSpeed,
    g_nStringIdSlaveUpdateInterval };

const CStringDictionary::TStringId Master::scm_anDataInputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT, g_nStringIdUDINT,
    g_nStringIdUDINT, g_nStringIdUINT };

const CStringDictionary::TStringId Master::scm_anDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId Master::scm_anDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 Master::scm_anEIWithIndexes[] = { 0 };
const TDataIOID Master::scm_anEIWith[] = { 0, 3, 5, 4, 1, 2, 255 };
const CStringDictionary::TStringId Master::scm_anEventInputNames[] = {
    g_nStringIdINIT };

const TDataIOID Master::scm_anEOWith[] = { 0, 1, 255, 0, 1, 255 };
const TForteInt16 Master::scm_anEOWithIndexes[] = { 0, 3, -1 };
const CStringDictionary::TStringId Master::scm_anEventOutputNames[] = {
    g_nStringIdINITO, g_nStringIdIND };

const SAdapterInstanceDef Master::scm_astAdapterInstances[] = { {
    g_nStringIdEBBusAdapter, g_nStringIdBusAdapterOut, true } };

const SFBInterfaceSpec Master::scm_stFBInterfaceSpec = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 6,
    scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 1, scm_astAdapterInstances };

const char * const Master::scmOK = "OK";
const char * const Master::scmFailedToInitSlaves =
    "Failed to init slaves. Check if the configuration matches the hardware setup.";

Master::Master(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId,
    TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CEventSourceFB(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData,
        paFBVarsData) {
  setEventChainExecutor(paSrcRes->getResourceEventExecution());

  bus = 0;
  errorCounter = 0;
}

Master::~Master() {
  if (bus != 0) {
    if (bus->isAlive())
      bus->end();

    bus->delegate = 0;
  }
}

void Master::setInitialValues() {
  BusInterface() = 1;
  BusSelectPin() = 49;
  BusInitSpeed() = 300000;
  BusLoopSpeed() = 700000;
  SlaveUpdateInterval() = 25;
}

void Master::executeEvent(int pa_nEIID) {
  if (BusAdapterOut().INITO() == pa_nEIID) {
    QO() = BusAdapterOut().QO();

    if (true == QO()) {
      // Successful init
      errorCounter = 0;
      STATUS() = scmOK;

      sendOutputEvent(scm_nEventINITOID);
    } else {
      STATUS() = scmFailedToInitSlaves;
      DEVLOG_ERROR("emBrick[Master]: Failed to init BusHandler. Reason: %s\n",
          STATUS().getValue());
      onError(false);
    }
  }

  switch (pa_nEIID) {

  case cg_nExternalEventID:
    return onBusEvent();

  case scm_nEventINITID:
    if (true == QI()) {
      init();
    } else {
      // Stop BusHandler
      if (bus->isAlive())
        bus->end();

      QO() = true;
      sendOutputEvent(scm_nEventINITOID);
    }
    break;
  }
}

void Master::init() {
  QO() = false;

  // Init and wait for BusHandler
  bus = &BusHandler::getInstance();
  if (bus->isAlive()) {
    DEVLOG_ERROR(
        "emBrick[Master]: BusHandler already running. Only one master function block is permitted.\n");

    QO() = false;
    sendOutputEvent(scm_nEventINITOID);
    return;
  }

  setEventChainExecutor(m_poInvokingExecEnv);
  bus->delegate = this;

  BusHandler::Config config;
  config.BusInterface = BusInterface();
  config.BusSelectPin = BusSelectPin();
  config.BusInitSpeed = BusInitSpeed();
  config.BusLoopSpeed = BusLoopSpeed();
  bus->setConfig(config);

  bus->start();
}

void Master::onBusEvent() {
  if (bus) {
    if (bus->hasError()) {
      STATUS() = bus->getStatus();
      if (false == QO()) {
        DEVLOG_ERROR("emBrick[Master]: Failed to init BusHandler. Reason: %s\n",
            STATUS().getValue());
      } else {
        DEVLOG_ERROR("emBrick[Master]: Runtime error. Reason: %s\n",
            STATUS().getValue());
      }
      onError();
    } else {
      // Init configuration
      BusAdapterOut().INDEX() = 0;
      BusAdapterOut().UpdateInterval() = SlaveUpdateInterval();
      sendAdapterEvent(scm_nBusAdapterAdpNum, BusAdapter::scm_nEventINITID);
    }
  }
}

void Master::onError(bool isFatal) {
  // TODO Replace with unblocking timer
  sleep(errorCounter);

  errorCounter++;
  bool retry = errorCounter < 5;

  // Check if bus is running
  if ((isFatal || retry) && bus->isAlive()) {
    bus->end();
  }

  // ReInit Bus
  if (retry) {
    init();
  } else {
    QO() = false;
    sendOutputEvent(scm_nEventINITOID);

    // Reset error counter if it was not a fatal error
    if (!isFatal) {
      errorCounter = 0;
    }
  }
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */
