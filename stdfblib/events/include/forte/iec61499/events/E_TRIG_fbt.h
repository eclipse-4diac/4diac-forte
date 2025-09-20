/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TRIG
 *** Description: Trigger unconnected input events of the specified type inside a resource
 *** Version:
 ***     1.0: 2025-01-08/Mario Kastner -  -
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  class FORTE_E_TRIG final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_E_TRIG)

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      CEventChainExecutionThread *mEcet;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      /*!\brief This function will trigger unconnected event ports of a certain EventType within a resource
       * \param paEventTypeId ID of event type to be triggered
       */
      void triggerEventsInResource(CFBContainer *paContainer,
                                   const TEventTypeID paEventType,
                                   CEventChainExecutionThread *const paECET);

      /*!\brief This function will trigger unconnected event ports of a certain EventType
       * \param paEventTypeId ID of event type to be triggered
       * \param paFb function block search for events to be triggerd
       * \param paECET event chain execution thread to add the event
       */
      void
      triggerEventsOfType(TEventTypeID paEventTypeId, CFunctionBlock *paFb, CEventChainExecutionThread *const paECET);

    public:
      FORTE_E_TRIG(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_STRING var_EVENTTYPE;

      CEventConnection conn_CNF;

      CDataConnection *conn_EVENTTYPE;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_STRING &paEVENTTYPE) {
        var_EVENTTYPE = paEVENTTYPE;
        executeEvent(scmEventREQID, nullptr);
      }

      void operator()(const CIEC_STRING &paEVENTTYPE) {
        evt_REQ(paEVENTTYPE);
      }
  };
} // namespace forte::iec61499::events
