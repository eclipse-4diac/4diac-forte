/*******************************************************************************
 * Copyright (c) 2012 - 2013 Profactor GmbH, ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Michael Hofmann, Gerhard Ebenhofer, Alois Zoitl, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef MONITOR_H_
#define MONITOR_H_

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <string>
#include <list>
#include <map>
#include <algorithm>

#include "resource.h"
#include "stringdict.h"
#include <forte_sync.h>

typedef struct{
  std::string resource;
  std::string fbName;
  std::string portName;
} T_DATAWATCH;

typedef CStringDictionary::TStringId T_PORT_ID;
typedef struct{
  std::list <T_PORT_ID>  addWatchList;
  std::list <T_PORT_ID>  breakpointList;
} T_FB_CONTAINER;

typedef std::map<const CStringDictionary::TStringId, T_FB_CONTAINER> T_RESOURCE_CONTAINER;
typedef std::map<const CStringDictionary::TStringId, T_RESOURCE_CONTAINER> T_DEVICE_MAP;
typedef T_DEVICE_MAP::iterator T_DEVICE_MAP_ITER;
typedef T_RESOURCE_CONTAINER::iterator T_RESOURCE_CONTAINER_ITER;
typedef std::list <T_PORT_ID> ::iterator T_FB_CONTAINER_ITER;


class CMonitor{
public:
  static T_DEVICE_MAP mDataWatches;
  // Get Iterators
  static T_DEVICE_MAP * getDeviceMap(){
    mLockDataWatches.lock();
    return &mDataWatches;
  }

  // this function is for locks
  static void freeDeviceMap(){
    mLockDataWatches.unlock();
  }

  static EMGMResponse executeMonitoringCommand(CDevice *pa_poDevice, SManagementCMD &pa_rstCommand);

  // Event Count
  /*! \brief start counting of fired events
   *
   *   Start the eventcounting of a specific port of a functionblock.
   *   \param device pointer to device
   *   \param resourceNameId StringID of the name of the resource in which the FB is located
   *   \param fbNameId StringID of the name of the functionblock
   *   \param eventID StringID of the name of the event
   *   \return returns true when the counter was successfully started
   */
  static bool startEventCount(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId eventID);
  /*! \brief stop counting of fired events
   *
   *   Stop the eventcounting of a specific port of a functionblock.
   *   \param device pointer to device
   *   \param resourceName name of the resource in which the FB is located
   *   \param fbID StringID of the name of the functionblock
   *   \param eventID StringID of the name of the event
   *   \return returns true when the counter was successfully stopped
   */
  static bool stopEventCount(CDevice *device, const char *resourceName, CStringDictionary::TStringId fbID, CStringDictionary::TStringId eventID);
  /*! \brief get counted events from eventcounter
   *
   *   Returns the current status of the eventcounter.
   *   \param device pointer to device
   *   \param resourceNameId StringID of the name of the resource in which the FB is located
   *   \param fbNameId StringID of the name of the functionblock
   *   \param eventID StringID of the name of the event
   *   \param retValue reference to returnvalue, counted events
   *   \return returns true when the counter was successfully read
   */
  static bool getEventCount(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId eventID, unsigned int & retValue);

  // Get Data
  /*! \brief get the value from a dataport of a functionblock
   *
   *   Reads the current value of a dataport.
   *   \param device pointer to device
   *   \param resourceNameId StringID of the name of the resource in which the FB is located
   *   \param fbNameId StringID of the name of the functionblock
   *   \param portNameId StringID of the name of the event
   *   \param pa_acValue pointer to char-Array for return-value
   *   \param pa_nBufferSize size of the provided buffer
   *   \param forced reference for returning the forced-flag
   *   \return returns true when the counter was successfully read
   */
  static bool GetData(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId, char* pa_acValue, unsigned int pa_nBufferSize, bool & forced);

  // Write Data
  /*! \brief overwrite a value to a dataport
   *
   *   Simply overwrites the current value of a port. this feature is only reasonable for output ports. Inputport are overwritten
   *   with the new value, so the written value is instantely overwritten.
   *   \param device pointer to device
   *   \param resourceNameId StringID of the name of the resource in which the FB is located
   *   \param fbNameId StringID of the name of the functionblock
   *   \param portNameId StringID of the name of the event
   *   \param Value value which will be written
   *   \return returns true when the counter was successfully read
   */
  static bool writeData(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId, const char * Value);



  static bool startBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId eventID);
  static bool stopBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId eventID);
  static bool clearBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId eventID);
  static bool GetBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId eventID, bool & enabled, int & set);
  static bool addBreakpoint(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId);

  static bool readWatches(CDevice *cDevice, CIEC_STRING &pa_roResponse);
  static bool getWatches(CDevice *cDevice, SManagementCMD &pa_oCommand);
  /*! \brief internal function, get a pointer from the real functionblock
   *
   *   This function helps getting a grip of the functionblock
   *   which will be modified.
   *   \param device pointer to device
   *   \param resourceNameId StringID of the name of the resource in which the FB is located
   *   \param fbNameId StringID of thee name of the functionblock
   *   \return pointer to the functionblock
   */
  static CFunctionBlock * getFbPointer(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId);
  static bool wasTriggered(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId);
  static bool isForced(CDevice *device, CStringDictionary::TStringId resourceNameId, CStringDictionary::TStringId fbNameId, CStringDictionary::TStringId portNameId);


protected:

  static void removeBreakpoint(CStringDictionary::TStringId resourceID, CStringDictionary::TStringId fbID, CStringDictionary::TStringId portNameId);


  static CResource * getResPointer(CDevice *device, CStringDictionary::TStringId resourceID);

  static CSyncObject mLockDataWatches;
  static TForteInt16 watchResponseSize;
  static SEventEntry m_stTriggerEvent;
};

#endif /* MONITOR_H_ */
