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
    g_nStringIdQI, g_nStringIdPARAMS };

const CStringDictionary::TStringId Master::scm_anDataInputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const CStringDictionary::TStringId Master::scm_anDataOutputNames[] = {
    g_nStringIdQO, g_nStringIdSTATUS };

const CStringDictionary::TStringId Master::scm_anDataOutputTypeIds[] = {
    g_nStringIdBOOL, g_nStringIdWSTRING };

const TForteInt16 Master::scm_anEIWithIndexes[] = { 0 };
const TDataIOID Master::scm_anEIWith[] = { 0, 1, 255 };
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
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 2,
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
      onError();
    }
  }

  switch (pa_nEIID) {

  case cg_nExternalEventID:
    if (bus) {
      if (bus->hasError()) {
        STATUS() = bus->getStatus();
        if (false == QO()) {
          DEVLOG_ERROR(
              "emBrick[Master]: Failed to init BusHandler. Reason: %s\n",
              STATUS().getValue());
        } else {
          DEVLOG_ERROR("emBrick[Master]: Runtime error. Reason: %s\n",
              STATUS().getValue());
        }
        onError();
      } else {
        // Init configuration
        BusAdapterOut().INDEX() = 0;
        sendAdapterEvent(scm_nBusAdapterAdpNum, BusAdapter::scm_nEventINITID);
      }
    }
    break;

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

  bus->start();
}

void Master::onError() {
  // TODO Replace with unblocking timer
  sleep(errorCounter);

  errorCounter++;

  // Check if bus is running
  if (bus->isAlive()) {
    bus->end();
  }

  // ReInit Bus
  if (errorCounter < 3) {
    init();
  } else {
    QO() = false;
    sendOutputEvent(scm_nEventINITOID);
  }
}

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */
