/*******************************************************************************
 * Copyright (c) 2015, 2025 fortiss GmbH, Johannes Kepler University,

 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial implementation and rework communication infrastructure
 *               - moved forced flag to FB
 *******************************************************************************/
#ifndef MONITORING_H_
#define MONITORING_H_

#include <functional>
#include "funcbloc.h"
#include "mgmcmdstruct.h"
#include "fortelist.h"
#include "conn.h"
#include "stringdict.h"

class CFunctionBlock;
class CResource;

namespace forte::core {
  namespace internal {

    class CWatchEntry {
      public:
        CStringDictionary::TStringId getPortId() const {
          return mPortId;
        }

      protected:
        CWatchEntry(CStringDictionary::TStringId paPortId) : mPortId(paPortId) {
        }

        CWatchEntry(const CWatchEntry &) = default;
        CWatchEntry(CWatchEntry &&) = default;
        CWatchEntry &operator=(const CWatchEntry &) = default;
        CWatchEntry &operator=(CWatchEntry &&) = default;

      private:
        CStringDictionary::TStringId mPortId;
    };

    class CDataWatchEntry : public CWatchEntry {
      public:
        CDataWatchEntry(CStringDictionary::TStringId paPortId, CIEC_ANY &paDataValue, TAbsDataPortNum paForceIndex) :
            CWatchEntry(paPortId),
            mDataBuffer(paDataValue.clone(nullptr)),
            mForceIndex(paForceIndex),
            mDataValueRef(paDataValue) {
        }

        CDataWatchEntry(const CDataWatchEntry &paSrc) :
            CWatchEntry(paSrc),
            mDataBuffer(paSrc.mDataBuffer->clone(nullptr)),
            mForceIndex(paSrc.mForceIndex),
            mDataValueRef(paSrc.mDataValueRef) {
        }

        void update(const CFunctionBlock &paFB);

        std::unique_ptr<CIEC_ANY> mDataBuffer; //!< buffer for copying the data from the data point reference
        bool mForced; //!< indication if pin is forced

        CDataWatchEntry &operator=(const CDataWatchEntry &paOther) {
          if (this != &paOther) {
            CWatchEntry::operator=(paOther);
            mDataBuffer = mDataBuffer ? std::unique_ptr<CIEC_ANY>(paOther.mDataBuffer->clone(nullptr)) : nullptr;
            mForceIndex = paOther.mForceIndex;
            mDataValueRef = paOther.mDataValueRef;
          }
          return *this;
        }

        CDataWatchEntry(CDataWatchEntry &&) = default;
        CDataWatchEntry &operator=(CDataWatchEntry &&) = default;

      private:
        TAbsDataPortNum mForceIndex;
        std::reference_wrapper<CIEC_ANY> mDataValueRef; //!< reference to the data point to watch
    };

    class CEventWatchEntry : public CWatchEntry {
      public:
        CEventWatchEntry(CStringDictionary::TStringId paPortId, TForteUInt32 &paEventData) :
            CWatchEntry(paPortId),
            mEventDataRef(paEventData) {
        }

        void update();

        TForteUInt32 mEventDataBuf; //!< buffer for the event count

        CEventWatchEntry(const CEventWatchEntry &) = default;
        CEventWatchEntry(CEventWatchEntry &&) = default;
        CEventWatchEntry &operator=(const CEventWatchEntry &) = default;
        CEventWatchEntry &operator=(CEventWatchEntry &&) = default;

      private:
        std::reference_wrapper<TForteUInt32> mEventDataRef; //!< reference to the event counter of the watched event pin
    };

    struct SFBMonitoringEntry {
        SFBMonitoringEntry(std::string paFullFBName, CFunctionBlock *paFB) : mFullFBName(paFullFBName), mFB(paFB) {
        }

        const std::string &getFullFBName() const {
          return mFullFBName;
        }

        const CFunctionBlock &getFB() const {
          return *mFB;
        }

        std::vector<internal::CDataWatchEntry> mWatchedDataPoints;
        std::vector<internal::CEventWatchEntry> mWatchedEventPoints;

        SFBMonitoringEntry(const SFBMonitoringEntry &) = default;
        SFBMonitoringEntry(SFBMonitoringEntry &&) = default;
        SFBMonitoringEntry &operator=(const SFBMonitoringEntry &) = default;
        SFBMonitoringEntry &operator=(SFBMonitoringEntry &&) = default;

      private:
        std::string mFullFBName;
        CFunctionBlock *mFB;
    };

  } // namespace internal

  /*!\brief class that handles all monitoring tasks
   *
   */
  class CMonitoringHandler {
    public:
      explicit CMonitoringHandler(CResource &paResource);

      EMGMResponse executeMonitoringCommand(SManagementCMD &paCommand);

    private:
      CFunctionBlock *getFB(forte::core::TNameIdentifier &paNameList);

      EMGMResponse addWatch(forte::core::TNameIdentifier &paNameList);
      EMGMResponse removeWatch(forte::core::TNameIdentifier &paNameList);
      EMGMResponse readWatches(std::string &paResponse);
      EMGMResponse clearForce(forte::core::TNameIdentifier &paNameList);
      EMGMResponse triggerEvent(forte::core::TNameIdentifier &paNameList);
      EMGMResponse resetEventCount(forte::core::TNameIdentifier &paNameList);

      internal::SFBMonitoringEntry &findOrCreateFBMonitoringEntry(CFunctionBlock *paFB,
                                                                  forte::core::TNameIdentifier &paNameList);
      static void addDataWatch(internal::SFBMonitoringEntry &paFBMonitoringEntry,
                               CStringDictionary::TStringId paPortId,
                               CIEC_ANY &paDataVal);
      static bool removeDataWatch(internal::SFBMonitoringEntry &paFBMonitoringEntry,
                                  CStringDictionary::TStringId paPortId);
      static void addEventWatch(internal::SFBMonitoringEntry &paFBMonitoringEntry,
                                CStringDictionary::TStringId paPortId,
                                TForteUInt32 &paEventData);
      static bool removeEventWatch(internal::SFBMonitoringEntry &paFBMonitoringEntry,
                                   CStringDictionary::TStringId paPortId);
      void readResourceWatches(std::string &paResponse);

      void updateMonitoringData();

      static void appendDataWatch(std::string &paResponse, internal::CDataWatchEntry &paDataWatchEntry);
      static void appendPortTag(std::string &paResponse, CStringDictionary::TStringId paPortId);
      void appendEventWatch(std::string &paResponse, internal::CEventWatchEntry &paEventWatchEntry);
      static void createFullFBName(std::string &paFullName, forte::core::TNameIdentifier &paNameList);

      static size_t getExtraSizeForEscapedChars(const CIEC_ANY &paDataValue);

      static size_t getExtraSizeForEscapedCharsArray(const CIEC_ARRAY &paDataValue);

      static size_t getExtraSizeForEscapedCharsStruct(const CIEC_STRUCT &paDataValue);

      //! List storing all FBs which are currently monitored
      std::vector<internal::SFBMonitoringEntry> mFBMonitoringList;
      CResource &mResource; //!< The resource this monitoring handler manages

    public:
      CMonitoringHandler(const CMonitoringHandler &) = delete;
      CMonitoringHandler &operator=(const CMonitoringHandler &) = delete;
  };

} // namespace forte::core

#endif /* MONITORING_H_ */
