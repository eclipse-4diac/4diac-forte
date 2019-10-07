/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Milan Vathoopan, Alois Zoitl - initial API and implementation and/or initial documentation
 *    Milan Vathoopan  - Extended functionality with anlog input and output blocks
 *******************************************************************************/
#include "processinterface.h"
#include <sstream>
#include <datatype.h>
#include <extevhandlerhelper.h>

DEFINE_HANDLER(WagoPFCProcessInterface::CKBusHandler)

WagoPFCProcessInterface::WagoPFCProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mSlot(0), mChannel(0), mTerminalInfo(0), mInitialized(false) {
}

WagoPFCProcessInterface::~WagoPFCProcessInterface() {
  deinitialise();
}

bool WagoPFCProcessInterface::initialise(bool paInput) {
  mInitialized = false;
  std::vector<std::string> paramsList(generateParameterList());
  char *pBuffer;

  if(!paramsList.empty()) {
    mSlot = strtol(paramsList[0].c_str(), &pBuffer, 10);
    mChannel = strtol(paramsList[1].c_str(), &pBuffer, 10);
  }

  if((getExtEvHandler<CKBusHandler>(*this).isIdValid(static_cast<TForteUInt8>(mSlot)))) {
    mTerminalInfo = getExtEvHandler<CKBusHandler>(*this).getTerminalInfo(static_cast<TForteUInt8>(mSlot));
    if(0 != mTerminalInfo) {
      if((paInput) && (getDO(2)->getDataTypeID() == CIEC_ANY::e_BOOL)) {
        getExtEvHandler<CKBusHandler>(*this).registerKBusReadFB(this);
      }

      QO() = QI();

      if(!getExtEvHandler<CKBusHandler>(*this).isAlive()) {
        getExtEvHandler<CKBusHandler>(*this).start();
      }

      mInitialized = true;
      setEventChainExecutor(m_poInvokingExecEnv);
    }
  }
  return mInitialized;
}

bool WagoPFCProcessInterface::deinitialise() {
  getExtEvHandler<CKBusHandler>(*this).unregisterKBusReadFB(this);
  return true;
}

bool WagoPFCProcessInterface::readPin() {
  return true;
}

bool WagoPFCProcessInterface::writePin() {
  getExtEvHandler<CKBusHandler>(*this).writeOutputDataBitToKBus(mTerminalInfo, mChannel, OUT_X());
  return true;
}

bool WagoPFCProcessInterface::readWord() {
  TForteWord inDataWord(0);
  getExtEvHandler<CKBusHandler>(*this).readInputDataWordfromKBus(mTerminalInfo, mChannel, &inDataWord);
  IN_W() = inDataWord;
  return true;
}

bool WagoPFCProcessInterface::writeWord() {
  getExtEvHandler<CKBusHandler>(*this).writeOutputDataWordToKBus(mTerminalInfo, mChannel, OUT_W());
  return true;
}

bool WagoPFCProcessInterface::checkInputData() {
  bool retVal = false;
  bool inDataBool(false);
  getExtEvHandler<CKBusHandler>(*this).readInputDataBitfromKBus(mTerminalInfo, mChannel, &inDataBool);
  if(inDataBool != IN_X()) {
    IN_X() = inDataBool;
    retVal = true;
  }
  return retVal;
}

WagoPFCProcessInterface::CKBusHandler::CKBusHandler(CDeviceExecution& paDeviceExecution) :
    CExternalEventHandler(paDeviceExecution), mTaskId(0) { // 0 has been taken from example may needs to be rechecked
  tDeviceInfo deviceList[10]; // the list of devices given by the ADI
  size_t nrDevicesFound; // number of devices found
  mAppDevInterface = adi_GetApplicationInterface();
  mAppDevInterface->Init();
  mAppDevInterface->ScanDevices();
  mAppDevInterface->GetDeviceList(sizeof(deviceList), deviceList, &nrDevicesFound);

  // find kbus device
  for(size_t i = 0; i < nrDevicesFound; ++i) {
    if(strcmp(deviceList[i].DeviceName, "libpackbus") == 0) {
      mKBusDeviceId = deviceList[i].DeviceId;
      if(mAppDevInterface->OpenDevice(mKBusDeviceId) == DAL_SUCCESS) {
        if(loadTerminalInformation()) {
          return; //we successfully initialized everything so we can return
        }
      }
    }
  }

  //if we are here the initialization could not be finished clean up and close everything
  closeKBusInterface();
}

