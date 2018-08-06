/*******************************************************************************
 * Copyright (c) 2015, 2018 fortiss GmbH, Johannes Kepler University
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef MONITORING_H_
#define MONITORING_H_

#include "mgmcmdstruct.h"
#include "fortelist.h"
#include "event.h"
#include "conn.h"
#include "stringdict.h"
#include "../arch/timerha.h"

class CFunctionBlock;
class CResource;

namespace forte {
  namespace core {

    const TForteUInt32 cgMonitorBufferLength = 1;

    enum EBreakPointStatus{
        eActive = 0,    // we are comparing in the main loop against this one therefore it gets zero to make it faster
        eOnce,
        eInactive
    };

    struct SMonitorEvent{
        TForteUInt16 mBufPos;
        EBreakPointStatus mBreakpointSet;        //!< eOnce if breakpoint is active and should continue once, eInactive - currently no breakpoint active, eActive - breakpoint currently active TODO change to enum
        bool mBreakpointEnable;     //!< breakpoint activated - a breackpoint is specified for this event

        struct SMonitorEventData{
          TForteUInt32 mEventCount; //!< Event count
          SForteTime mTimeStamp;   //!< Timestamp when the event occurred

          SMonitorEventData() : mEventCount(0) {
          }
        };

        SMonitorEventData mMonitorEventData[cgMonitorBufferLength];

        SMonitorEvent() : mBufPos(0), mBreakpointSet(eInactive), mBreakpointEnable(false){
        }
    };


    /*!\brief class that handles all monitoring tasks
     *
     */
    class CMonitoringHandler{
      public:
        explicit CMonitoringHandler(CResource &paResource);

        EMGMResponse executeMonitoringCommand(SManagementCMD &paCommand);

      private:
        struct SDataWatchEntry{
            SDataWatchEntry(CStringDictionary::TStringId paPortId, CIEC_ANY &paDataValue) :
                mPortId(paPortId), mDataValue(paDataValue){
            }

            CStringDictionary::TStringId mPortId;
            CIEC_ANY &mDataValue;
        };

        struct SEventWatchEntry{
            SEventWatchEntry(CStringDictionary::TStringId pa_unPortId,
                SMonitorEvent &pa_roEventData) :
                m_unPortId(pa_unPortId), m_roEventData(pa_roEventData){
            }

            CStringDictionary::TStringId m_unPortId;
            SMonitorEvent &m_roEventData;
        };

        typedef CSinglyLinkedList<SDataWatchEntry> TDataWatchList;
        typedef CSinglyLinkedList<SEventWatchEntry> TEventWatchList;

        struct SFBMonitoringEntry{
            CIEC_STRING mFullFBName;
            CFunctionBlock *m_poFB;
            TDataWatchList m_lstWatchedDataPoints;
            TEventWatchList m_lstWatchedEventPoints;
        };

        typedef CSinglyLinkedList<SFBMonitoringEntry> TFBMonitoringList;

        CFunctionBlock* getFB(forte::core::TNameIdentifier &paNameList);

        EMGMResponse addWatch(forte::core::TNameIdentifier &paNameList);
        EMGMResponse removeWatch(forte::core::TNameIdentifier &paNameList);
        EMGMResponse readWatches(CIEC_STRING &pa_roResponse);
        EMGMResponse getWatches(CIEC_STRING &pa_roResponse, char pa_cQualifier);
        EMGMResponse clearForce(forte::core::TNameIdentifier &paNameList);
        EMGMResponse triggerEvent(forte::core::TNameIdentifier &paNameList);
        EMGMResponse resetEventCount(forte::core::TNameIdentifier &paNameList);

        SFBMonitoringEntry &findOrCreateFBMonitoringEntry(CFunctionBlock *pa_poFB, forte::core::TNameIdentifier &paNameList);
        void addDataWatch(SFBMonitoringEntry &pa_roFBMonitoringEntry,
            CStringDictionary::TStringId pa_unPortId, CIEC_ANY &pa_poDataVal);
        bool removeDataWatch(SFBMonitoringEntry &pa_roFBMonitoringEntry,
            CStringDictionary::TStringId pa_unPortId);
        void addEventWatch(SFBMonitoringEntry &pa_roFBMonitoringEntry,
            CStringDictionary::TStringId pa_unPortId, SMonitorEvent &pa_rstEventData);
        bool removeEventWatch(SFBMonitoringEntry &pa_roFBMonitoringEntry,
            CStringDictionary::TStringId pa_unPortId);
        void readResourceWatches(CIEC_STRING &pa_roResponse);
        void getResourceWatches(CIEC_STRING &pa_roResponse, char pa_cQualifier);

        static void appendDataWatch(CIEC_STRING &pa_roResponse,
            SDataWatchEntry &pa_roDataWatchEntry);
        static void appendPortTag(CIEC_STRING &pa_roResponse,
            CStringDictionary::TStringId pa_unPortId);
        static void appendEventWatch(CIEC_STRING &pa_roResponse,
            SEventWatchEntry &pa_roEventWatchEntry);

        static void createFullFBName(CIEC_STRING &paFullName, forte::core::TNameIdentifier &paNameList);

        //!List storing all FBs which are currently monitored
        TFBMonitoringList mFBMonitoringList;

        //!Event entry for triggering input events
        SEventEntry mTriggerEvent;

        CResource &mResource; //!< The resource this monitoring handler manages

        //don't allow that CMonitoringHandler can be copy therefore making the copy constructor private and not implemented
        CMonitoringHandler(const CMonitoringHandler&);
        CMonitoringHandler& operator =(const CMonitoringHandler &);
    };

  }
}
#endif /* MONITORING_H_ */
