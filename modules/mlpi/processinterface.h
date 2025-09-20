/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef PROCESSINTERFACE_H_
#define PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <mlpiApiLib.h>

#include "forte/extevhan.h"
#include "forte/arch/forte_thread.h"
#include "forte/util/singlet.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

class CMLPIFaceProcessInterface : public CProcessInterfaceBase {
  public:
    CMLPIFaceProcessInterface(forte::CFBContainer &paContainer,
                              const SFBInterfaceSpec &paInterfaceSpec,
                              const forte::StringId paInstanceNameId);

    ~CMLPIFaceProcessInterface() override;

    class CIOHandler : public CExternalEventHandler, public RegisterExternalEventHandler<CIOHandler>, public CThread {
      public:
        explicit CIOHandler(CDeviceExecution &paDeviceExecution);
        ~CIOHandler() override;

      private:
        std::vector<CMLPIFaceProcessInterface *> mReadFBList;
        CSyncObject mReadFBListSync;

      public:
        void run() override;
        void updateReadData();
        void registerIXFB(CMLPIFaceProcessInterface *paFB);
        void unregisterIXFB(CMLPIFaceProcessInterface *paFB);
        /* functions needed for the external event handler interface */
        void enableHandler() override;
        void disableHandler() override;
    };

  protected:
    bool initialise(bool paInput, CEventChainExecutionThread *const paECET);
    bool deinitialise();
    bool readPin();
    bool writePin();
    bool checkInputData();

  private:
    bool connectToMLPI();
    void disconnectFromMLPI();

    WCHAR16 *mVariableName;

    static const char *const scmOK;
    static const char *const scmCallToApiFailed;
    static const char *const scmAPINotInitialised;
    static const char *const scmFBNotInitialised;
    static MLPIHANDLE smConnection;

    static const unsigned int mMaxNumberOfTriesToReconnect = 10;
};

// tell the IX and QX FB that this is the process interface to be used
typedef CMLPIFaceProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