WagoPFCProcessInterface::CKBusHandler::~CKBusHandler() {
  closeKBusInterface();
}

bool WagoPFCProcessInterface::CKBusHandler::isKBusRunning() {
  return ((isAlive()) && (mKBusDeviceId != scmInvalidDeviceId));
}

bool WagoPFCProcessInterface::CKBusHandler::loadTerminalInformation() {
  bool bRetVal = false;

  if(KbusInfo_Failed != ldkc_KbusInfo_Create()) {
    if(KbusInfo_Failed != ldkc_KbusInfo_GetTerminalInfo(OS_ARRAY_SIZE(mTerminalDescription), mTerminalDescription, &mTerminalCount)) {
      if(KbusInfo_Failed != ldkc_KbusInfo_GetTerminalList(OS_ARRAY_SIZE(mTerminalIds), mTerminalIds, NULL)) {
        bRetVal = true;
      } else {
        DEVLOG_ERROR("CKBusHandler: ldkc_KbusInfo_GetTerminalList() failed\n");
      }
    } else {
      DEVLOG_ERROR("CKBusHandler: ldkc_KbusInfo_GetTerminalInfo() failed\n");
    }
    bRetVal = true;
  } else {
    DEVLOG_ERROR("CKBusHandler: ldkc_KbusInfo_Create() failed\n");
  }

  if(!bRetVal) {
    ldkc_KbusInfo_Destroy();
  }

  return bRetVal;
}

bool WagoPFCProcessInterface::CKBusHandler::isIdValid(TForteUInt8 paSlot) {
  return (paSlot <= mTerminalCount);
}

tldkc_KbusInfo_TerminalInfo *WagoPFCProcessInterface::CKBusHandler::getTerminalInfo(TForteUInt8 paSlot) {
  tldkc_KbusInfo_TerminalInfo *pstRetVal = 0;
  if(paSlot <= mTerminalCount) {
    pstRetVal = &(mTerminalDescription[paSlot]);
  }
  return pstRetVal;
}

void WagoPFCProcessInterface::CKBusHandler::run() {
  //TODO add thread priority settings
  //bool retVal= false;
  tApplicationStateChangedEvent stEvent;
  // Set application state to "Running" to drive kbus by ourselves.
  stEvent.State = ApplicationState_Running;
  if(DAL_SUCCESS == mAppDevInterface->ApplicationStateChanged(stEvent)) {
    while(isAlive()) {
      CThread::sleepThread(10); // wait 10 ms  TODO make this configurable
      if(!triggerKBusCycle()) {
        //we have severe problem exit KBus handling thread
        //TODO check how can we recover or at least inform the user
        break;
      }
      // read inputs inform FBs
      updateReadData();
    }
  } else {
    DEVLOG_ERROR("CKBusHandler: Set application state to 'Running' failed\n");
  }
  closeKBusInterface();
}

bool WagoPFCProcessInterface::CKBusHandler::triggerKBusCycle() {
  bool bRetVal = false;
  uint32_t unPushRetVal = 0;

  if(DAL_SUCCESS == mAppDevInterface->CallDeviceSpecificFunction("libpackbus_Push", &unPushRetVal)) {
    if(DAL_SUCCESS == unPushRetVal) {
      mAppDevInterface->WatchdogTrigger();
      bRetVal = true;
    } else {
      DEVLOG_ERROR("CKBusHandler: Function 'libpackbus_Push' failed\n");
    }
  } else {
    DEVLOG_ERROR("CKBusHandler: CallDeviceSpecificFunction for 'libpackbus_Push' failed\n");
  }
  return bRetVal;
}

std::vector<std::string> WagoPFCProcessInterface::generateParameterList() {
  std::stringstream streamBuf(std::string(PARAMS().getValue()));
  std::vector<std::string> retVal;
  std::string segment;

  while(std::getline(streamBuf, segment, '.')) { //seperate the PARAMS input by '.' for easier processing
    retVal.push_back(segment);
  }
  return retVal;
}

void WagoPFCProcessInterface::CKBusHandler::updateReadData() {
  //long pa_Value = strtol(m_acIndata, NULL, 16);
  mReadFBListSync.lock();
  mAppDevInterface->ReadStart(mKBusDeviceId, mTaskId); /* lock PD-In data */
  TReadFBContainer::Iterator itEnd(mReadFBList.end());
  for(TReadFBContainer::Iterator itRunner = mReadFBList.begin(); itRunner != itEnd; ++itRunner) {
    if((*itRunner)->checkInputData()) {
      // If data has changed, give the indication event
      startNewEventChain(*itRunner);
    }
  }

  mAppDevInterface->ReadEnd(mKBusDeviceId, mTaskId); /* unlock PD-In data */
  mReadFBListSync.unlock();
}

