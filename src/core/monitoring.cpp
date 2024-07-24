
/*******************************************************************************
 * Copyright (c) 2015, 2018, 2022 fortiss GmbH, Johannes Kepler University
 *  Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *  Martin Melik Merkumians
 *    - adds functionality for W/CHAR
 *******************************************************************************/
#include <forte_config.h>
#include "monitoring.h"
#include "resource.h"
#include "device.h"
#include "ecet.h"
#include "utils/criticalregion.h"
#include "utils/string_utils.h"

using namespace std::string_literals;
using namespace forte::core;

const std::string cgClosingXMLTag = "\">"s;


CMonitoringHandler::CMonitoringHandler(CResource &paResource) :
    mTriggerEvent(nullptr, 0),
        mResource(paResource){
}

EMGMResponse CMonitoringHandler::executeMonitoringCommand(SManagementCMD &paCommand){
  EMGMResponse retVal = EMGMResponse::UnsupportedCmd;

  switch (paCommand.mCMD){
    case EMGMCommandType::MonitoringAddWatch:
      retVal = addWatch(paCommand.mFirstParam);
      break;
    case EMGMCommandType::MonitoringRemoveWatch:
      retVal = removeWatch(paCommand.mFirstParam);
      break;
    case EMGMCommandType::MonitoringReadWatches:
      retVal = readWatches(paCommand.mMonitorResponse);
      break;
    case EMGMCommandType::MonitoringForce:
      retVal = mResource.writeValue(paCommand.mFirstParam, paCommand.mAdditionalParams, true);
      break;
    case EMGMCommandType::MonitoringClearForce:
      retVal = clearForce(paCommand.mFirstParam);
      break;
    case EMGMCommandType::MonitoringTriggerEvent:
      retVal = triggerEvent(paCommand.mFirstParam);
      break;
    case EMGMCommandType::MonitoringResetEventCount:
      retVal = resetEventCount(paCommand.mFirstParam);
      break;
    default:
      break;
  }
  return retVal;
}

CFunctionBlock* CMonitoringHandler::getFB(forte::core::TNameIdentifier &paNameList){
  forte::core::TNameIdentifier::CIterator runner(paNameList.begin());
  return mResource.getFB(runner);
}

EMGMResponse CMonitoringHandler::addWatch(forte::core::TNameIdentifier &paNameList){
  EMGMResponse eRetVal = EMGMResponse::NoSuchObject;

  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.popBack();
  CFunctionBlock *fB = getFB(paNameList);

  if(nullptr != fB){
    SFBMonitoringEntry &fbMonitoringEntry(findOrCreateFBMonitoringEntry(fB, paNameList));

    CIEC_ANY *dataVal = fB->getVar(&portName, 1);
    if(nullptr != dataVal){
      addDataWatch(fbMonitoringEntry, portName, *dataVal);
      eRetVal = EMGMResponse::Ready;
    }
    else{
      TEventID eventId = fB->getEIID(portName);
      if(cgInvalidEventID != eventId){
        addEventWatch(fbMonitoringEntry, portName, fB->getEIMonitorData(eventId));
        eRetVal = EMGMResponse::Ready;
      }
      else{
        eventId = fB->getEOID(portName);
        if(cgInvalidEventID != eventId){
          addEventWatch(fbMonitoringEntry, portName, fB->getEOMonitorData(eventId));
          eRetVal = EMGMResponse::Ready;
        }
      }
    }
  }

  return eRetVal;
}

EMGMResponse CMonitoringHandler::removeWatch(forte::core::TNameIdentifier &paNameList){
  EMGMResponse eRetVal = EMGMResponse::NoSuchObject;

  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.popBack();
  CFunctionBlock *fB = getFB(paNameList);
  if(nullptr != fB){
    TFBMonitoringList::Iterator itRefNode = mFBMonitoringList.end(); //needed for deleting the entry from the list
    TFBMonitoringList::Iterator itRunner = mFBMonitoringList.begin();

    while(itRunner != mFBMonitoringList.end()){
      if(itRunner->mFB == fB){
        SFBMonitoringEntry &poFBMonitoringEntry(*itRunner);

        if(removeDataWatch(poFBMonitoringEntry, portName) || removeEventWatch(poFBMonitoringEntry, portName)){ //if element is not watched, end search and return error

          if(poFBMonitoringEntry.mWatchedDataPoints.isEmpty() && (poFBMonitoringEntry.mWatchedEventPoints.isEmpty())){
            //no further values are monitored so remove the entry
            if(itRefNode == mFBMonitoringList.end()){
              //we have the first entry in the list
              mFBMonitoringList.popFront();
            }else{
              mFBMonitoringList.eraseAfter(itRefNode);
            }
          }
          eRetVal = EMGMResponse::Ready;
        }
        break;
      }

      itRefNode = itRunner;
      ++itRunner;
    }
  }
  return eRetVal;
}

