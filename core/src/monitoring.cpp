
/*******************************************************************************
 * Copyright (c) 2015, 2026 fortiss GmbH, Johannes Kepler University,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - adds functionality for W/CHAR
 *   Markus Meingast - Change type of CWatchEntry::mPortId to TNameIdentifier
 *******************************************************************************/

#include "monitoring.h"

#include <algorithm>
#include "forte/config/forte_config.h"
#include "forte/funcbloc.h"
#include "forte/resource.h"
#include "resource_internal.h"
#include "forte/ecet.h"
#include "forte/util/string_utils.h"
#include <string>

using namespace std::string_literals;

namespace forte {
  namespace internal {
    const std::string cgClosingXMLTag = "\">"s;
    const char cgNameDelimiter = '.';

    namespace {
      constexpr auto watchEntryComparator = [](const CWatchEntry &paItem, std::span<const StringId> paPortId) {
        return paItem.getPortId() < paPortId;
      };

      constexpr auto monitoringEntryComparator =
          [](const SFBMonitoringEntry &paItem, const CFunctionBlock *const paFB) { return &paItem.getFB() < paFB; };

      void createFullFBName(std::string &paFullName, std::span<const StringId> paNameList) {
        util::join(paNameList.begin(), paNameList.end(), {}, cgNameDelimiter, paFullName);
      }

      void
      addDataWatch(SFBMonitoringEntry &paFBMonitoringEntry, std::span<const StringId> paPortId, CIEC_ANY &paDataVal) {
        auto &dataWatches = paFBMonitoringEntry.mWatchedDataPoints;
        auto it = std::lower_bound(dataWatches.begin(), dataWatches.end(), paPortId, watchEntryComparator);

        if (it != dataWatches.end() && it->getPortId() == paPortId) {
          // the data point is already in the watch list
          return;
        }

        dataWatches.emplace(it, paPortId, paDataVal, paFBMonitoringEntry.getFB().getAbsDataPortNum(paPortId.front()));
      }

      bool removeDataWatch(SFBMonitoringEntry &paFBMonitoringEntry, std::span<const StringId> paPortId) {
        auto &dataWatches = paFBMonitoringEntry.mWatchedDataPoints;
        auto it = std::lower_bound(dataWatches.begin(), dataWatches.end(), paPortId, watchEntryComparator);

        if (it != dataWatches.end() && it->getPortId() == paPortId) {
          dataWatches.erase(it);
          return true;
        }
        return false;
      }

      void addEventWatch(SFBMonitoringEntry &paFBMonitoringEntry,
                         std::span<const StringId> paPortId,
                         TForteUInt32 &paEventData) {
        auto &eventWatches = paFBMonitoringEntry.mWatchedEventPoints;
        auto it = std::lower_bound(eventWatches.begin(), eventWatches.end(), paPortId, watchEntryComparator);

        if (it != eventWatches.end() && it->getPortId() == paPortId) {
          // the data point is already in the watch list
          return;
        }

        eventWatches.emplace(it, paPortId, paEventData);
      }

      bool removeEventWatch(SFBMonitoringEntry &paFBMonitoringEntry, std::span<const StringId> paPortId) {
        auto &eventWatches = paFBMonitoringEntry.mWatchedEventPoints;
        auto it = std::lower_bound(eventWatches.begin(), eventWatches.end(), paPortId, watchEntryComparator);

        if (it != eventWatches.end() && it->getPortId() == paPortId) {
          eventWatches.erase(it);
          return true;
        }
        return false;
      }

      void appendPortTag(std::string &paResponse, const TNameIdentifier &paPortId) {
        paResponse += "<Port name=\""s;
        util::join(paPortId.cbegin(), paPortId.cend(), {}, cgNameDelimiter, paResponse);
        paResponse += cgClosingXMLTag;
      }

      void appendDataWatch(std::string &paResponse, CDataWatchEntry &paDataWatchEntry) {
        appendPortTag(paResponse, paDataWatchEntry.getPortId());
        paResponse += "<Data value=\""s;

        auto start = paResponse.size();
        paDataWatchEntry.mDataBuffer->toString(paResponse);

        if (start != paResponse.size()) {
          switch (paDataWatchEntry.mDataBuffer->getDataTypeID()) {
            case CIEC_ANY::e_ANY:
            case CIEC_ANY::e_WSTRING:
            case CIEC_ANY::e_STRING:
            case CIEC_ANY::e_CHAR:
            case CIEC_ANY::e_WCHAR:
            case CIEC_ANY::e_ARRAY:
            case CIEC_ANY::e_STRUCT: util::transformNonEscapedToEscapedXMLText(paResponse, start); break;
            default: break;
          }
        }
        paResponse += "\" forced=\""s;
        paResponse += (paDataWatchEntry.mForced) ? "true"s : "false"s;
        paResponse += "\"/></Port>"s;
      }

      void appendEventWatch(std::string &paResponse, CEventWatchEntry &paEventWatchEntry) {
        appendPortTag(paResponse, paEventWatchEntry.getPortId());

        paResponse += "<Data value=\""s;
        paResponse.append(std::to_string(paEventWatchEntry.mEventDataBuf));
        paResponse += "\"/>\n</Port>"s;
      }
    } // namespace

    void CDataWatchEntry::update(const CFunctionBlock &paFB) {
      mDataBuffer->setValue(mDataValueRef);
      mForced = paFB.isForced(mForceIndex);
    }

    void CEventWatchEntry::update() {
      mEventDataBuf = mEventDataRef;
    }
  } // namespace internal

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

