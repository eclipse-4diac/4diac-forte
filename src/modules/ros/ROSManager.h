/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
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

#ifndef CROSMANAGER_H_
#define CROSMANAGER_H_

#include <extevhan.h>
#include <forte_thread.h>

#include <forte_string.h>
#include <string>

class CROSManager : public CExternalEventHandler, private CThread{
  DECLARE_HANDLER(CROSManager)

  public:
    virtual void enableHandler();
    virtual void disableHandler();
    virtual void setPriority(int pa_prio);
    virtual int getPriority() const;

    void startChain(CEventSourceFB *pa_poECStartF);
    std::string ciecStringToStdString(const CIEC_STRING &pa_DataInput);
    CIEC_STRING stdStringToCiecString(const std::string &pa_inputString);

  protected:
    virtual void run();

};

#endif /* CROSMANAGER_H_ */
