/*******************************************************************************
 * Copyright (c) 2015, 2018 fortiss GmbH, Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
#include "datatypes/forte_array.h"
#include "datatypes/forte_struct.h"

class CFunctionBlock;
class CResource;

namespace forte {
  namespace core {

    /*!\brief class that handles all monitoring tasks
     *
     */
    class CMonitoringHandler{
      public:
        explicit CMonitoringHandler(CResource &paResource);

        EMGMResponse executeMonitoringCommand(SManagementCMD &paCommand);

      private:
        class  SDataWatchEntry{
          public:
            SDataWatchEntry(CStringDictionary::TStringId paPortId, CIEC_ANY &paDataValue) :
                mPortId(paPortId), mDataValueRef(paDataValue), mDataBuffer(paDataValue.clone(0)){
            }

            SDataWatchEntry(const SDataWatchEntry& paSrc):
              mPortId(paSrc.mPortId), mDataValueRef(paSrc.mDataValueRef), mDataBuffer(paSrc.mDataBuffer->clone(0)){
            }

            ~SDataWatchEntry(){
              delete mDataBuffer;
            }

            CStringDictionary::TStringId mPortId;
            CIEC_ANY &mDataValueRef;  //!< reference to the data point to watch
            CIEC_ANY *mDataBuffer;    //!< buffer for copying the data from the data point reference

          private:
            SDataWatchEntry &operator=(const SDataWatchEntry&);  // only declaration no implementation to forbid assignment
        };

        struct SEventWatchEntry{
            SEventWatchEntry(CStringDictionary::TStringId paPortId,
                TForteUInt32 &paEventData) :
                mPortId(paPortId), mEventDataRef(paEventData){
            }

            CStringDictionary::TStringId mPortId;
            TForteUInt32 &mEventDataRef;    //!< reference to the event counter of the watched event pin
            TForteUInt32 mEventDataBuf;  //!< buffer for the event count
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
        EMGMResponse clearForce(forte::core::TNameIdentifier &paNameList);
        EMGMResponse triggerEvent(forte::core::TNameIdentifier &paNameList);
        EMGMResponse resetEventCount(forte::core::TNameIdentifier &paNameList);

        SFBMonitoringEntry &findOrCreateFBMonitoringEntry(CFunctionBlock *pa_poFB, forte::core::TNameIdentifier &paNameList);
        static void addDataWatch(SFBMonitoringEntry& pa_roFBMonitoringEntry, CStringDictionary::TStringId pa_unPortId, CIEC_ANY& pa_poDataVal);
        static bool removeDataWatch(SFBMonitoringEntry& pa_roFBMonitoringEntry, CStringDictionary::TStringId pa_unPortId);
        static void addEventWatch(SFBMonitoringEntry& paFBMonitoringEntry, CStringDictionary::TStringId paPortId, TForteUInt32& paEventData);
        static bool removeEventWatch(SFBMonitoringEntry& pa_roFBMonitoringEntry, CStringDictionary::TStringId pa_unPortId);
        void readResourceWatches(CIEC_STRING &pa_roResponse);

        void updateMonitringData();

        static void appendDataWatch(CIEC_STRING &pa_roResponse,
            SDataWatchEntry &pa_roDataWatchEntry);
        static void appendPortTag(CIEC_STRING &pa_roResponse,
            CStringDictionary::TStringId pa_unPortId);
        void appendEventWatch(CIEC_STRING &pa_roResponse, SEventWatchEntry &pa_roEventWatchEntry);

        static void createFullFBName(CIEC_STRING &paFullName, forte::core::TNameIdentifier &paNameList);

        static size_t getExtraSizeForEscapedChars(const CIEC_ANY& paDataValue);

        static size_t getExtraSizeForEscapedCharsArray(const CIEC_ARRAY& paDataValue);

        static size_t getExtraSizeForEscapedCharsStruct(const CIEC_STRUCT& paDataValue);

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
