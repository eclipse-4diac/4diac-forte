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
#include "forte/funcbloc.h"
#include "forte/mgmcmdstruct.h"
#include "forte/conn.h"
#include "forte/stringid.h"

namespace forte {
  class CFunctionBlock;
  class CResource;

  namespace internal {

    class CWatchEntry {
      public:
        StringId getPortId() const {
          return mPortId;
        }

        CWatchEntry(const CWatchEntry &) = delete;
        CWatchEntry &operator=(const CWatchEntry &) = delete;

      protected:
        CWatchEntry(StringId paPortId) : mPortId(paPortId) {
        }

        CWatchEntry(CWatchEntry &&) = default;
        CWatchEntry &operator=(CWatchEntry &&) = default;

      private:
        StringId mPortId;
    };

    class CDataWatchEntry : public CWatchEntry {
      public:
        CDataWatchEntry(StringId paPortId, CIEC_ANY &paDataValue, TAbsDataPortNum paForceIndex) :
            CWatchEntry(paPortId),
            mDataBuffer(paDataValue.clone(nullptr)),
            mForceIndex(paForceIndex),
            mDataValueRef(paDataValue) {
        }

        void update(const CFunctionBlock &paFB);

        std::unique_ptr<CIEC_ANY> mDataBuffer; //!< buffer for copying the data from the data point reference
        bool mForced; //!< indication if pin is forced

        CDataWatchEntry(const CDataWatchEntry &paSrc) = delete;
        CDataWatchEntry(CDataWatchEntry &&) = default;
        CDataWatchEntry &operator=(const CDataWatchEntry &paOther) = delete;
        CDataWatchEntry &operator=(CDataWatchEntry &&) = default;

      private:
        TAbsDataPortNum mForceIndex;
        std::reference_wrapper<CIEC_ANY> mDataValueRef; //!< reference to the data point to watch
    };

    class CEventWatchEntry : public CWatchEntry {
      public:
        CEventWatchEntry(StringId paPortId, TForteUInt32 &paEventData) :
            CWatchEntry(paPortId),
            mEventDataRef(paEventData) {
        }

        void update();

        TForteUInt32 mEventDataBuf; //!< buffer for the event count

        CEventWatchEntry(const CEventWatchEntry &) = delete;
        CEventWatchEntry(CEventWatchEntry &&) = default;
        CEventWatchEntry &operator=(const CEventWatchEntry &) = delete;
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

        std::vector<CDataWatchEntry> mWatchedDataPoints;
        std::vector<CEventWatchEntry> mWatchedEventPoints;

        SFBMonitoringEntry(const SFBMonitoringEntry &) = delete;
        SFBMonitoringEntry(SFBMonitoringEntry &&) = default;
        SFBMonitoringEntry &operator=(const SFBMonitoringEntry &) = delete;
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
      CFunctionBlock *getFB(TNameIdentifier &paNameList);

      EMGMResponse addWatch(TNameIdentifier &paNameList);
      EMGMResponse removeWatch(TNameIdentifier &paNameList);
      EMGMResponse readWatches(std::string &paResponse);
      EMGMResponse clearForce(TNameIdentifier &paNameList);
      EMGMResponse triggerEvent(TNameIdentifier &paNameList);
      EMGMResponse resetEventCount(TNameIdentifier &paNameList);

      internal::SFBMonitoringEntry &findOrCreateFBMonitoringEntry(CFunctionBlock *paFB, TNameIdentifier &paNameList);
      void readResourceWatches(std::string &paResponse);

      void updateMonitoringData();

      //! List storing all FBs which are currently monitored
      std::vector<internal::SFBMonitoringEntry> mFBMonitoringList;
      CResource &mResource; //!< The resource this monitoring handler manages

    public:
      CMonitoringHandler(const CMonitoringHandler &) = delete;
      CMonitoringHandler &operator=(const CMonitoringHandler &) = delete;
  };

} // namespace forte

#endif /* MONITORING_H_ */
