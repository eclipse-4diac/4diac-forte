/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _EVENCONN_H_
#define _EVENCONN_H_

#include "conn.h"

class CEventChainExecutionThread;

/*!\ingroup CORE \brief Class for handling an event connection.
 *
 */
class CEventConnection : public CConnection{
  public:
    CEventConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId);

    ~CEventConnection() override;

    EMGMResponse connect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    EMGMResponse connectToCFBInterface(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    EMGMResponse disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) override;

    /*! \brief Triggers the event connection and all destinations are notified.
     *
     *
     *  \param paExecEnv Pointer to the execution environment the event is sent in.
     */
    void triggerEvent(CEventChainExecutionThread *paExecEnv) const;

};

typedef CEventConnection *TEventConnectionPtr;

#endif /*_EVENCONN_H_*/
