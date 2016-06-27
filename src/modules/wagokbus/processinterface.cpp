/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Milan Vathoopan, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "processinterface.h"
#include <sstream>
#include <datatype.h>

DEFINE_SINGLETON(WagoPFCProcessInterface::CKBusHandler)

WagoPFCProcessInterface::WagoPFCProcessInterface(CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
    TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData),
    mSlot(0), mPin(0),m_bInitialized(false),pa_acIndata(false){

}

WagoPFCProcessInterface::~WagoPFCProcessInterface(){
 deinitialise();
}

bool WagoPFCProcessInterface::initialise(bool paInput){

	m_bInitialized = false;
	std::vector<std::string> paramsList(generateParameterList());
	char *ptr;

	if(!paramsList.empty())	{
		mSlot = strtol(paramsList[0].c_str(),&ptr,10);
		mPin = strtol(paramsList[1].c_str(),&ptr,10);

	}

	if ((CKBusHandler::getInstance().getTerminalId(mSlot)))
	{

		m_pstTerminalInfo = CKBusHandler::getInstance().getTerminalInfo(mSlot);
    	if(0 != m_pstTerminalInfo){
    		if(paInput)
    		{
    			CKBusHandler::getInstance().registerKBusReadFB(this);
    		}

    		QO() = QI();

    		if(!CKBusHandler::getInstance().isAlive()){
    		    CKBusHandler::getInstance().start();
    		}

    		      m_bInitialized = true;
    		      setEventChainExecutor(m_poInvokingExecEnv);
    		      return m_bInitialized;

    							}
	}
}

bool WagoPFCProcessInterface::deinitialise(){

	CKBusHandler::getInstance().unregisterKBusReadFB(this);
	return true;
}

bool WagoPFCProcessInterface::readPin(){
	return true;
}

bool WagoPFCProcessInterface::writePin(){

	CKBusHandler::getInstance().writeOutputDataToKBus(mPin, OUT_X());
	return true;
}

bool WagoPFCProcessInterface::checkInputData(){

	bool retVal = false;
	bool OldValue = pa_acIndata ;
	CKBusHandler::getInstance().readInputDatafromKBus(mPin, &pa_acIndata);
	if (OldValue != pa_acIndata)
	{
		IN_X() = pa_acIndata;
		retVal = true;
    }
	return retVal;

}

WagoPFCProcessInterface::CKBusHandler::CKBusHandler() :
	m_unTaskId(0){ // 0 has been taken from example may needs to be rechecked
    tDeviceInfo deviceList[10]; // the list of devices given by the ADI
    size_t nrDevicesFound; // number of devices found
    m_pstAppDevInterface = adi_GetApplicationInterface();
    m_pstAppDevInterface->Init();
    m_pstAppDevInterface->ScanDevices();
    m_pstAppDevInterface->GetDeviceList(sizeof(deviceList), deviceList, &nrDevicesFound);
     // find kbus device

    for(size_t i = 0; i < nrDevicesFound; ++i){

    	if(strcmp(deviceList[i].DeviceName, "libpackbus") == 0){
             m_nKBusDeviceId = deviceList[i].DeviceId;
             if(m_pstAppDevInterface->OpenDevice(m_nKBusDeviceId) == DAL_SUCCESS){
               if(loadTerminalInformation()){
                 return; //we successfully initialized everything so we can return
               }
             }
           }
         }

         //if we are here the initialization could not be finished clean up and close everything
         closeKBusInterface();
				}

 WagoPFCProcessInterface::CKBusHandler::~CKBusHandler(){
	 closeKBusInterface();
 }

 bool WagoPFCProcessInterface::CKBusHandler::isKBusRunning(){

	 return ((isAlive()) && (m_nKBusDeviceId != scm_nInvalidDeviceId));
 }

 bool WagoPFCProcessInterface::CKBusHandler::loadTerminalInformation(){

	 bool bRetVal = false;

	 if(KbusInfo_Failed != ldkc_KbusInfo_Create()){

           if(KbusInfo_Failed != ldkc_KbusInfo_GetTerminalInfo(OS_ARRAY_SIZE(m_astTerminalDescription), m_astTerminalDescription, &m_unTerminalCount)){

           if(KbusInfo_Failed != ldkc_KbusInfo_GetTerminalList(OS_ARRAY_SIZE(m_aunTerminalIds), m_aunTerminalIds, NULL)){
               bRetVal = true;
             }
             else{
               DEVLOG_ERROR("CKBusHandler: ldkc_KbusInfo_GetTerminalList() failed\n");
             }
           }
           else{
             DEVLOG_ERROR("CKBusHandler: ldkc_KbusInfo_GetTerminalInfo() failed\n");
           }
           bRetVal = true;

        }
         else{
           DEVLOG_ERROR("CKBusHandler: ldkc_KbusInfo_Create() failed\n");
         }

         if(!bRetVal){
           ldkc_KbusInfo_Destroy();
         }

         return bRetVal;
 }

 bool WagoPFCProcessInterface::CKBusHandler::getTerminalId(TForteUInt8 pa_unSlot)
 {
	 u16 unRetVal = 0;
	 bool retVal = false;
	 if(pa_unSlot <= m_unTerminalCount){
		 unRetVal = m_aunTerminalIds[pa_unSlot];
		 retVal = true;
	 }
	 return retVal;
 }
 tldkc_KbusInfo_TerminalInfo *WagoPFCProcessInterface::CKBusHandler::getTerminalInfo(TForteUInt8 pa_unSlot){

	 tldkc_KbusInfo_TerminalInfo *pstRetVal = 0;
	 if(pa_unSlot <= m_unTerminalCount){
		 pstRetVal = &(m_astTerminalDescription[pa_unSlot]);
	 }
	 return pstRetVal;
 }

