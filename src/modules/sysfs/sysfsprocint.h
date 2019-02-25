/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
    CSysFsProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
        TForteByte *paFBConnData, TForteByte *paFBVarsData);
    virtual ~CSysFsProcessInterface();

    class CIOHandler : public CExternalEventHandler, public CThread {
      DECLARE_HANDLER(CIOHandler)
        ;
      public:
        virtual void run();
        void updateReadData();
        void registerIXFB(CSysFsProcessInterface *paFB);
        void unregisterIXFB(CSysFsProcessInterface *paFB);
        /* functions needed for the external event handler interface */
        void enableHandler(void);
        void disableHandler(void);
        void setPriority(int paPriority);
        int getPriority(void) const;

      private:
        typedef CSinglyLinkedList<CSysFsProcessInterface *> TReadFBContainer;
        TReadFBContainer mReadFBList;
        CSyncObject mReadFBListSync;
    };

  protected:
    bool initialise(bool paIsInput);
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

    static const char * const scmOK;
    static const char * const scmPinInUse;
    static const char * const scmNotInitialised;
    static const char * const scmError;
    static const char * const scmCouldNotRead;
    static const char * const scmCouldNotWrite;
};

#endif /* PROCESSINTERFACE_H_ */
