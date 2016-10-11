/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
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
#include <extevhan.h>
#include <thread.h>
#include <singlet.h>

class CPiFaceProcessInterface : public CProcessInterfaceBase{
  public:
    CPiFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
        TForteByte *paFBVarsData);

    virtual ~CPiFaceProcessInterface();

  protected:
    bool initialise(bool paInput);
    bool deinitialise();
    bool readPin();
    bool writePin();

    int mPin;

  private:
    bool checkInputData(long paValue);
    bool getValue(long paValue);

    class CPiFaceIOHandler : public CExternalEventHandler, public CThread{
      DECLARE_SINGLETON(CPiFaceIOHandler)

      public:
        void registerIXFB(CPiFaceProcessInterface *pa_poFB);
        void unregisterIXFB(CPiFaceProcessInterface *pa_poFB);

        void updateReadData(TForteUInt8 paInBuffer);
        void updateWriteData(bool paValue, int paPin);

        /* functions needed for the external event handler interface */
        void enableHandler(void);
        void disableHandler(void);
        void setPriority(int pa_nPriority);
        int getPriority(void) const;

      private:
        typedef CSinglyLinkedList<CPiFaceProcessInterface *> TReadFBContainer;
        TReadFBContainer m_lstReadFBList;
        CSyncObject m_oReadFBListSync;
        TForteUInt8 mOutBuffer;

        virtual void run();

    };

};

//tell the IX and QX FB that this is the process interface to be used
typedef CPiFaceProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
