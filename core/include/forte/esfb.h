/*******************************************************************************
 * Copyright (c) 2005 - 2014 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"

namespace forte {
  /*!\ingroup CORE\brief Base-Class for all event sources.
   */
  class CEventSourceFB : public CFunctionBlock {
    private:
      /* \brief the event chain executor used by this ES.
       */
      CEventChainExecutionThread *mEventChainExecutor;
      TEventEntry mEventSourceEventEntry; //! the event entry to start the event chain

    public:
      CEventSourceFB(CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, StringId paInstanceNameId) :
          CFunctionBlock(paContainer, paInterfaceSpec, paInstanceNameId),
          mEventChainExecutor(nullptr),
          mEventSourceEventEntry(*this, cgExternalEventID) {
      }

      ~CEventSourceFB() override = default;
      void setEventChainExecutor(CEventChainExecutionThread *paEventChainExecutor) {
        mEventChainExecutor = paEventChainExecutor;
      };
      CEventChainExecutionThread *getEventChainExecutor() {
        return mEventChainExecutor;
      };

      TEventEntry *getEventSourceEventEntry() {
        return &mEventSourceEventEntry;
      };
  };
} // namespace forte