  CFunctionBlock *CMonitoringHandler::getFB(TNameIdentifier &paNameList) {
    auto runner = paNameList.cbegin();
    return mResource.getFB(runner, paNameList.cend());
  }

  CFunctionBlock *CMonitoringHandler::getFB(TNameIdentifier &paNameList,
                                            std::span<const StringId> &paFBNameList,
                                            std::span<const StringId> &paPortNameList) {
    auto runner = paNameList.cbegin();
    CFunctionBlock *fb = mResource.getFB(runner, paNameList.cend());
    if (fb != nullptr) {
      const auto fbLength = static_cast<size_t>(std::distance(paNameList.cbegin(), runner));
      const auto portLength = paNameList.size() - fbLength;
      paFBNameList = std::span<const StringId>(paNameList.begin(), fbLength);
      paPortNameList = std::span<const StringId>(paNameList.begin() + fbLength, portLength);
    }
    return fb;
  }

  EMGMResponse CMonitoringHandler::addWatch(TNameIdentifier &paNameList) {
    EMGMResponse eRetVal = EMGMResponse::NoSuchObject;
    std::span<const StringId> fbNameList;
    std::span<const StringId> portNameList;
    CFunctionBlock *fB = getFB(paNameList, fbNameList, portNameList);

    if (nullptr != fB) {
      internal::SFBMonitoringEntry &fbMonitoringEntry(findOrCreateFBMonitoringEntry(fB, fbNameList));

      CIEC_ANY *dataVal = fB->getVar(portNameList);
      if (nullptr != dataVal) {
        internal::addDataWatch(fbMonitoringEntry, portNameList, *dataVal);
        eRetVal = EMGMResponse::Ready;
      } else {
        TEventID eventId = fB->getFBInterfaceSpec().getEIID(portNameList.front());
        if (cgInvalidEventID != eventId) {
          internal::addEventWatch(fbMonitoringEntry, portNameList, fB->getEIMonitorData(eventId));
          eRetVal = EMGMResponse::Ready;
        } else {
          eventId = fB->getFBInterfaceSpec().getEOID(portNameList.front());
          if (cgInvalidEventID != eventId) {
            internal::addEventWatch(fbMonitoringEntry, portNameList, fB->getEOMonitorData(eventId));
            eRetVal = EMGMResponse::Ready;
          }
        }
      }
    }

    return eRetVal;
  }

  EMGMResponse CMonitoringHandler::removeWatch(TNameIdentifier &paNameList) {
    std::span<const StringId> fbNameList;
    std::span<const StringId> portNameList;
    CFunctionBlock *fB = getFB(paNameList, fbNameList, portNameList);
    if (nullptr != fB) {
      auto it =
          std::lower_bound(mFBMonitoringList.begin(), mFBMonitoringList.end(), fB, internal::monitoringEntryComparator);

      if (it != mFBMonitoringList.end() && &it->getFB() == fB) {
        internal::SFBMonitoringEntry &monitoringEntry = *it;
        if (internal::removeDataWatch(monitoringEntry, portNameList) ||
            internal::removeEventWatch(monitoringEntry,
                                       portNameList)) { // if element is not watched, end search and return error

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
        static_cast<CResource *>(*itRunner)->getInternal().getMonitoringHandler().readResourceWatches(paResponse);
      }
    } else {
      // we are within a resource
      readResourceWatches(paResponse);
    }

    return EMGMResponse::Ready;
  }

  EMGMResponse CMonitoringHandler::clearForce(TNameIdentifier &paNameList) {
    if (!mResource.setForced(paNameList, false)) {
      return EMGMResponse::NoSuchObject;
    }
    return EMGMResponse::Ready;
  }

  EMGMResponse CMonitoringHandler::triggerEvent(TNameIdentifier &paNameList) {
    EMGMResponse eRetVal = EMGMResponse::NoSuchObject;
    StringId portName = paNameList.back();
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

  EMGMResponse CMonitoringHandler::resetEventCount(TNameIdentifier &paNameList) {
    EMGMResponse eRetVal = EMGMResponse::NoSuchObject;
    StringId portName = paNameList.back();
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

  internal::SFBMonitoringEntry &
  CMonitoringHandler::findOrCreateFBMonitoringEntry(CFunctionBlock *paFB, std::span<const StringId> paNameList) {
    auto it =
        std::lower_bound(mFBMonitoringList.begin(), mFBMonitoringList.end(), paFB, internal::monitoringEntryComparator);

    if (it != mFBMonitoringList.end() && &it->getFB() == paFB) {
      // the data point is already in the watch list
      return *it;
    }

    std::string fullFBName;
    internal::createFullFBName(fullFBName, paNameList);
    return *mFBMonitoringList.emplace(it, std::move(fullFBName), paFB);
  }

  void CMonitoringHandler::readResourceWatches(std::string &paResponse) {
    if (!mFBMonitoringList.empty()) {
      paResponse += "<Resource name=\""s;
      paResponse += mResource.getInstanceName();
      paResponse += internal::cgClosingXMLTag;

      updateMonitoringData();

      for (auto &monitoringEntry : mFBMonitoringList) {
        paResponse += "<FB name=\""s;
        paResponse += monitoringEntry.getFullFBName();
        paResponse += internal::cgClosingXMLTag;

        // add the data watches
        for (auto &dataWatch : monitoringEntry.mWatchedDataPoints) {
          internal::appendDataWatch(paResponse, dataWatch);
        }

        // add the event watches
        for (auto &eventWatch : monitoringEntry.mWatchedEventPoints) {
          internal::appendEventWatch(paResponse, eventWatch);
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
} // namespace forte
