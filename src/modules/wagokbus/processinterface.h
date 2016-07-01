/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Milan Vathoopan, Alois Zoitl - initial API and implementation and/or initial documentation
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
		// find where exaclty are these variables applied
		//Valid device ids are always greater than 0
		typedef CSinglyLinkedList<WagoPFCProcessInterface *> TReadFBContainer;
		TReadFBContainer m_lstReadFBList;
		CSyncObject m_oReadFBListSync;
		static const unsigned int scm_unMaxTerminalInputData = 16;   //!< max number of bytes of input data per terminal
		//Data types for KBus Search
		static const tDeviceId scm_nInvalidDeviceId = -1;
		tApplicationDeviceInterface * m_pstAppDevInterface;
		tDeviceInfo deviceList[10]; // the list of devices given by the ADI
		size_t nrDevicesFound;
		size_t nrKbusFound;
		uint32_t m_unTaskId;
		tDeviceId m_nKBusDeviceId;
		//KBusConfiguration
		tApplicationStateChangedEvent stEvent;
		/*KBus Terminal information */
		size_t m_unTerminalCount;
		u16 m_aunTerminalIds[LDKC_KBUS_TERMINAL_COUNT_MAX];
		tldkc_KbusInfo_TerminalInfo m_astTerminalDescription[LDKC_KBUS_TERMINAL_COUNT_MAX];

	// Private methods
		void updateReadData();
		void closeKBusInterface();
		bool triggerKBusCycle();
		bool loadTerminalInformation();
		bool checkTerminalId(u16 pa_unTerminalId);

	public:
		// KBusHanlder ;Public meethod
		//!KBus interface handling is up and running correctly
		bool isKBusRunning();
		bool onKBusCylce(WagoPFCProcessInterface &pa_roKBusHandler);
		bool  getTerminalId(TForteUInt8 pa_unSlot);
		tldkc_KbusInfo_TerminalInfo *getTerminalInfo(TForteUInt8 pa_unSlot);
		void registerKBusReadFB(WagoPFCProcessInterface *pa_poFB);
		void unregisterKBusReadFB(WagoPFCProcessInterface *pa_poFB);
		void writeOutputDataToKBus( TForteUInt32 paPin, bool pa_acData);
		void readInputDatafromKBus( TForteUInt32 paPin, bool *pa_acIndata);
		TForteByte *readInputDataFromKBus(tldkc_KbusInfo_TerminalInfo *pa_pstTerminal);
		virtual void run();
		/*!Go through the read list notifying the registered FBs on the new cycle allowing
			* them to update their data and if necessary activate an event chain
			*/

		/* functions needed for the external event handler interface */
		void enableHandler(void);
		void disableHandler(void);
		void setPriority(int pa_nPriority);
		int getPriority(void) const;
      };
	// Private data of WagoPFCProcessinterface
    bool m_bInitialized;
    void allocateIOData();
    bool getValue(long paValue);
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
    TForteUInt32 mPin;
    tldkc_KbusInfo_TerminalInfo *m_pstTerminalInfo;
    bool pa_acIndata;
    // Wago PFCprocess interafec protected methods
    bool initialise(bool paInput);
    bool deinitialise();
    bool readPin();
    bool writePin();

};

      //tell the IX and QX FB that this is the process interface to be used
typedef WagoPFCProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
