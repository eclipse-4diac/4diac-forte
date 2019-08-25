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

#include <extevhan.h>
#include <forte_thread.h>
#include <singlet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

class CMLPIFaceProcessInterface : public CProcessInterfaceBase{
  public:
    CMLPIFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
        TForteByte *paFBVarsData);

    virtual ~CMLPIFaceProcessInterface();

    class CIOHandler : public CExternalEventHandler, public CThread{
      DECLARE_HANDLER(CIOHandler)
      private:
        typedef CSinglyLinkedList<CMLPIFaceProcessInterface *> TReadFBContainer;
        TReadFBContainer mReadFBList;
        CSyncObject mReadFBListSync;
      public:
        virtual void run();
        void updateReadData();
        void registerIXFB(CMLPIFaceProcessInterface *paFB);
        void unregisterIXFB(CMLPIFaceProcessInterface *paFB);
        /* functions needed for the external event handler interface */
        void enableHandler(void);
        void disableHandler(void);
        void setPriority(int paPriority);
        int getPriority(void) const;
    };

  protected:
    bool initialise(bool paInput);
    bool deinitialise();
    bool readPin();
    bool writePin();
    bool checkInputData();

  private:

    bool connectToMLPI();
    void disconnectFromMLPI();

    WCHAR16* mVariableName;

    static const char * const scmOK;
    static const char * const scmCallToApiFailed;
    static const char * const scmAPINotInitialised;
    static const char * const scmFBNotInitialised;
    static MLPIHANDLE smConnection;

    static const unsigned int mMaxNumberOfTriesToReconnect = 10;

};

//tell the IX and QX FB that this is the process interface to be used
typedef CMLPIFaceProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