EMGMResponse CMonitoringHandler::readWatches(std::string &paResponse){
  paResponse.clear();
  if(&mResource == &mResource.getParent()){
    //we are in the device
    for(CFBContainer::TFBContainerList::iterator itRunner = mResource.getChildren().begin();
        itRunner != mResource.getChildren().end();
        ++itRunner){
      static_cast<CResource*>(*itRunner)->getMonitoringHandler().readResourceWatches(paResponse);
    }
  }
  else{
    //we are within a resource
    readResourceWatches(paResponse);
  }

  return EMGMResponse::Ready;
}

EMGMResponse CMonitoringHandler::clearForce(forte::core::TNameIdentifier &paNameList){
  EMGMResponse eRetVal = EMGMResponse::NoSuchObject;
  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.popBack();
  CFunctionBlock *fB = getFB(paNameList);

  if(nullptr != fB){
    CIEC_ANY *poDataVal = fB->getVar(&portName, 1);
    if(nullptr != poDataVal){
      poDataVal->setForced(false);
      eRetVal = EMGMResponse::Ready;
    }
  }
  return eRetVal;
}

EMGMResponse CMonitoringHandler::triggerEvent(forte::core::TNameIdentifier &paNameList){
  EMGMResponse eRetVal = EMGMResponse::NoSuchObject;
  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.popBack();
  CFunctionBlock *fB = getFB(paNameList);
  if(nullptr != fB){
    TEventID eventId = fB->getEIID(portName);
    if(cgInvalidEventID != eventId){
      //only a single event can be triggered simultaneously (until it is executed by ecet!)
      //otherwise the triggerEvent structure needs to be moved to another place!
      mTriggerEvent.mFB = fB;
      mTriggerEvent.mPortId = eventId;
      mResource.getResourceEventExecution()->startEventChain(mTriggerEvent);
      eRetVal = EMGMResponse::Ready;
    }
    else{
      eventId = fB->getEOID(portName);
      if(cgInvalidEventID != eventId){
        fB->sendOutputEvent(eventId, mResource.getResourceEventExecution());
        mResource.getResourceEventExecution()->resumeSelfSuspend();
        eRetVal = EMGMResponse::Ready;
      }
    }

  }
  return eRetVal;
}

EMGMResponse CMonitoringHandler::resetEventCount(forte::core::TNameIdentifier &paNameList){
  EMGMResponse eRetVal = EMGMResponse::NoSuchObject;
  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.popBack();
  CFunctionBlock *fB = getFB(paNameList);
  if(nullptr != fB){
    TEventID eventId = fB->getEIID(portName);
    TForteUInt32 *eventMonitorData = nullptr;

    if(cgInvalidEventID != eventId){
      eventMonitorData = &fB->getEIMonitorData(eventId);
    }
    else{
      eventId = fB->getEOID(portName);
      if(cgInvalidEventID != eventId){
        eventMonitorData = &fB->getEOMonitorData(eventId);
      }
    }
    if(nullptr != eventMonitorData){
      *eventMonitorData = 0;
      eRetVal = EMGMResponse::Ready;
    }
  }
  return eRetVal;
}

CMonitoringHandler::SFBMonitoringEntry &CMonitoringHandler::findOrCreateFBMonitoringEntry(
    CFunctionBlock *paFB, forte::core::TNameIdentifier &paNameList){
  for(TFBMonitoringList::Iterator itRunner = mFBMonitoringList.begin();
      itRunner != mFBMonitoringList.end(); ++itRunner){
    if(itRunner->mFB == paFB){
      return *itRunner;
    }
  }

  mFBMonitoringList.pushBack(SFBMonitoringEntry());
  TFBMonitoringList::Iterator itLastEntry(mFBMonitoringList.back());
  itLastEntry->mFB = paFB;
  createFullFBName(itLastEntry->mFullFBName, paNameList);
  return *itLastEntry;
}

