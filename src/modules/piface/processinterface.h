/*******************************************************************************
 * Copyright (c) 2016, 2021 fortiss GmbH, HIT robot group
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Tibalt Zhao - adapt to new handler infrastructure
 *******************************************************************************/

#ifndef PROCESSINTERFACE_H_
#define PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <extevhan.h>
#include <forte_thread.h>
#include "../conmeleon_c1/spi/spidevice.h"

class CPiFaceProcessInterface : public CProcessInterfaceBase{
  public:
    CPiFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
        TForteByte *paFBVarsData);

    virtual ~CPiFaceProcessInterface();

  friend class CDeviceExecution;

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
        DECLARE_HANDLER(CPiFaceIOHandler)

      public:
        void registerIXFB(CPiFaceProcessInterface *paFB);
        void unregisterIXFB(CPiFaceProcessInterface *paFB);

        void updateReadData(TForteUInt8 paInBuffer);
        void updateWriteData(bool paValue, int paPin);

        /* functions needed for the external event handler interface */
        void enableHandler(void);
        void disableHandler(void);
        void setPriority(int paPriority);
        int getPriority(void) const;

      private:
        typedef CSinglyLinkedList<CPiFaceProcessInterface *> TReadFBContainer;

        static const int scmPiFaceWrite = 0x40;
        static const int scmPiFaceRead = 0x41;

        enum EPiFaceRegister{
          eIODirectionPortA = 0x00,
          eIODirectionPortB = 0x01,
          eIOConfiguration =  0x0A,
          eGPIOPortA = 0x12,
          eGPIOPortB = 0x13,
          eGPIOPullupResistorsPortA = 0x0C,
          eGPIOPullupResistorsPortB = 0x0D
        };

        TReadFBContainer mReadFBList;
        CSyncObject mReadFBListSync;
        TForteUInt8 mOutBuffer;

        virtual void run();

        static void setupPiFaceIOChip(CONMELEON::CSpiDevice &paDev);
        static TForteByte readInputs(CONMELEON::CSpiDevice &paDev);
        static void writePiFaceRegister(CONMELEON::CSpiDevice &paDev, EPiFaceRegister paRegister, TForteByte paValue);

    };

};

//tell the IX and QX FB that this is the process interface to be used
typedef CPiFaceProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
