/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Waldemar Eisenmenger - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _SYSFSPROCINT_H_
#define _SYSFSPROCINT_H_

#include "../../stdfblib/io/processinterfacebase.h"
#include <forte_thread.h>
#include <extevhan.h>
#include <fstream>

class CSysFsProcessInterface : public CProcessInterfaceBase {

  public:
    CSysFsProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId);
    ~CSysFsProcessInterface() override;

    class CIOHandler : public CExternalEventHandler, public CThread {
      DECLARE_HANDLER(CIOHandler)
        ;
      public:
        void run() override;
        void updateReadData();
        void registerIXFB(CSysFsProcessInterface *paFB);
        void unregisterIXFB(CSysFsProcessInterface *paFB);
        /* functions needed for the external event handler interface */
        void enableHandler() override;
        void disableHandler() override;
        void setPriority(int paPriority) override;
        int getPriority() const override;

      private:
        typedef CSinglyLinkedList<CSysFsProcessInterface *> TReadFBContainer;
        TReadFBContainer mReadFBList;
        CSyncObject mReadFBListSync;
    };

  protected:
    bool initialise(bool paIsInput, CEventChainExecutionThread *const paECET);
    bool deinitialise();
    bool writePin();
    bool readPin();
    bool checkInputData();

  private:
    std::fstream mFile;

    bool exportGPIO();
    bool setDirection(bool paIsInput);
    bool valueGPIO(bool paIsInput);

    bool unexportIO();

    static const CIEC_STRING scmOK;
    static const CIEC_STRING scmPinInUse;
    static const CIEC_STRING scmNotInitialised;
    static const CIEC_STRING scmError;
    static const CIEC_STRING scmCouldNotRead;
    static const CIEC_STRING scmCouldNotWrite;
};

#endif /* PROCESSINTERFACE_H_ */
