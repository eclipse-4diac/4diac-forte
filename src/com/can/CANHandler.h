/*******************************************************************************
 * Copyright (c) 2025 Alexander Trojnin github.com/trojninalex
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alexander Trojnin - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef CANHANDLER_H_
#define CANHANDLER_H_

#include <linux/can.h>
#include <linux/can/raw.h>

#include <sockhand.h>
#include <forte_config.h>
#include <extevhan.h>
#include <comlayer.h>

#include "CANLayer.h"

class CCANHandler : public CExternalEventHandler, private CThread
{
    DECLARE_HANDLER(CCANHandler)
public:
    void addComCallback(CFDSelectHandler::TFileDescriptor paFD, forte::com_infra::CComCallback *paCanLayer);
    void removeComCallback(CFDSelectHandler::TFileDescriptor paFD);

    void enableHandler(void)
    {
        start();
    }

    void disableHandler(void)
    {
        end();
    }

    void setPriority(int)
    {
    }

    int getPriority(void) const
    {
        return 0;
    }

protected:
    virtual void run(void);

private:
    struct TConnContType
    {
        CFDSelectHandler::TFileDescriptor mSockDes;
        forte::com_infra::CComCallback *mCalled;
    };

    struct can_frame frame;

    const CFDSelectHandler::TFileDescriptor scmInvalidFileDescriptor = FORTE_INVALID_SOCKET;

    typedef std::vector <TConnContType> TConnectionContainer;

    CFDSelectHandler::TFileDescriptor createFDSet(fd_set *m_panFDSet);

    TConnectionContainer mConnectionsList;
    CSyncObject mSync;
    bool mConnectionListChanged;
};

#endif
