
/*******************************************************************************
 * Copyright (c) 2015, 2025 fortiss GmbH, Johannes Kepler University
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - adds functionality for W/CHAR
 *******************************************************************************/
#include <algorithm>
#include <forte_config.h>
#include "monitoring.h"
#include "funcbloc.h"
#include "resource.h"
#include "device.h"
#include "ecet.h"
#include "core/util/string_utils.h"
#include "../arch/timerha.h"
#include "datatypes/forte_array.h"
#include "datatypes/forte_struct.h"

using namespace std::string_literals;
using namespace forte::core;
using namespace forte::core::internal;

const std::string cgClosingXMLTag = "\">"s;

namespace {
  constexpr auto watchEntryComparator = [](const CWatchEntry &paItem, CStringDictionary::TStringId paPortId) {
    return paItem.getPortId() < paPortId;
  };

  constexpr auto monitoringEntryComparator = [](const SFBMonitoringEntry &paItem, const CFunctionBlock *const paFB) {
    return &paItem.getFB() < paFB;
  };

} // namespace

void CDataWatchEntry::update(const CFunctionBlock &paFB) {
  mDataBuffer->setValue(mDataValueRef);
  mForced = paFB.getForce(mForceIndex);
}

void CEventWatchEntry::update() {
  mEventDataBuf = mEventDataRef;
}

CMonitoringHandler::CMonitoringHandler(CResource &paResource) : mResource(paResource) {
}

EMGMResponse CMonitoringHandler::executeMonitoringCommand(SManagementCMD &paCommand) {
  EMGMResponse retVal = EMGMResponse::UnsupportedCmd;

  switch (paCommand.mCMD) {
    case EMGMCommandType::MonitoringAddWatch: retVal = addWatch(paCommand.mFirstParam); break;
    case EMGMCommandType::MonitoringRemoveWatch: retVal = removeWatch(paCommand.mFirstParam); break;
    case EMGMCommandType::MonitoringReadWatches: retVal = readWatches(paCommand.mMonitorResponse); break;
    case EMGMCommandType::MonitoringForce:
      retVal = mResource.writeValue(paCommand.mFirstParam, paCommand.mAdditionalParams, true);
      break;
    case EMGMCommandType::MonitoringClearForce: retVal = clearForce(paCommand.mFirstParam); break;
    case EMGMCommandType::MonitoringTriggerEvent: retVal = triggerEvent(paCommand.mFirstParam); break;
    case EMGMCommandType::MonitoringResetEventCount: retVal = resetEventCount(paCommand.mFirstParam); break;
    default: break;
  }
  return retVal;
}

CFunctionBlock *CMonitoringHandler::getFB(forte::core::TNameIdentifier &paNameList) {
  auto runner = paNameList.cbegin();
  return mResource.getFB(runner, paNameList.cend());
}

EMGMResponse CMonitoringHandler::addWatch(forte::core::TNameIdentifier &paNameList) {
  EMGMResponse eRetVal = EMGMResponse::NoSuchObject;

  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.pop_back();
  CFunctionBlock *fB = getFB(paNameList);

  if (nullptr != fB) {
    SFBMonitoringEntry &fbMonitoringEntry(findOrCreateFBMonitoringEntry(fB, paNameList));

    CIEC_ANY *dataVal = fB->getVar(&portName, 1);
    if (nullptr != dataVal) {
      addDataWatch(fbMonitoringEntry, portName, *dataVal);
      eRetVal = EMGMResponse::Ready;
    } else {
      TEventID eventId = fB->getFBInterfaceSpec().getEIID(portName);
      if (cgInvalidEventID != eventId) {
        addEventWatch(fbMonitoringEntry, portName, fB->getEIMonitorData(eventId));
        eRetVal = EMGMResponse::Ready;
      } else {
        eventId = fB->getFBInterfaceSpec().getEOID(portName);
        if (cgInvalidEventID != eventId) {
          addEventWatch(fbMonitoringEntry, portName, fB->getEOMonitorData(eventId));
          eRetVal = EMGMResponse::Ready;
        }
      }
    }
  }

  return eRetVal;
}

EMGMResponse CMonitoringHandler::removeWatch(forte::core::TNameIdentifier &paNameList) {
  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.pop_back();
  CFunctionBlock *fB = getFB(paNameList);
  if (nullptr != fB) {
    auto it = std::lower_bound(mFBMonitoringList.begin(), mFBMonitoringList.end(), fB, monitoringEntryComparator);

    if (it != mFBMonitoringList.end() && &it->getFB() == fB) {
      SFBMonitoringEntry &monitoringEntry = *it;
      if (removeDataWatch(monitoringEntry, portName) ||
          removeEventWatch(monitoringEntry, portName)) { // if element is not watched, end search and return error

        if (monitoringEntry.mWatchedDataPoints.empty() && (monitoringEntry.mWatchedEventPoints.empty())) {
          mFBMonitoringList.erase(it);
        }
        return EMGMResponse::Ready;
      }
    }
  }
  return EMGMResponse::NoSuchObject;
}

