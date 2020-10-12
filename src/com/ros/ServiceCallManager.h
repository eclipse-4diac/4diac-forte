/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ben Schneider
 *      - initial implementation and documentation
 *******************************************************************************/

#ifndef CSERVICECALLMANAGER_H
#define CSERVICECALLMANAGER_H

#include <esfb.h>
#include <extevhan.h>
#include <forte_thread.h>

#include <vector>

class FORTE_TRIGGER_SERVICE_CLIENT;

class CServiceCallManager : public CExternalEventHandler, private CThread{
  DECLARE_HANDLER(CServiceCallManager)

  private:
    std::vector<FORTE_TRIGGER_SERVICE_CLIENT*> m_callerVector;
    std::vector<FORTE_TRIGGER_SERVICE_CLIENT*> m_connectVector;

  public:
    virtual void enableHandler();
    virtual void disableHandler();
    virtual void setPriority(int pa_prio);
    virtual int getPriority() const;

    void startChain(CEventSourceFB* pa_poECStartF);
    void queueServiceCall(FORTE_TRIGGER_SERVICE_CLIENT* pa_serviceClientPtr);
    void queueConnectWait(FORTE_TRIGGER_SERVICE_CLIENT* pa_serviceClientPtr);

  protected:
    virtual void run();

};

#endif