void WagoPFCProcessInterface::CKBusHandler::registerKBusReadFB(WagoPFCProcessInterface *paFB) {
  mReadFBListSync.lock();
  mReadFBList.pushBack(paFB);
  mReadFBListSync.unlock();
}

void WagoPFCProcessInterface::CKBusHandler::unregisterKBusReadFB(WagoPFCProcessInterface *paFB) {
  mReadFBListSync.lock();
  TReadFBContainer::Iterator itRunner(mReadFBList.begin());
  TReadFBContainer::Iterator itRefNode(mReadFBList.end());
  TReadFBContainer::Iterator itEnd(mReadFBList.end());

  while(itRunner != itEnd) {
    if(*itRunner == paFB) {
      if(itRefNode == itEnd) {
        mReadFBList.popFront();
      } else {
        mReadFBList.eraseAfter(itRefNode);
      }
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }

  mReadFBListSync.unlock();
}

void WagoPFCProcessInterface::CKBusHandler::writeOutputDataBitToKBus(tldkc_KbusInfo_TerminalInfo *paTerminal, TForteUInt32 paChannel, bool paOutDataBool) {
  mAppDevInterface->WriteStart(mKBusDeviceId, mTaskId);
  mAppDevInterface->WriteBool(mKBusDeviceId, mTaskId, ((paTerminal->OffsetOutput_bits) + paChannel), paOutDataBool);
  mAppDevInterface->WriteEnd(mKBusDeviceId, mTaskId);
}

void WagoPFCProcessInterface::CKBusHandler::readInputDataBitfromKBus(tldkc_KbusInfo_TerminalInfo *paTerminal, TForteUInt32 paChannel, bool *paInDataBool) {
  mAppDevInterface->ReadBool(mKBusDeviceId, mTaskId, ((paTerminal->OffsetInput_bits) + paChannel), paInDataBool);
}

void WagoPFCProcessInterface::CKBusHandler::readInputDataWordfromKBus(tldkc_KbusInfo_TerminalInfo *paTerminal, TForteUInt32 paChannel,
    TForteWord *paInDataWord) {
  TForteByte InData[2];
  mAppDevInterface->ReadStart(mKBusDeviceId, mTaskId); /* lock PD-In data */
  mAppDevInterface->ReadBytes(mKBusDeviceId, mTaskId, ((paTerminal->OffsetInput_bits) + (paChannel * 2)), 2, InData);
  mAppDevInterface->ReadEnd(mKBusDeviceId, mTaskId); /* unlock PD-In data */
  *paInDataWord = static_cast<TForteWord>((static_cast<TForteWord>(InData[1]) << 8) + (static_cast<TForteWord>(InData[0])));
}

void WagoPFCProcessInterface::CKBusHandler::writeOutputDataWordToKBus(tldkc_KbusInfo_TerminalInfo *paTerminal, TForteUInt32 paChannel,
    TForteWord paOutDataWord) {
  TForteByte outData[2];
  outData[0] = static_cast<TForteByte>(paOutDataWord & 0x00FF);
  outData[1] = static_cast<TForteByte>(paOutDataWord >> 8);
  mAppDevInterface->WriteStart(mKBusDeviceId, mTaskId);
  mAppDevInterface->WriteBytes(mKBusDeviceId, mTaskId, ((paTerminal->OffsetOutput_bits) + (paChannel * 2)), 2, outData);
  mAppDevInterface->WriteEnd(mKBusDeviceId, mTaskId);
}

void WagoPFCProcessInterface::CKBusHandler::closeKBusInterface() {
  if(0 != mAppDevInterface) {
    mAppDevInterface->CloseDevice(mKBusDeviceId); // close kbus device
    mAppDevInterface->Exit(); // disconnect ADI-Interface
    mKBusDeviceId = scmInvalidDeviceId;
    mAppDevInterface = 0;
  }
}

void WagoPFCProcessInterface::CKBusHandler::enableHandler(void) {
}

void WagoPFCProcessInterface::CKBusHandler::disableHandler(void) {
}

void WagoPFCProcessInterface::CKBusHandler::setPriority(int) {
}

int WagoPFCProcessInterface::CKBusHandler::getPriority(void) const {
  return 0;
}