EMGMResponse CMonitoringHandler::readWatches(std::string &paResponse) {
  paResponse.clear();
  if (&mResource == &mResource.getParent()) {
    // we are in the device
    for (CFBContainer::TFBContainerList::iterator itRunner = mResource.getChildren().begin();
         itRunner != mResource.getChildren().end(); ++itRunner) {
      static_cast<CResource *>(*itRunner)->getMonitoringHandler().readResourceWatches(paResponse);
    }
  } else {
    // we are within a resource
    readResourceWatches(paResponse);
  }

  return EMGMResponse::Ready;
}

EMGMResponse CMonitoringHandler::clearForce(forte::core::TNameIdentifier &paNameList) {
  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.pop_back();
  CFunctionBlock *fB = getFB(paNameList);

  if (fB == nullptr) {
    return EMGMResponse::NoSuchObject;
  }

  auto absDataPortId = fB->getAbsDataPortNum(portName);
  if (absDataPortId == INVALID_ABS_DATA_PORT_ID) {
    return EMGMResponse::NoSuchObject;
  }

  fB->setForce(absDataPortId, false);
  return EMGMResponse::Ready;
}

EMGMResponse CMonitoringHandler::triggerEvent(forte::core::TNameIdentifier &paNameList) {
  EMGMResponse eRetVal = EMGMResponse::NoSuchObject;
  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.pop_back();
  CFunctionBlock *fB = getFB(paNameList);
  if (nullptr != fB) {
    TEventID eventId = fB->getFBInterfaceSpec().getEIID(portName);
    if (cgInvalidEventID != eventId) {
      // only a single event can be triggered simultaneously (until it is executed by ecet!)
      // otherwise the triggerEvent structure needs to be moved to another place!
      mResource.getResourceEventExecution()->startEventChain(TEventEntry(*fB, eventId));
      eRetVal = EMGMResponse::Ready;
    } else {
      eventId = fB->getFBInterfaceSpec().getEOID(portName);
      if (cgInvalidEventID != eventId) {
        fB->sendOutputEvent(eventId, mResource.getResourceEventExecution());
        mResource.getResourceEventExecution()->resumeSelfSuspend();
        eRetVal = EMGMResponse::Ready;
      }
    }
  }
  return eRetVal;
}

EMGMResponse CMonitoringHandler::resetEventCount(forte::core::TNameIdentifier &paNameList) {
  EMGMResponse eRetVal = EMGMResponse::NoSuchObject;
  CStringDictionary::TStringId portName = paNameList.back();
  paNameList.pop_back();
  CFunctionBlock *fB = getFB(paNameList);
  if (nullptr != fB) {
    TEventID eventId = fB->getFBInterfaceSpec().getEIID(portName);
    TForteUInt32 *eventMonitorData = nullptr;

    if (cgInvalidEventID != eventId) {
      eventMonitorData = &fB->getEIMonitorData(eventId);
    } else {
      eventId = fB->getFBInterfaceSpec().getEOID(portName);
      if (cgInvalidEventID != eventId) {
        eventMonitorData = &fB->getEOMonitorData(eventId);
      }
    }
    if (nullptr != eventMonitorData) {
      *eventMonitorData = 0;
      eRetVal = EMGMResponse::Ready;
    }
  }
  return eRetVal;
}

SFBMonitoringEntry &CMonitoringHandler::findOrCreateFBMonitoringEntry(CFunctionBlock *paFB,
                                                                      forte::core::TNameIdentifier &paNameList) {
  auto it = std::lower_bound(mFBMonitoringList.begin(), mFBMonitoringList.end(), paFB, monitoringEntryComparator);

  if (it != mFBMonitoringList.end() && &it->getFB() == paFB) {
    // the data point is already in the watch list
    return *it;
  }

  std::string fullFBName;
  createFullFBName(fullFBName, paNameList);
  return *mFBMonitoringList.emplace(it, std::move(fullFBName), paFB);
}

void CMonitoringHandler::addDataWatch(SFBMonitoringEntry &paFBMonitoringEntry,
                                      CStringDictionary::TStringId paPortId,
                                      CIEC_ANY &paDataVal) {
  auto &dataWatches = paFBMonitoringEntry.mWatchedDataPoints;
  auto it = std::lower_bound(dataWatches.begin(), dataWatches.end(), paPortId, watchEntryComparator);

  if (it != dataWatches.end() && it->getPortId() == paPortId) {
    // the data point is already in the watch list
    return;
  }

  dataWatches.emplace(it, paPortId, paDataVal, paFBMonitoringEntry.getFB().getAbsDataPortNum(paPortId));
}