void WagoPFCProcessInterface::CKBusHandler::run(){
         	     	 //TODO add thread priority settings
	bool retVal= false;
	tApplicationStateChangedEvent stEvent;
    // Set application state to "Running" to drive kbus by ourselves.
	stEvent.State = ApplicationState_Running;
	if(DAL_SUCCESS == m_pstAppDevInterface->ApplicationStateChanged(stEvent)){
         while(isAlive()){
        	 usleep(10000); // wait 10 ms  TODO make this configurable
        	 if(!triggerKBusCycle()){
        		 //we have severe problem exit KBus handling thread
        		 //TODO check how can we recover or at least inform the user
        		 break;

        	 }
        	 // read inputs inform FBs
        	 updateReadData();

         }
	}
	else{
		DEVLOG_ERROR("CKBusHandler: Set application state to 'Running' failed\n");
	}
	closeKBusInterface();
}


bool WagoPFCProcessInterface::CKBusHandler::triggerKBusCycle(){
	bool bRetVal = false;
	uint32_t unPushRetVal = 0;

	if(DAL_SUCCESS == m_pstAppDevInterface->CallDeviceSpecificFunction("libpackbus_Push", &unPushRetVal)){
		if(DAL_SUCCESS == unPushRetVal){
			m_pstAppDevInterface->WatchdogTrigger();
			bRetVal = true;
		}
		else{
			DEVLOG_ERROR("CKBusHandler: Function 'libpackbus_Push' failed\n");
		}
	}
	else{
		DEVLOG_ERROR("CKBusHandler: CallDeviceSpecificFunction for 'libpackbus_Push' failed\n");
	}
	return bRetVal;
}

std::vector<std::string> WagoPFCProcessInterface::generateParameterList(){

	std:: stringstream streamBuf(std::string(PARAMS().getValue()));
    std:: vector<std::string> retVal;
    std:: string segment;

	while(std::getline(streamBuf, segment, '.')){   //seperate the PARAMS input by '.' for easier processing
	      retVal.push_back(segment);
	}
	return retVal;
}

void WagoPFCProcessInterface::CKBusHandler::updateReadData(){

	//long pa_Value = strtol(m_acIndata, NULL, 16);
	m_oReadFBListSync.lock();
	m_pstAppDevInterface->ReadStart(m_nKBusDeviceId, m_unTaskId); /* lock PD-In data */
	TReadFBContainer::Iterator itEnd(m_lstReadFBList.end());
	for(TReadFBContainer::Iterator itRunner = m_lstReadFBList.begin(); itRunner != itEnd; ++itRunner){
		if((*itRunner)->checkInputData()){
			// If data has changed, give the indication event
			startNewEventChain(*itRunner);
		}
	}

	m_pstAppDevInterface->ReadEnd(m_nKBusDeviceId, m_unTaskId); /* unlock PD-In data */
	m_oReadFBListSync.unlock();
}

void WagoPFCProcessInterface::CKBusHandler::registerKBusReadFB(WagoPFCProcessInterface *pa_poFB){
	m_oReadFBListSync.lock();
	m_lstReadFBList.push_back(pa_poFB);
	m_oReadFBListSync.unlock();
}

void WagoPFCProcessInterface::CKBusHandler::unregisterKBusReadFB(WagoPFCProcessInterface *pa_poFB){
	m_oReadFBListSync.lock();
	TReadFBContainer::Iterator itRunner(m_lstReadFBList.begin());
	TReadFBContainer::Iterator itRefNode(m_lstReadFBList.end());
	TReadFBContainer::Iterator itEnd(m_lstReadFBList.end());

	while(itRunner != itEnd){
		if(*itRunner == pa_poFB){
			if(itRefNode == itEnd){
				m_lstReadFBList.pop_front();
			}
			else{
				m_lstReadFBList.eraseAfter(itRefNode);
			}
			break;
		}

		itRefNode = itRunner;
		++itRunner;
	}

	m_oReadFBListSync.unlock();
}

void WagoPFCProcessInterface::CKBusHandler::writeOutputDataToKBus(TForteUInt32 paPin, bool pa_acData){
	m_pstAppDevInterface->WriteStart(m_nKBusDeviceId, m_unTaskId);
	m_pstAppDevInterface->WriteBool(m_nKBusDeviceId, m_unTaskId, paPin, pa_acData);
	m_pstAppDevInterface->WriteEnd(m_nKBusDeviceId, m_unTaskId);
}

void WagoPFCProcessInterface::CKBusHandler::readInputDatafromKBus(TForteUInt32 paPin, bool *pa_acInData){

	m_pstAppDevInterface->ReadBool(m_nKBusDeviceId, m_unTaskId, paPin, pa_acInData);

}
void WagoPFCProcessInterface::CKBusHandler::closeKBusInterface()
{
	if(0 != m_pstAppDevInterface){
		m_pstAppDevInterface->CloseDevice(m_nKBusDeviceId); // close kbus device
		m_pstAppDevInterface->Exit(); // disconnect ADI-Interface

		m_nKBusDeviceId = scm_nInvalidDeviceId;
		m_pstAppDevInterface = 0;
	}

}
void WagoPFCProcessInterface::CKBusHandler::enableHandler(void){
}
void WagoPFCProcessInterface::CKBusHandler::disableHandler(void){
}
void  WagoPFCProcessInterface::CKBusHandler::setPriority(int pa_nPriority){
}
int WagoPFCProcessInterface::CKBusHandler::getPriority(void) const{
	return 0;
}
