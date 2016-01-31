/*******************************************************************************
 * Copyright (c) 2012 - 2014 Profactor GmbH, ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Michael Hofmann, Gerhard Ebenhofer, Ingo Hegny, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include <iostream>
#include "device.h"
#include "resource.h"
#include "monitor.h"
#include "class1objhand.h"
#include "mgmcmd.h"
#include "mgmcmdstruct.h"
#include "utils/criticalregion.h"

T_DEVICE_MAP CMonitor::mDataWatches = T_DEVICE_MAP();
CSyncObject CMonitor::mLockDataWatches = CSyncObject();
TForteInt16 CMonitor::watchResponseSize = 0;
SEventEntry CMonitor::m_stTriggerEvent = SEventEntry(0,0);

EMGMResponse CMonitor::executeMonitoringCommand(CDevice *pa_poDevice, SManagementCMD &pa_rstCommand){
  EMGMResponse eResp = e_RDY;

  switch(pa_rstCommand.m_nCMD){
    case cg_nMGM_CMD_Monitoring_Read_Watches:
      if(!CMonitor::readWatches(pa_poDevice, pa_rstCommand.m_oMonitorResponse)){
        eResp = e_INVALID_OPERATION;
        pa_rstCommand.m_oMonitorResponse.clear();
      } else {
        DEVLOG_DEBUG("Response: %s \n", pa_rstCommand.m_oMonitorResponse.getValue());
      }
      break;
    case cg_nMGM_CMD_Monitoring_Get_Watches:
      if(!CMonitor::getWatches(pa_poDevice, pa_rstCommand)){
        eResp = e_INVALID_OPERATION;
        pa_rstCommand.m_oMonitorResponse.clear();
      } else {
        DEVLOG_DEBUG("Response: %s \n", pa_rstCommand.m_oMonitorResponse.getValue());
      }
      break;
    default:
       eResp = e_UNSUPPORTED_CMD;
       break;
  }
  return eResp;
}

CFunctionBlock * CMonitor::getFbPointer(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId){
  if(device == 0){
		return 0;
	}
	// Get functionblock
	C61499Class1ObjectHandler &devObjHand = device->getObjectHandler();
	CResource *pres = (CResource*) devObjHand.getFB(resourceNameId);
	if(pres == 0){
		return 0;
	}
	C61499Class1ObjectHandler &resObjHand = pres->getObjectHandler();
	CFunctionBlock *fb = resObjHand.getFB(fbNameId);
	return fb;
}


bool CMonitor::wasTriggered(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId) {
	CFunctionBlock *fb = CMonitor::getFbPointer(device, resourceNameId, fbNameId);
	if (fb == 0){ 
		return false;
	}
	bool ret = fb->getUpdated();
	if(ret) {
		fb->setUpdated(false);
	}
	return ret;
}

CResource * CMonitor::getResPointer(CDevice *device, CStringDictionary::TStringId resourceID){
	if(device == 0){
		return 0;
	}
	// Get functionblock
	C61499Class1ObjectHandler &devObjHand = device->getObjectHandler();
	CResource *pres = (CResource*) devObjHand.getFB(resourceID);
	return pres;
}


bool CMonitor::isForced(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId){
 // Read Data from FB

 CFunctionBlock *fb = getFbPointer(device, resourceNameId, fbNameId);
  if(fb == 0){
   return false;
 }

 // get DataInput
 return fb->isForced(portNameId);
}

bool CMonitor::getWatches(CDevice *cDevice, SManagementCMD &pa_oCommand){
  // Go through DeviceMap and create a response-object for every watched fb
  T_DEVICE_MAP_ITER rspResIter;
  T_RESOURCE_CONTAINER_ITER rspFbIter;
  T_FB_CONTAINER_ITER rspPortIter;

  T_DEVICE_MAP *dataWatches = CMonitor::getDeviceMap();

  rspResIter = dataWatches->begin();
  pa_oCommand.m_oMonitorResponse.reserve(watchResponseSize);
  while(rspResIter != dataWatches->end()){
    rspFbIter = rspResIter->second.begin();
    const char *fbRes = CStringDictionary::getInstance().get(rspResIter->first);
    pa_oCommand.m_oMonitorResponse.append("<Resource name=\"");
    pa_oCommand.m_oMonitorResponse.append(fbRes);
    pa_oCommand.m_oMonitorResponse.append("\">");

    while(rspFbIter != rspResIter->second.end()){
      const char *fbName = CStringDictionary::getInstance().get(rspFbIter->first);
      pa_oCommand.m_oMonitorResponse.append("<FB name=\"");
      pa_oCommand.m_oMonitorResponse.append(fbName);
      pa_oCommand.m_oMonitorResponse.append("\">");
      //CFunctionBlock *fb = CMonitor::getFbPointer(this, fbRes, fbName);
      //if (CMonitor::wasTriggered(this, fbRes, fbName)) {
      // create containers for port/event-Data

      // Data Watch
      rspPortIter = rspFbIter->second.addWatchList.begin();
      while(rspPortIter != rspFbIter->second.addWatchList.end()){
        if (pa_oCommand.m_oAdditionalParams.getValue()[0] == '*') {
          pa_oCommand.m_oMonitorResponse.append("<Port name=\"");
          pa_oCommand.m_oMonitorResponse.append(CStringDictionary::getInstance().get( *rspPortIter ));
          pa_oCommand.m_oMonitorResponse.append("\" forced=\"");
          if(CMonitor::isForced(cDevice, rspResIter->first, rspFbIter->first, *rspPortIter)){
            pa_oCommand.m_oMonitorResponse.append("true");
          } else {
            pa_oCommand.m_oMonitorResponse.append("false");
          }
          pa_oCommand.m_oMonitorResponse.append("\">");
        } else if (pa_oCommand.m_oAdditionalParams.getValue()[0] == 't' && CMonitor::isForced(cDevice, rspResIter->first, rspFbIter->first, *rspPortIter)) {
          pa_oCommand.m_oMonitorResponse.append("<Port name=\"");
          pa_oCommand.m_oMonitorResponse.append(CStringDictionary::getInstance().get( *rspPortIter ));
          pa_oCommand.m_oMonitorResponse.append("\" forced=\"");
          pa_oCommand.m_oMonitorResponse.append("true");
          pa_oCommand.m_oMonitorResponse.append("\">");
        } else if (pa_oCommand.m_oAdditionalParams.getValue()[0] == 'f' && !CMonitor::isForced(cDevice, rspResIter->first, rspFbIter->first, *rspPortIter)) {
          pa_oCommand.m_oMonitorResponse.append("<Port name=\"");
          pa_oCommand.m_oMonitorResponse.append(CStringDictionary::getInstance().get( *rspPortIter ));
          pa_oCommand.m_oMonitorResponse.append("\" forced=\"");
          pa_oCommand.m_oMonitorResponse.append("false");
          pa_oCommand.m_oMonitorResponse.append("\">");
        }
        rspPortIter++;
        pa_oCommand.m_oMonitorResponse.append("</Port>");
      }
      rspFbIter++;
      pa_oCommand.m_oMonitorResponse.append("</FB>");
    }
    rspResIter++;
    pa_oCommand.m_oMonitorResponse.append("</Resource>");
  }
  CMonitor::freeDeviceMap();
  dataWatches = 0;
  return true;
}

bool CMonitor::getEventCount(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId eventNameId, unsigned int & retValue){
	CFunctionBlock *fb = getFbPointer(device, resourceNameId, fbNameId);
	if(fb == 0){
		return false;
	}

	// get Event count
	TEventID eventId = fb->getEIID(eventNameId);
	if(eventId != cg_nInvalidEventID){
    retValue = fb->m_nEIMonitorCount[eventId].eventCount[fb->m_nEIMonitorCount[eventId].bufPos];
		return true;
	}

	eventId = fb->getEOID(eventNameId);
	if(eventId != cg_nInvalidEventID){
    retValue = fb->m_nEOMonitorCount[eventId].eventCount[fb->m_nEOMonitorCount[eventId].bufPos];
		return true;
	}
	// possibly No Event found
	return false;
}

bool CMonitor::writeData(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId, const char * Value){

	CFunctionBlock *fb = getFbPointer(device, resourceNameId, fbNameId);
	if(fb == 0){
		return false;
	}
	// get DataInput
	unsigned int id = fb->getDIID(portNameId);
	CIEC_ANY *pData = fb->getDataInput(id);
	if(pData != 0){
		pData->fromString(Value);
		return true;
	}

	// get DataOutput
	id = fb->getDIID(portNameId);
	pData = fb->getDataOutput(id);
	if(pData != 0){
		pData->fromString(Value);
		return true;
	}
	return false;
}

bool CMonitor::startBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId){

	CFunctionBlock *fb = getFbPointer(device, resourceNameId, fbNameId);
	if(fb == 0){
		return false;
	}

	// start Event counting
	TEventID eventId = fb->getEIID(portNameId);
	if(eventId != cg_nInvalidEventID){
		fb->startEIBreakpoint(eventId);
		return true;
	}

	eventId = fb->getEOID(portNameId);
	if(eventId != cg_nInvalidEventID){
		fb->startEOBreakpoint(eventId);
		return true;
	}
	// possibly No Event found
	return false;
}
bool CMonitor::stopBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId){

	CFunctionBlock *fb = getFbPointer(device, resourceNameId, fbNameId);
	if(fb == 0){
		return false;
	}

	// start Event counting
	TEventID eventId = fb->getEIID(portNameId);
	if(eventId != cg_nInvalidEventID){
		fb->stopEIBreakpoint(eventId);
		return true;
	}

	eventId = fb->getEOID(portNameId);
	if(eventId != cg_nInvalidEventID){
		fb->stopEOBreakpoint(eventId);
		return true;
	}
	// possibly No Event found
	return false;
}

bool CMonitor::clearBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId){

	CFunctionBlock *fb = getFbPointer(device, resourceNameId, fbNameId);
	if(fb == 0){
		return false;
	}

	// clear Breakpoint
	TEventID eventId = fb->getEIID(portNameId);
	if(eventId != cg_nInvalidEventID){
		fb->clearEIBreakpoint(eventId);
		//TODO check why trigger an event on clearing a break point
		//triggerEvent(device, resourceNameId, fbNameId, portNameId);
		return true;
	}

	eventId = fb->getEOID(portNameId);
	if(eventId != cg_nInvalidEventID){
		fb->clearEOBreakpoint(eventId);
		//TODO check why trigger an event on clearing a break point
    //triggerEvent(device, resourceNameId, fbNameId, portNameId);
		return true;
	}
	// possibly No Event found
	return false;
}


bool CMonitor::addBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId){


	T_DEVICE_MAP::iterator resIter;
	T_RESOURCE_CONTAINER::iterator fbIter;
	T_FB_CONTAINER_ITER portIter;

	if(CMonitor::startBreakpoint(device, resourceNameId, fbNameId, portNameId) == false){
		return false;
	}

	{
	  CCriticalRegion criticalRegion(mLockDataWatches);

    // Test if Resource is in device, otherwise add it
    resIter = mDataWatches.find(resourceNameId);
    if(resIter == mDataWatches.end()){
      T_RESOURCE_CONTAINER newResCont;
      mDataWatches[resourceNameId] = newResCont;
      resIter = mDataWatches.find(resourceNameId);
    }

    // Test if FB is in Resource, otherwise add it
    fbIter = resIter->second.find(fbNameId);
    if(fbIter == resIter->second.end()){
      T_FB_CONTAINER newFbCont;
      resIter->second[fbNameId] = newFbCont;
      fbIter = resIter->second.find(fbNameId);
    }

    // Test if Port is in FB, otherwise add it
    portIter = std::find(fbIter->second.breakpointList.begin(), fbIter->second.breakpointList.end(),portNameId);
    if(portIter == fbIter->second.breakpointList.end()){
      fbIter->second.breakpointList.push_back(portNameId);
    }else{} // port already inserted
	} //End Critical region
	return true;
}
void CMonitor::removeBreakpoint(CStringDictionary::TStringId fbResId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId){


	T_DEVICE_MAP::iterator resIter;
	T_RESOURCE_CONTAINER::iterator fbIter;
	T_FB_CONTAINER_ITER portIter;

	CCriticalRegion criticalRegion(mLockDataWatches);

	// Test if Resource is in device, otherwise do nothing
	resIter = mDataWatches.find(fbResId);
	if(resIter != mDataWatches.end()){
		// Test if FB is in Resource, otherwise do nothing
		fbIter = resIter->second.find(fbNameId);
		if(fbIter != resIter->second.end()){
			// Test if Port is in FB, otherwise do nothing
			portIter = std::find(fbIter->second.breakpointList.begin(), fbIter->second.breakpointList.end(),portNameId);
			if(portIter != fbIter->second.breakpointList.end()){
				fbIter->second.breakpointList.erase(portIter);
			}

			// clean up unused maps/lists
			if(fbIter->second.addWatchList.size() == 0 && fbIter->second.breakpointList.size() == 0){
				resIter->second.erase(fbIter);
			}
		}
		// clean up unused maps/lists
		//if(resIter->second.size() == 0){
		//	mDataWatches.erase(resIter);
		//}
	}
}

bool CMonitor::GetBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId eventNameId, bool & enabled, int & set){
	// Read Data from FB

	CFunctionBlock *fb = getFbPointer(device, resourceNameId, fbNameId);
	if(fb == 0){
		return false;
	}

	// check events 
	TEventID eventId = fb->getEIID(eventNameId);
	if(eventId != cg_nInvalidEventID){
		return fb->getEIBreakpoint(eventId, enabled, set);
//		return true;
	}
	eventId = fb->getEOID(eventNameId);
	if(eventId != cg_nInvalidEventID){
		return fb->getEOBreakpoint(eventId, enabled, set);
		//return true;
	}
	return false;
}