bool CMonitoringHandler::removeDataWatch(SFBMonitoringEntry &paFBMonitoringEntry,
                                         CStringDictionary::TStringId paPortId) {
  auto &dataWatches = paFBMonitoringEntry.mWatchedDataPoints;
  auto it = std::lower_bound(dataWatches.begin(), dataWatches.end(), paPortId, watchEntryComparator);

  if (it != dataWatches.end() && it->getPortId() == paPortId) {
    dataWatches.erase(it);
    return true;
  }
  return false;
}

void CMonitoringHandler::addEventWatch(SFBMonitoringEntry &paFBMonitoringEntry,
                                       CStringDictionary::TStringId paPortId,
                                       TForteUInt32 &paEventData) {
  auto &eventWatches = paFBMonitoringEntry.mWatchedEventPoints;
  auto it = std::lower_bound(eventWatches.begin(), eventWatches.end(), paPortId, watchEntryComparator);

  if (it != eventWatches.end() && it->getPortId() == paPortId) {
    // the data point is already in the watch list
    return;
  }

  eventWatches.emplace(it, paPortId, paEventData);
}

bool CMonitoringHandler::removeEventWatch(SFBMonitoringEntry &paFBMonitoringEntry,
                                          CStringDictionary::TStringId paPortId) {
  auto &eventWatches = paFBMonitoringEntry.mWatchedEventPoints;
  auto it = std::lower_bound(eventWatches.begin(), eventWatches.end(), paPortId, watchEntryComparator);

  if (it != eventWatches.end() && it->getPortId() == paPortId) {
    eventWatches.erase(it);
    return true;
  }
  return false;
}

void CMonitoringHandler::readResourceWatches(std::string &paResponse) {
  if (!mFBMonitoringList.empty()) {
    paResponse += "<Resource name=\""s;
    paResponse += mResource.getInstanceName();
    paResponse += cgClosingXMLTag;

    updateMonitoringData();

    for (auto &monitoringEntry : mFBMonitoringList) {
      paResponse += "<FB name=\""s;
      paResponse += monitoringEntry.getFullFBName();
      paResponse += cgClosingXMLTag;

      // add the data watches
      for (auto &dataWatch : monitoringEntry.mWatchedDataPoints) {
        appendDataWatch(paResponse, dataWatch);
      }

      // add the event watches
      for (auto &eventWatch : monitoringEntry.mWatchedEventPoints) {
        appendEventWatch(paResponse, eventWatch);
      }

      paResponse += "</FB>"s;
    }
    paResponse += "</Resource>"s;
  }
}

void CMonitoringHandler::updateMonitoringData() {
  for (auto &monitoringEntry : mFBMonitoringList) {

    for (auto &dataWatch : monitoringEntry.mWatchedDataPoints) {
      dataWatch.update(monitoringEntry.getFB());
    }

    for (auto &eventWatch : monitoringEntry.mWatchedEventPoints) {
      eventWatch.update();
    }
  }
}

void CMonitoringHandler::appendDataWatch(std::string &paResponse, CDataWatchEntry &paDataWatchEntry) {
  appendPortTag(paResponse, paDataWatchEntry.getPortId());
  paResponse += "<Data value=\""s;
  size_t bufferSize = paDataWatchEntry.mDataBuffer->getToStringBufferSize() +
                      getExtraSizeForEscapedChars(*paDataWatchEntry.mDataBuffer);
  char *acDataValue = new char[bufferSize];
  int consumedBytes = paDataWatchEntry.mDataBuffer->toString(acDataValue, bufferSize);
  if (consumedBytes > 0 && static_cast<size_t>(consumedBytes) < bufferSize) {
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
      default: break;
    }
    acDataValue[consumedBytes] = '\0';
    paResponse += acDataValue;
  }
  paResponse += "\" forced=\""s;
  paResponse += (paDataWatchEntry.mForced) ? "true"s : "false"s;
  paResponse += "\"/></Port>"s;
  delete[] acDataValue;
}

