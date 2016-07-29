/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Milan Vathoopan, Alois Zoitl - initial API and implementation and/or initial documentation
 *    Milan Vathoopan - Extended functionality with anlog input and output blocks
 *******************************************************************************/
#ifndef PROCESSINTERFACE_H_
#define PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <extevhan.h>
#include <thread.h>
#include <sync.h>
#include <fortelist.h>
#include <singlet.h>
#include <esfb.h>
#include <devexec.h>
#include <devlog.h>
#include <unistd.h>
#include <vector>
#include <string>


#define OS_MUST_BE_ARRAY

extern "C" {
#include <dal/adi_application_interface.h>
#include <ldkc_kbus_information.h>
#include <ldkc_kbus_register_communication.h>
}

class WagoPFCProcessInterface : public CProcessInterfaceBase{
  
private:
class CKBusHandler : public CExternalEventHandler, public CThread{
DECLARE_SINGLETON(CKBusHandler)

  private:
		//Valid device ids are always greater than 0
		typedef CSinglyLinkedList<WagoPFCProcessInterface *> TReadFBContainer;
		TReadFBContainer mReadFBList;
		CSyncObject mReadFBListSync;
		//static const unsigned int scm_MaxTerminalInputData = 16;   //!< max number of bytes of input data per terminal: not used as only works on a sinlgekBUS device
		//Data types for KBus Search
		static const tDeviceId scmInvalidDeviceId = -1;
		tApplicationDeviceInterface * mAppDevInterface;
		tDeviceInfo deviceList[10]; // the list of devices given by the ADI
		size_t nrDevicesFound;
		//size_t nrKbusFound;
		uint32_t mTaskId;
		tDeviceId mKBusDeviceId;
		//KBusConfiguration
		tApplicationStateChangedEvent stEvent;
		/*KBus Terminal information */
		size_t mTerminalCount;
		u16 mTerminalIds[LDKC_KBUS_TERMINAL_COUNT_MAX];
		tldkc_KbusInfo_TerminalInfo mTerminalDescription[LDKC_KBUS_TERMINAL_COUNT_MAX];

	// Private methods
		void updateReadData();
		void closeKBusInterface();
		bool triggerKBusCycle();
		bool loadTerminalInformation();

	public:
		// KBusHanlder ;Public meethod
		//!KBus interface handling is up and running correctly
		bool isKBusRunning();
		//bool onKBusCylce(WagoPFCProcessInterface &pa_roKBusHandler); can be used for more complex kbus handling
		bool  getTerminalId(TForteUInt8 paSlot);
		tldkc_KbusInfo_TerminalInfo *getTerminalInfo(TForteUInt8 paSlot);
		void registerKBusReadFB(WagoPFCProcessInterface *paFB);
		void unregisterKBusReadFB(WagoPFCProcessInterface *paFB);
		void writeOutputDataBitToKBus( tldkc_KbusInfo_TerminalInfo *paTerminal, TForteUInt32 , bool paOutDataBool);
		void writeOutputDataWordToKBus(tldkc_KbusInfo_TerminalInfo *paTerminal, TForteUInt32 paChannel, TForteWord paOutDataWord);
		void readInputDataBitfromKBus( tldkc_KbusInfo_TerminalInfo *paTerminal, TForteUInt32 paChannel, bool *paInDataBool);
		void readInputDataWordfromKBus( tldkc_KbusInfo_TerminalInfo *paTerminal, TForteUInt32 paChannel, TForteWord *paInDataWord);
		virtual void run();
		/*!Go through the read list notifying the registered FBs on the new cycle allowing
			* them to update their data and if necessary activate an event chain*/

		/* functions needed for the external event handler interface */
		void enableHandler(void);
		void disableHandler(void);
		void setPriority(int paPriority);
		int getPriority(void) const;
      };
	// Private data of WagoPFCProcessinterface
    bool mInitialized;
    bool checkInputData();
    std::vector<std::string> generateParameterList();
public:
    // wago PFC process interface declaration
    WagoPFCProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
    		const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
virtual ~WagoPFCProcessInterface();

protected:
    // Protected member variables
  TForteUInt32 mSlot;
  TForteUInt32 mChannel;
  tldkc_KbusInfo_TerminalInfo *mTerminalInfo;
  bool mInDataBool;
  TForteWord mInDataWord;

    // Wago PFCprocess interafec protected methods
  bool initialise(bool paInput);
  bool deinitialise();
  bool readPin();
  bool writePin();
  bool readWord();
  bool writeWord();

};

      //tell the IX and QX FB that this is the process interface to be used
typedef WagoPFCProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