void CMonitoringHandler::addDataWatch(SFBMonitoringEntry &paFBMonitoringEntry,
    CStringDictionary::TStringId paPortId, CIEC_ANY &paDataVal){
  for(TDataWatchList::Iterator itRunner = paFBMonitoringEntry.mWatchedDataPoints.begin();
      itRunner != paFBMonitoringEntry.mWatchedDataPoints.end(); ++itRunner){
    if(itRunner->mPortId == paPortId){
      //the data point is already in the watch list
      return;
    }
  }
  paFBMonitoringEntry.mWatchedDataPoints.pushBack(SDataWatchEntry(paPortId, paDataVal));
}

bool CMonitoringHandler::removeDataWatch(SFBMonitoringEntry &paFBMonitoringEntry,
    CStringDictionary::TStringId paPortId){
  bool bRetVal = false;

  TDataWatchList::Iterator itRunner = paFBMonitoringEntry.mWatchedDataPoints.begin();
  TDataWatchList::Iterator itRefNode = paFBMonitoringEntry.mWatchedDataPoints.end();

  while(itRunner != paFBMonitoringEntry.mWatchedDataPoints.end()){
    if(itRunner->mPortId == paPortId){
      if(itRefNode == paFBMonitoringEntry.mWatchedDataPoints.end()){
        //we have the first entry in the list
        paFBMonitoringEntry.mWatchedDataPoints.popFront();
      }
      else{
        paFBMonitoringEntry.mWatchedDataPoints.eraseAfter(itRefNode);
      }
      bRetVal = true;
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }

  return bRetVal;
}

void CMonitoringHandler::addEventWatch(SFBMonitoringEntry &paFBMonitoringEntry, CStringDictionary::TStringId paPortId, TForteUInt32 &paEventData){
  for(TEventWatchList::Iterator itRunner = paFBMonitoringEntry.mWatchedEventPoints.begin();
      itRunner != paFBMonitoringEntry.mWatchedEventPoints.end(); ++itRunner){
    if(itRunner->mPortId == paPortId){
      //the event point is already in the watch list
      return;
    }
  }
  paFBMonitoringEntry.mWatchedEventPoints.pushBack(SEventWatchEntry(paPortId, paEventData));
}

bool CMonitoringHandler::removeEventWatch(SFBMonitoringEntry &paFBMonitoringEntry, CStringDictionary::TStringId paPortId){
  bool bRetVal = false;

  TEventWatchList::Iterator itRunner = paFBMonitoringEntry.mWatchedEventPoints.begin();
  TEventWatchList::Iterator itRefNode = paFBMonitoringEntry.mWatchedEventPoints.end();

  while(itRunner != paFBMonitoringEntry.mWatchedEventPoints.end()){
    if(itRunner->mPortId == paPortId){
      if(itRefNode == paFBMonitoringEntry.mWatchedEventPoints.end()){
        //we have the first entry in the list
        paFBMonitoringEntry.mWatchedEventPoints.popFront();
      }
      else{
        paFBMonitoringEntry.mWatchedEventPoints.eraseAfter(itRefNode);
      }
      bRetVal = true;
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }

  return bRetVal;
}

void CMonitoringHandler::readResourceWatches(std::string &paResponse){
  if(!mFBMonitoringList.isEmpty()){
    paResponse += "<Resource name=\""s;
    paResponse += mResource.getInstanceName();
    paResponse += cgClosingXMLTag;

    updateMonitringData();

    for(TFBMonitoringList::Iterator itRunner = mFBMonitoringList.begin(); itRunner != mFBMonitoringList.end(); ++itRunner){
      paResponse += "<FB name=\""s;
      paResponse += itRunner->mFullFBName.c_str();
      paResponse += cgClosingXMLTag;

      //add the data watches
      for(TDataWatchList::Iterator itDataRunner = itRunner->mWatchedDataPoints.begin(); itDataRunner != itRunner->mWatchedDataPoints.end(); ++itDataRunner){
        appendDataWatch(paResponse, *itDataRunner);
      }

      //add the event watches
      for(TEventWatchList::Iterator itEventRunner = itRunner->mWatchedEventPoints.begin(); itEventRunner != itRunner->mWatchedEventPoints.end(); ++itEventRunner){
        appendEventWatch(paResponse, *itEventRunner);
      }

      paResponse += "</FB>"s;
    }
    paResponse += "</Resource>"s;
  }
}


void CMonitoringHandler::updateMonitringData(){
  for(TFBMonitoringList::Iterator itRunner = mFBMonitoringList.begin(); itRunner != mFBMonitoringList.end(); ++itRunner){
    for(TDataWatchList::Iterator itDataRunner = itRunner->mWatchedDataPoints.begin(); itDataRunner != itRunner->mWatchedDataPoints.end(); ++itDataRunner){
      itDataRunner->mDataBuffer->setValue(itDataRunner->mDataValueRef);
      itDataRunner->mDataBuffer->setForced(itDataRunner->mDataValueRef.isForced());
    }
    for(TEventWatchList::Iterator itEventRunner = itRunner->mWatchedEventPoints.begin(); itEventRunner != itRunner->mWatchedEventPoints.end(); ++itEventRunner){
      itEventRunner->mEventDataBuf = itEventRunner->mEventDataRef;
    }
  }
}


void CMonitoringHandler::appendDataWatch(std::string &paResponse, SDataWatchEntry &paDataWatchEntry){
  appendPortTag(paResponse, paDataWatchEntry.mPortId);
  paResponse += "<Data value=\""s;
  size_t bufferSize = paDataWatchEntry.mDataBuffer->getToStringBufferSize() + getExtraSizeForEscapedChars(*paDataWatchEntry.mDataBuffer);
  char* acDataValue = new char [bufferSize];
  int consumedBytes = paDataWatchEntry.mDataBuffer->toString(acDataValue, bufferSize);
  if(consumedBytes > 0 && static_cast<size_t>(consumedBytes) < bufferSize) {
    switch (paDataWatchEntry.mDataBuffer->getDataTypeID()) {
      case CIEC_ANY::e_ANY:
      case CIEC_ANY::e_WSTRING:
      case CIEC_ANY::e_STRING:
      case CIEC_ANY::e_CHAR:
      case CIEC_ANY::e_WCHAR:
      case CIEC_ANY::e_ARRAY:
      case CIEC_ANY::e_STRUCT:
        consumedBytes += static_cast<int>(forte::core::util::transformNonEscapedToEscapedXMLText(acDataValue));
        break;
      default:
        break;
    }
    acDataValue[consumedBytes] = '\0';
    paResponse += acDataValue;
  }
  paResponse += "\" forced=\""s;
  paResponse += (paDataWatchEntry.mDataBuffer->isForced()) ? "true"s : "false"s;
  paResponse += "\"/></Port>"s;
  delete [] acDataValue;
}

size_t CMonitoringHandler::getExtraSizeForEscapedChars(const CIEC_ANY& paDataValue){
  size_t retVal = 0;

  switch(paDataValue.getDataTypeID()){
    case CIEC_ANY::e_ANY:
      retVal = getExtraSizeForEscapedChars(paDataValue.unwrap());
      break;
    case CIEC_ANY::e_STRING:
      retVal = forte::core::util::getExtraSizeForXMLEscapedChars(static_cast<const CIEC_STRING &>(paDataValue).getStorage().c_str()) + 10; // for opening and closing quotes or apos
      break;
    case CIEC_ANY::e_WSTRING:
      retVal = forte::core::util::getExtraSizeForXMLEscapedChars(static_cast<const CIEC_WSTRING&>(paDataValue).getValue()) + 10; //for opening and closing quotes or apos
      break;
    case CIEC_ANY::e_CHAR:
      retVal = 5 + 5 + 5; // Both outer quotes and symbol gets evetually replaced
      break;
    case CIEC_ANY::e_WCHAR:
      retVal = 5 + 5 + 5; //Both outer quotes and symbol gets evetually replaced
    break;
    case CIEC_ANY::e_ARRAY:
      retVal = getExtraSizeForEscapedCharsArray(static_cast<const CIEC_ARRAY &>(paDataValue));
      break;
    case CIEC_ANY::e_STRUCT:
      retVal = getExtraSizeForEscapedCharsStruct(static_cast<const CIEC_STRUCT&>(paDataValue));
      break;
    default:
      break;
  }

  return retVal;
}

size_t CMonitoringHandler::getExtraSizeForEscapedCharsArray(const CIEC_ARRAY &paDataValue) {
  size_t retVal = 0;
  auto lowerBound = paDataValue.getLowerBound();
  auto upperBound = paDataValue.getUpperBound();
  switch(paDataValue[lowerBound].getDataTypeID()){
    case CIEC_ANY::e_STRING:
      for (auto i = lowerBound; i <= upperBound; i++) {
        retVal += forte::core::util::getExtraSizeForXMLEscapedChars(static_cast<const CIEC_STRING&>(paDataValue[static_cast<TForteUInt16>(i)]).getStorage().c_str()) + 10; //for opening and closing quotes or apos
      }
      break;
    case CIEC_ANY::e_WSTRING:
      for (auto i = lowerBound; i <= upperBound; i++) {
        retVal += forte::core::util::getExtraSizeForXMLEscapedChars(static_cast<const CIEC_WSTRING&>(paDataValue[static_cast<TForteUInt16>(i)]).getValue()) + 10; //for opening and closing quotes or apos
      }
      break;
    case CIEC_ANY::e_STRUCT:
      for (auto i = lowerBound; i <= upperBound; i++) {
        retVal += getExtraSizeForEscapedCharsStruct(static_cast<const CIEC_STRUCT&>(paDataValue[static_cast<TForteUInt16>(i)]));
      }
      break;
    default:
      break;
  }

  return retVal;
}

size_t CMonitoringHandler::getExtraSizeForEscapedCharsStruct(const CIEC_STRUCT& paDataValue) {
  size_t retVal = 0;

  for(size_t i = 0; i < paDataValue.getStructSize(); i++) {
    const CIEC_ANY *member = paDataValue.getMember(i);
    switch(member->getDataTypeID()){
      case CIEC_ANY::e_STRING:
        retVal += forte::core::util::getExtraSizeForXMLEscapedChars(static_cast<const CIEC_STRING *>(member)->getStorage().c_str()) + 10; // for opening and closing quotes or apos
        break;
      case CIEC_ANY::e_WSTRING:
        retVal += forte::core::util::getExtraSizeForXMLEscapedChars(static_cast<const CIEC_WSTRING*>(member)->getValue()) + 10; //for opening and closing quotes or apos
        break;
      case CIEC_ANY::e_ARRAY:
        retVal += getExtraSizeForEscapedCharsArray(*static_cast<const CIEC_ARRAY *>(member));
        break;
      case CIEC_ANY::e_STRUCT:
        retVal += getExtraSizeForEscapedCharsStruct(*static_cast<const CIEC_STRUCT *>(member));
        break;
      default:
        break;
    }
  }

  return retVal;
}

void CMonitoringHandler::appendPortTag(std::string &paResponse,
    CStringDictionary::TStringId paPortId){
  paResponse += "<Port name=\""s;
  paResponse += CStringDictionary::getInstance().get(paPortId);
  paResponse += cgClosingXMLTag;
}

void CMonitoringHandler::appendEventWatch(std::string &paResponse, SEventWatchEntry &paEventWatchEntry){
  appendPortTag(paResponse, paEventWatchEntry.mPortId);

  CIEC_UDINT udint(paEventWatchEntry.mEventDataBuf);
  CIEC_ULINT ulint(mResource.getDevice()->getTimer().getForteTime());

  paResponse += "<Data value=\""s;
  char buf[21]; // the bigest number in an ulint is 18446744073709551616, TODO directly use paResponse
  udint.toString(buf, sizeof(buf));
  paResponse += buf;
  paResponse += "\" time=\""s;
  ulint.toString(buf, sizeof(buf));
  paResponse += buf;
  paResponse += "\"/>\n</Port>"s;
}

void CMonitoringHandler::createFullFBName(std::string &paFullName, forte::core::TNameIdentifier &paNameList){
  for(forte::core::TNameIdentifier::CIterator runner(paNameList.begin()); runner != paNameList.end(); ++runner){
    paFullName.append(CStringDictionary::getInstance().get(*runner));
    if(!runner.isLastEntry()){
      paFullName.append(".");
    }
  }
}