size_t CMonitoringHandler::getExtraSizeForEscapedChars(const CIEC_ANY &paDataValue) {
  size_t retVal = 0;

  switch (paDataValue.getDataTypeID()) {
    case CIEC_ANY::e_ANY: retVal = getExtraSizeForEscapedChars(paDataValue.unwrap()); break;
    case CIEC_ANY::e_STRING:
      retVal = forte::core::util::getExtraSizeForXMLEscapedChars(
                   static_cast<const CIEC_STRING &>(paDataValue).getStorage().c_str()) +
               10; // for opening and closing quotes or apos
      break;
    case CIEC_ANY::e_WSTRING:
      retVal =
          forte::core::util::getExtraSizeForXMLEscapedChars(static_cast<const CIEC_WSTRING &>(paDataValue).getValue()) +
          10; // for opening and closing quotes or apos
      break;
    case CIEC_ANY::e_CHAR:
      retVal = 5 + 5 + 5; // Both outer quotes and symbol gets evetually replaced
      break;
    case CIEC_ANY::e_WCHAR:
      retVal = 5 + 5 + 5; // Both outer quotes and symbol gets evetually replaced
      break;
    case CIEC_ANY::e_ARRAY:
      retVal = getExtraSizeForEscapedCharsArray(static_cast<const CIEC_ARRAY &>(paDataValue));
      break;
    case CIEC_ANY::e_STRUCT:
      retVal = getExtraSizeForEscapedCharsStruct(static_cast<const CIEC_STRUCT &>(paDataValue));
      break;
    default: break;
  }

  return retVal;
}

size_t CMonitoringHandler::getExtraSizeForEscapedCharsArray(const CIEC_ARRAY &paDataValue) {
  size_t retVal = 0;
  auto lowerBound = paDataValue.getLowerBound();
  auto upperBound = paDataValue.getUpperBound();
  switch (paDataValue[lowerBound].getDataTypeID()) {
    case CIEC_ANY::e_STRING:
      for (auto i = lowerBound; i <= upperBound; i++) {
        retVal +=
            forte::core::util::getExtraSizeForXMLEscapedChars(
                static_cast<const CIEC_STRING &>(paDataValue[static_cast<TForteUInt16>(i)]).getStorage().c_str()) +
            10; // for opening and closing quotes or apos
      }
      break;
    case CIEC_ANY::e_WSTRING:
      for (auto i = lowerBound; i <= upperBound; i++) {
        retVal += forte::core::util::getExtraSizeForXMLEscapedChars(
                      static_cast<const CIEC_WSTRING &>(paDataValue[static_cast<TForteUInt16>(i)]).getValue()) +
                  10; // for opening and closing quotes or apos
      }
      break;
    case CIEC_ANY::e_STRUCT:
      for (auto i = lowerBound; i <= upperBound; i++) {
        retVal += getExtraSizeForEscapedCharsStruct(
            static_cast<const CIEC_STRUCT &>(paDataValue[static_cast<TForteUInt16>(i)]));
      }
      break;
    default: break;
  }

  return retVal;
}

size_t CMonitoringHandler::getExtraSizeForEscapedCharsStruct(const CIEC_STRUCT &paDataValue) {
  size_t retVal = 0;

  for (size_t i = 0; i < paDataValue.getStructSize(); i++) {
    const CIEC_ANY *member = paDataValue.getMember(i);
    switch (member->getDataTypeID()) {
      case CIEC_ANY::e_STRING:
        retVal += forte::core::util::getExtraSizeForXMLEscapedChars(
                      static_cast<const CIEC_STRING *>(member)->getStorage().c_str()) +
                  10; // for opening and closing quotes or apos
        break;
      case CIEC_ANY::e_WSTRING:
        retVal +=
            forte::core::util::getExtraSizeForXMLEscapedChars(static_cast<const CIEC_WSTRING *>(member)->getValue()) +
            10; // for opening and closing quotes or apos
        break;
      case CIEC_ANY::e_ARRAY:
        retVal += getExtraSizeForEscapedCharsArray(*static_cast<const CIEC_ARRAY *>(member));
        break;
      case CIEC_ANY::e_STRUCT:
        retVal += getExtraSizeForEscapedCharsStruct(*static_cast<const CIEC_STRUCT *>(member));
        break;
      default: break;
    }
  }

  return retVal;
}

void CMonitoringHandler::appendPortTag(std::string &paResponse, CStringDictionary::TStringId paPortId) {
  paResponse += "<Port name=\""s;
  paResponse += CStringDictionary::get(paPortId);
  paResponse += cgClosingXMLTag;
}

void CMonitoringHandler::appendEventWatch(std::string &paResponse, CEventWatchEntry &paEventWatchEntry) {
  appendPortTag(paResponse, paEventWatchEntry.getPortId());

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

void CMonitoringHandler::createFullFBName(std::string &paFullName, forte::core::TNameIdentifier &paNameList) {
  for (const auto &runner : paNameList) {
    paFullName.append(CStringDictionary::get(runner));
    paFullName.append(".");
  }
  paFullName.pop_back();
}
