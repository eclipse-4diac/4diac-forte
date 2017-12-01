/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
#define MAX_NUMBER_OF_RETRIES_TO_CONNECT 10

class CMLPIFaceProcessInterface : public CProcessInterfaceBase{
  public:
    CMLPIFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
        TForteByte *paFBVarsData);

    virtual ~CMLPIFaceProcessInterface();

  protected:
    bool initialise(bool paInput);
    bool deinitialise();
    bool readPin();
    bool writePin();
    bool checkInputData();

  private:
    class CIOHandler : public CExternalEventHandler, public CThread{
        DECLARE_SINGLETON(CIOHandler)
        private:
          typedef CSinglyLinkedList<CMLPIFaceProcessInterface *> TReadFBContainer;
          TReadFBContainer m_lstReadFBList;
          CSyncObject m_oReadFBListSync;
          uint32_t m_unTaskId;
        public:
          virtual void run();
          void updateReadData();
          void registerIXFB(CMLPIFaceProcessInterface *pa_poFB);
          void unregisterIXFB(CMLPIFaceProcessInterface *pa_poFB);
          /* functions needed for the external event handler interface */
          void enableHandler(void);
          void disableHandler(void);
          void setPriority(int pa_nPriority);
          int getPriority(void) const;
      };

    bool connectToMLPI();
    void disconnectFromMLPI();

    WCHAR16* mVariableName;

    static const char * const scmOK;
    static const char * const scmCallToApiFailed;
    static const char * const scmAPINotInitialised;
    static const char * const scmFBNotInitialised;
    static MLPIHANDLE smConnection;

};

//tell the IX and QX FB that this is the process interface to be used
typedef CMLPIFaceProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
