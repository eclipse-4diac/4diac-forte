/*******************************************************************************
 * Copyright (c) 2012 - 2024 AIT, ACIN, fortiss
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl, Ewald Weinhandl - initial API and implementation and/or initial documentation
 *   Michael Gafert - renamed variables to match powerlink library
 *******************************************************************************/
#include "EplWrapper.h"
#include "ProcessImageMatrix.h"
#include "EplXmlReader.h"
#include <forte_thread.h>

#if (TARGET_SYSTEM == _WIN32_)
#define _WINSOCKAPI_ // prevent windows.h from including winsock.h
#endif  // (TARGET_SYSTEM == _WIN32_)
/* includes */
#include "Epl.h"

#undef EPL_STACK_VERSION
#define EPL_STACK_VERSION(ver,rev,rel)      (((((ver)) & 0xFF)<<24)|((((rev))&0xFF)<<16)|(((rel))&0xFFFF))

#if (TARGET_SYSTEM == _LINUX_)
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <errno.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>

#ifndef CONFIG_POWERLINK_USERSTACK
#include <pthread.h>
#else
#include <pcap.h>
#endif

#elif (TARGET_SYSTEM == _WIN32_)
#include <Iphlpapi.h>
#include <pcap.h>
#endif  // (TARGET_SYSTEM == _WIN32_)
#include <EplTgtConio.h>
//#include <conio.h>

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*          G L O B A L   D E F I N I T I O N S                            */
/*                                                                         */
/*                                                                         */
/***************************************************************************/

//---------------------------------------------------------------------------
// const defines
//---------------------------------------------------------------------------
#if (TARGET_SYSTEM == _LINUX_)

#define SET_CPU_AFFINITY
#define MAIN_THREAD_PRIORITY            20

#elif (TARGET_SYSTEM == _WIN32_)

// TracePoint support for realtime-debugging
#ifdef _DBG_TRACE_POINTS_
void PUBLIC TgtDbgSignalTracePoint (BYTE bTracePointNumber_p);
#define TGT_DBG_SIGNAL_TRACE_POINT(p)   TgtDbgSignalTracePoint(p)
#else
#define TGT_DBG_SIGNAL_TRACE_POINT(p)
#endif

#endif // (TARGET_SYSTEM == _WIN32_)
const DWORD NODEID = 0xF0; //=> MN
const DWORD IP_ADDR = 0xc0a86401; // 192.168.100.1
const DWORD SUBNET_MASK = 0xFFFFFF00; // 255.255.255.0
const char *HOSTNAME = "EPL Stack";

//---------------------------------------------------------------------------
// module global vars
//---------------------------------------------------------------------------

CONST BYTE abMacAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned int uiNodeId_g = EPL_C_ADR_INVALID;

static tEplApiProcessImageCopyJob AppProcessImageCopyJob_g;

static bool waitingUntilOperational;

#ifdef CONFIG_POWERLINK_USERSTACK

//static char* pszCdcFilename_g = "mnobd.cdc";

#else

static pthread_t eventThreadId;
static pthread_t syncThreadId;

void *powerlinkEventThread(void * arg);
void *powerlinkSyncThread(void * arg);

#endif

//---------------------------------------------------------------------------
// local function prototypes
//---------------------------------------------------------------------------

// This function is the entry point for your object dictionary. It is defined
// in OBJDICT.C by define EPL_OBD_INIT_RAM_NAME. Use this function name to define
// this function prototype here. If you want to use more than one Epl
// instances then the function name of each object dictionary has to differ.

extern "C" tEplKernel PUBLIC EplObdInitRam(tEplObdInitParam MEM*paInitParam);

tEplKernel PUBLIC appCbEvent(
  tEplApiEventType paEventType, // IN: event type (enum)
  tEplApiEventArg *paEventArg, // IN: event argument (union)
  void GENERIC*paUserArg);

tEplKernel PUBLIC appCbSync();

char *CEplStackWrapper::allocProcImage(unsigned int pa_nNumOfBytes) {
  auto procImage = static_cast<char *>(malloc(pa_nNumOfBytes));
  for (unsigned int i = 0; i < pa_nNumOfBytes; i++) {
    procImage[i] = 0x00;
  }

  return procImage;
}

//=========================================================================//
//                                                                         //
//          S T A T I C   F U N C T I O N S                                //
//                                                                         //
//=========================================================================//

DEFINE_SINGLETON(CEplStackWrapper);

void CEplStackWrapper::eplMainInit() {
#if (TARGET_SYSTEM == _LINUX_)
  sigset_t mask;

  /*
   * We have to block the real time signals used by the timer modules so
   * that they are able to wait on them using sigwaitinfo!
   */
  sigemptyset(&mask);
  sigaddset(&mask, SIGRTMIN);
  sigaddset(&mask, SIGRTMIN + 1);
  pthread_sigmask(SIG_BLOCK, &mask, nullptr);
#endif
}

//=========================================================================//
//                                                                         //
//          C L A S S   F U N C T I O N S                                  //
//                                                                         //
//=========================================================================//

CEplStackWrapper::CEplStackWrapper() {
}

CEplStackWrapper::~CEplStackWrapper() {
}

int CEplStackWrapper::eplStackInit(const char *pa_pXmlFile, const char *pa_pCdcFile, const char *pa_pEthDeviceName) {
  tEplKernel ret;
  static tEplApiInitParam eplApiInitParam;
  const char *hostname = HOSTNAME;

  // Read and process XML file
  CEplXmlReader xmlReader(&mProcMatrixIn, &mProcMatrixOut);
  xmlReader.readXmlFile(pa_pXmlFile);

  mProcInSize = mProcMatrixIn.getProcessImageSize();
  mProcOutSize = mProcMatrixOut.getProcessImageSize();

  mAppProcessImageIn = allocProcImage(mProcInSize);
  mAppProcessImageOut = allocProcImage(mProcOutSize);

#ifdef CONFIG_POWERLINK_USERSTACK
#if (TARGET_SYSTEM == _LINUX_)
  struct sched_param schedParam;
#endif

  // variables for Pcap
  pcap_if_t *alldevs;
  pcap_if_t *seldev;
  int i = 0;
#endif

#ifdef CONFIG_POWERLINK_USERSTACK

#if (TARGET_SYSTEM == _LINUX_)
  /* adjust process priority */
  if (nice(-20) == -1) // push nice level in case we have no RTPreempt
  {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
  EPL_DBGLVL_ERROR_TRACE("%s() couldn't set nice value! (%s)\n", __func__, strerror(errno));
#else
    EPL_DBGLVL_ERROR_TRACE2("%s() couldn't set nice value! (%s)\n", __func__, strerror(errno));
#endif
  }
  schedParam.__sched_priority = MAIN_THREAD_PRIORITY;
  if (pthread_setschedparam(pthread_self(), SCHED_RR, &schedParam) != 0) {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
  EPL_DBGLVL_ERROR_TRACE("%s() couldn't set thread scheduling parameters! %d\n", __func__, schedParam.__sched_priority);
#else
    EPL_DBGLVL_ERROR_TRACE2("%s() couldn't set thread scheduling parameters! %d\n", __func__,
                schedParam.__sched_priority);
#endif
  }

  /* Initialize target specific stuff */
  // EplTgtInit();
#elif (TARGET_SYSTEM == _WIN32_)

  // activate realtime priority class
  SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
  // lower the priority of this thread
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);

#endif // (TARGET_SYSTEM == _WIN32_)
#endif // CONFIG_POWERLINK_USERSTACK
  /* Enabling ftrace for debugging */
  FTRACE_OPEN();
  FTRACE_ENABLE(TRUE);

  EPL_MEMSET(&eplApiInitParam, 0, sizeof(eplApiInitParam));
  eplApiInitParam.m_uiSizeOfStruct = sizeof(eplApiInitParam);

#ifdef CONFIG_POWERLINK_USERSTACK

  ////////////////////////////////////////////////////////////////////////////////
  // Find ETH card specified by user //
  ////////////////////////////////////////////////////////////////////////////////

  char correctDevName[1024];
  bool macFound = findMAC(pa_pEthDeviceName, &correctDevName[0]);

  /* Retrieve the device list on the local machine */

  if (char err[PCAP_ERRBUF_SIZE]; pcap_findalldevs(&alldevs, err) == -1) {
    DEVLOG_ERROR("[powerlink] Error in pcap_findalldevs: %s\n", err);
    ret = kEplNoResource;
    return ret;
  }

  for (seldev = alldevs, i = 0; seldev != nullptr; seldev = seldev->next, i++) {
    if (seldev->description) {
      DEVLOG_INFO("[powerlink] %d: %s\n      %s\n", i, seldev->description, seldev->name);
    } else {
      DEVLOG_INFO("[powerlink] %d: %s\n", i, seldev->name);
    }

    if (macFound) {
      const char *userDescLoc = strstr(seldev->name, correctDevName);
      if (userDescLoc != nullptr) {
        if (seldev->description) {
          DEVLOG_INFO("[powerlink] Chosen Ethernet Card: %s\n      %s\n", seldev->description, seldev->name);
        } else {
          DEVLOG_INFO("[powerlink] Chosen Ethernet Card: %s\n", seldev->name);
        }
        break;
      }
    }

    if (seldev->description) {
      const char *userDescLoc = strstr(seldev->description, pa_pEthDeviceName);
      if (userDescLoc != nullptr) {
        DEVLOG_INFO("[powerlink] Chosen Ethernet Card: %s\n", seldev->description);
        break;
      }
    } else {
      const char *userDescLoc = strstr(seldev->name, pa_pEthDeviceName);
      if (userDescLoc != nullptr) {
        DEVLOG_INFO("[powerlink] Chosen Ethernet Card: %s\n", seldev->name);
        break;
      }
    }
  }

  // Check if a device was found, otherwise shutdown stack
  if (!seldev) {
    DEVLOG_ERROR("[powerlink] %s(Err/Warn): Invalid MAC address or device name specified. Shutting down Powerlink stack\n",
        __func__);
    ret = kEplNoResource;
    return ret;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Setup EplApiInitParam (some of them can be removed as we have obd?) //
  ////////////////////////////////////////////////////////////////////////////////

  // pass selected device name to Edrv
  char devName[128];
  strncpy(devName, seldev->name, 127);
  eplApiInitParam.m_HwParam.m_pszDevName = devName;

#endif

  eplApiInitParam.m_uiNodeId = uiNodeId_g = NODEID;
  eplApiInitParam.m_dwIpAddress = (0xFFFFFF00 & IP_ADDR) | eplApiInitParam.m_uiNodeId;

  /* write 00:00:00:00:00:00 to MAC address, so that the driver uses the real hardware address */
  EPL_MEMCPY(eplApiInitParam.m_abMacAddress, abMacAddr, sizeof(eplApiInitParam.m_abMacAddress));

  eplApiInitParam.m_fAsyncOnly = FALSE;

  eplApiInitParam.m_dwFeatureFlags = -1;
  eplApiInitParam.m_dwCycleLen = 10000; // required for error detection
  eplApiInitParam.m_uiIsochrTxMaxPayload = 256; // const
  eplApiInitParam.m_uiIsochrRxMaxPayload = 256; // const
  eplApiInitParam.m_dwPresMaxLatency = 50000; // const; only required for IdentRes
  eplApiInitParam.m_uiPreqActPayloadLimit = 36; // required for initialisation (+28 bytes)
  eplApiInitParam.m_uiPresActPayloadLimit = 36; // required for initialisation of Pres frame (+28 bytes)
  eplApiInitParam.m_dwAsndMaxLatency = 150000; // const; only required for IdentRes
  eplApiInitParam.m_uiMultiplCycleCnt = 0; // required for error detection
  eplApiInitParam.m_uiAsyncMtu = 1500; // required to set up max frame size
  eplApiInitParam.m_uiPrescaler = 2; // required for sync
  eplApiInitParam.m_dwLossOfFrameTolerance = 500000;
  eplApiInitParam.m_dwAsyncSlotTimeout = 3000000;
  eplApiInitParam.m_dwWaitSocPreq = 150000;
  eplApiInitParam.m_dwDeviceType = -1; // NMT_DeviceType_U32
  eplApiInitParam.m_dwVendorId = -1; // NMT_IdentityObject_REC.VendorId_U32
  eplApiInitParam.m_dwProductCode = -1; // NMT_IdentityObject_REC.ProductCode_U32
  eplApiInitParam.m_dwRevisionNumber = -1; // NMT_IdentityObject_REC.RevisionNo_U32
  eplApiInitParam.m_dwSerialNumber = -1; // NMT_IdentityObject_REC.SerialNo_U32

  eplApiInitParam.m_dwSubnetMask = SUBNET_MASK;
  eplApiInitParam.m_dwDefaultGateway = 0;
  EPL_MEMCPY(eplApiInitParam.m_sHostname, hostname, sizeof(eplApiInitParam.m_sHostname));
  eplApiInitParam.m_uiSyncNodeId = EPL_C_ADR_SYNC_ON_SOA;
  eplApiInitParam.m_fSyncOnPrcNode = FALSE;

  // set callback functions
  eplApiInitParam.m_pfnCbEvent = appCbEvent;

#ifdef CONFIG_POWERLINK_USERSTACK
  eplApiInitParam.m_pfnObdInitRam = EplObdInitRam;
  eplApiInitParam.m_pfnCbSync = appCbSync;
#else
  EplApiInitParam.m_pfnCbSync = nullptr;
#endif
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // Initialize Powerlink Stack //
  ////////////////////////////////////////////////////////////////////////////////
  DEVLOG_INFO("[powerlink] Powerlink %s running. (build: %s / %s) \n",
       (NODEID == EPL_C_ADR_MN_DEF_NODE_ID ? "Managing Node" : "Controlled Node"), __DATE__, __TIME__);

  // initialize POWERLINK stack
  ret = EplApiInitialize(&eplApiInitParam);
  if (ret != kEplSuccessful) {
    DEVLOG_ERROR("[powerlink] Could not initialize POWERLINK stack at EplApiInitialize\n");
    return ret;
  }

#ifdef CONFIG_POWERLINK_USERSTACK
  /* At this point, we don't need any more the device list. Free it */
  pcap_freealldevs(alldevs);

  ret = EplApiSetCdcFilename(const_cast<char*>(pa_pCdcFile));
  if (ret != kEplSuccessful) {
    DEVLOG_ERROR("[powerlink] Could not set CDC filename at EplApiSetCdcFilename\n", pa_pCdcFile);
    return ret;
  }
#else
  // create event thread
  if(pthread_create(&eventThreadId, nullptr, &powerlinkEventThread, nullptr) != 0){
  return ret;
  }

  // create sync thread
  if(pthread_create(&syncThreadId, nullptr, &powerlinkSyncThread, nullptr) != 0){
  return ret;
  }
#endif

  AppProcessImageCopyJob_g.m_fNonBlocking = FALSE;
  AppProcessImageCopyJob_g.m_uiPriority = 0;
  AppProcessImageCopyJob_g.m_In.m_pPart = mAppProcessImageIn;
  AppProcessImageCopyJob_g.m_In.m_uiOffset = 0;
  AppProcessImageCopyJob_g.m_In.m_uiSize = mProcInSize;
  AppProcessImageCopyJob_g.m_Out.m_pPart = mAppProcessImageOut;
  AppProcessImageCopyJob_g.m_Out.m_uiOffset = 0;
  AppProcessImageCopyJob_g.m_Out.m_uiSize = mProcOutSize;

  ret = EplApiProcessImageAlloc(mProcInSize, mProcOutSize, 2, 2);
  if (ret != kEplSuccessful) {
    DEVLOG_ERROR("[powerlink] Could not allocate process image at EplApiProcessImageAlloc\n");
    eplStackShutdown();
  }

  ret = EplApiProcessImageSetup();
  if (ret != kEplSuccessful) {
    DEVLOG_ERROR("[powerlink] Could not setup process image at EplApiProcessImageSetup\n");
    eplStackShutdown();
  }

  // start processing
  ret = EplApiExecNmtCommand(kEplNmtEventSwReset);
  if (ret != kEplSuccessful) {
    DEVLOG_ERROR("[powerlink] Could not start processing at EplApiExecNmtCommand\n");
    eplStackShutdown();
  }

  waitingUntilOperational = false;
  if (mInitWait == true) {
    while (!waitingUntilOperational) {
      // Waiting
      CThread::sleepThread(1);
    }
  }

  return ret;
}

////////////////////////////////////////////////////////////////////////////////
// Stop the stack //
////////////////////////////////////////////////////////////////////////////////
int CEplStackWrapper::eplStackShutdown() {
  // halt the NMT state machine
  // so the processing of POWERLINK frames stops
  EplApiExecNmtCommand(kEplNmtEventSwitchOff);

  // delete process image
  EplApiProcessImageFree();

  // delete instance for all modules
  tEplKernel eplRet = EplApiShutdown();
  DEVLOG_INFO("EplApiShutdown():  0x%X\n", eplRet);

  mProcMatrixIn.clearAll();
  mProcMatrixOut.clearAll();
  mCallbackList.clearAll();
  free(mAppProcessImageIn);
  free(mAppProcessImageOut);

  return eplRet;
}

CProcessImageMatrix *CEplStackWrapper::getProcessImageMatrixIn() {
  return &mProcMatrixIn;
}

CProcessImageMatrix *CEplStackWrapper::getProcessImageMatrixOut() {
  return &mProcMatrixOut;
}

char *CEplStackWrapper::getProcImageIn() {
  return mAppProcessImageIn;
}

char *CEplStackWrapper::getProcImageOut() {
  return mAppProcessImageOut;
}

void CEplStackWrapper::waitUntilOperational(bool pa_bWait) {
  mInitWait = pa_bWait;
}

void CEplStackWrapper::registerCallback(IEplCNCallback *pa_pCallback) {
  mSync.lock();
  mCallbackList.pushBack(pa_pCallback);
  mSync.unlock();
}

bool CEplStackWrapper::findMAC(const char *pa_pUserMAC, char *pa_pDeviceName) {
  //char* correctDevName;

#if (TARGET_SYSTEM == _LINUX_)
  int socketDescriptor;
  struct ifreq ifReq; // Interface request
  struct if_nameindex *ifList; // Ptr to interface name index
  struct if_nameindex *listSave; // Ptr to interface name index

#ifndef SIOCGIFADDR
  // The kernel does not support the required ioctls
  return (false);
#endif

  //
  // Create a socket that we can use for all of our ioctls
  //
  socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
  if (socketDescriptor < 0) {
    // Socket creation failed, this is a fatal error
    DEVLOG_ERROR("File %s: line %d: Socket failed\n", __FILE__, __LINE__);
    return (0);
  }

  //
  // Obtain a list of dynamically allocated structures
  //
  ifList = listSave = if_nameindex();

  //
  // Walk thru the array returned and query for each interface's
  // address
  //
  for (ifList; *(char *) ifList != 0; ifList++) {
    strncpy(ifReq.ifr_name, ifList->if_name, IF_NAMESIZE);

    //
    // Get the MAC address for this interface
    //
    if (ioctl(socketDescriptor, SIOCGIFHWADDR, &ifReq) != 0) {
      // We failed to get the MAC address for the interface
      DEVLOG_ERROR("File %s: line %d: Ioctl failed\n", __FILE__, __LINE__);
      return false;
    }

    //
    // Determine if we are processing the interface that we
    // are interested in
    //
    char chMAC[6 * 2 + 5 + 2];
    sprintf(chMAC, "%02X-%02X-%02X-%02X-%02X-%02X", (unsigned char) ifReq.ifr_hwaddr.sa_data[0],
        (unsigned char) ifReq.ifr_hwaddr.sa_data[1], (unsigned char) ifReq.ifr_hwaddr.sa_data[2],
        (unsigned char) ifReq.ifr_hwaddr.sa_data[3], (unsigned char) ifReq.ifr_hwaddr.sa_data[4],
        (unsigned char) ifReq.ifr_hwaddr.sa_data[5]);

    if (compareMACs(chMAC, pa_pUserMAC)) {
      strncpy(pa_pDeviceName, ifList->if_name, IF_NAMESIZE);

      //
      // Clean up things and return
      //
      if_freenameindex(listSave);
      close(socketDescriptor);

      return true;
    }
  }

  //
  // Clean up things and return
  //
  if_freenameindex(listSave);
  close(socketDescriptor);

#elif (TARGET_SYSTEM == _WIN32_)

  // Find MAC address
  IP_ADAPTER_INFO AdapterInfo[16];// Allocate information for up to 16 NICs
  DWORD dwBufLen = sizeof(AdapterInfo);// Save memory size of buffer

  DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
  assert(dwStatus == ERROR_SUCCESS);// Verify return value is valid, no buffer overflow

  PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info

  do{
  char* chMAC = new char[6*2+5+1];
  BYTE *macAddr = pAdapterInfo->Address;
  for (int i = 0; i < 6*2+5; i = i+2)
  {
    if (i>0){
    chMAC[i] = '-';
    i++;
    }
    sprintf(&chMAC[i],"%02x",*macAddr++);
  }

  if (compareMACs(chMAC, pa_pchUserMAC)){
    //correctDevName = new char[strlen(pAdapterInfo->AdapterName)+1];
    strcpy(pa_pchDeviceName,pAdapterInfo->AdapterName);
    delete chMAC;

    //deviceName = correctDevName;
    return true;
  }

  pAdapterInfo = pAdapterInfo->Next; // Progress through linked list
  delete chMAC;
  }
  while(pAdapterInfo); // Terminate if last adapter

#endif

  //deviceName = nullptr; //No effect
  return false;
}

bool CEplStackWrapper::compareMACs(const char *pa_pMacA, const char *pa_pMacB) {
  if (strcmp(pa_pMacA, pa_pMacB) == 0) {
    return true;
  }

  auto *macCopyA = new char[strlen(pa_pMacA) + 1];
  strcpy(macCopyA, pa_pMacA);
  auto *macCopyB = new char[strlen(pa_pMacB) + 1];
  strcpy(macCopyB, pa_pMacB);

  // Change to upper case
  for (int i = 0; i < strlen(pa_pMacA); i++) {
    switch (macCopyA[i]) {
      case 'a':
        macCopyA[i] = 'A';
        break;
      case 'b':
        macCopyA[i] = 'B';
        break;
      case 'c':
        macCopyA[i] = 'C';
        break;
      case 'd':
        macCopyA[i] = 'D';
        break;
      case 'e':
        macCopyA[i] = 'E';
        break;
      case 'f':
        macCopyA[i] = 'F';
        break;
    }
  }
  for (int i = 0; i < strlen(pa_pMacB); i++) {
    switch (macCopyB[i]) {
      case 'a':
        macCopyB[i] = 'A';
        break;
      case 'b':
        macCopyB[i] = 'B';
        break;
      case 'c':
        macCopyB[i] = 'C';
        break;
      case 'd':
        macCopyB[i] = 'D';
        break;
      case 'e':
        macCopyB[i] = 'E';
        break;
      case 'f':
        macCopyB[i] = 'F';
        break;
    }
  }

  if (strcmp(macCopyA, macCopyB) == 0) {
    delete[] macCopyA;
    delete[] macCopyB;
    return true;
  }

  delete[] macCopyA;
  delete[] macCopyB;
  return false;
}

//=========================================================================//
//                                                                         //
//          P R I V A T E   F U N C T I O N S                              //
//                                                                         //
//=========================================================================//

//---------------------------------------------------------------------------
//
// Function:    appCbEvent
//
// Description: event callback function called by EPL API layer within
//              user part (low priority).
//
// Parameters:  eventType     = event type
//              eventArg     = pointer to union, which describes
//                                the event in detail
//              userArg      = user specific argument
//
// Returns:     tEplKernel      = error code,
//                                kEplSuccessful = no error
//                                kEplReject = reject further processing
//                                otherwise = post error event to API layer
//
// State:
//
//---------------------------------------------------------------------------

tEplKernel PUBLIC appCbEvent(
  tEplApiEventType paEventType, // IN: event type (enum)
  tEplApiEventArg *paEventArg, // IN: event argument (union)
  void GENERIC* paUserArg) {
  tEplKernel ret = kEplSuccessful;

  UNUSED_PARAMETER(userArg);

  // check if NMT_GS_OFF is reached
  switch (paEventType) {
    case kEplApiEventNmtStateChange: {
      switch (paEventArg->m_NmtStateChange.m_NewNmtState) {
        case kEplNmtGsOff: {
          // NMT state machine was shut down,
          // because of user signal (CTRL-C) or critical EPL stack error
          // -> also shut down EplApiProcess() and main()
          ret = kEplShutdown;
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(kEplNmtGsOff) originating event = 0x%X\n", __func__, eventArg->m_NmtStateChange.m_NmtEvent);
#else
          DEVLOG_DEBUG("[powerlink] %s(kEplNmtGsOff) originating event = 0x%X\n", __func__,
              paEventArg->m_NmtStateChange.m_NmtEvent);
#endif
          break;
        }

        case kEplNmtGsResetCommunication: {
          // continue
        }

        case kEplNmtGsResetConfiguration: {
          // continue
        }

        case kEplNmtMsPreOperational1: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(0x%X) originating event = 0x%X\n",
        __func__,
        eventArg->m_NmtStateChange.m_NewNmtState,
        eventArg->m_NmtStateChange.m_NmtEvent);
#else
          DEVLOG_DEBUG("[powerlink] %s(0x%X) originating event = 0x%X\n",
              __func__,
              paEventArg->m_NmtStateChange.m_NewNmtState,
              paEventArg->m_NmtStateChange.m_NmtEvent);
#endif

          // continue
        }

        case kEplNmtGsInitialising:
        case kEplNmtGsResetApplication:
        case kEplNmtMsNotActive:
        case kEplNmtCsNotActive:
        case kEplNmtCsPreOperational1: {
          break;
        }
        case kEplNmtCsOperational:
        case kEplNmtMsOperational: {
          break;
        }
        default: {
          break;
        }
      }

      break;
    }

    case kEplApiEventCriticalError:
    case kEplApiEventWarning: {
      // error or warning occured within the stack or the application
      // on error the API layer stops the NMT state machine
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
    DEVLOG_DEBUG("[powerlink] %s(Err/Warn): Source=%02X EplError=0x%03X\n",
      __func__,
      eventArg->m_InternalError.m_EventSource,
      eventArg->m_InternalError.m_EplError);
#else
      DEVLOG_DEBUG("[powerlink] %s(Err/Warn): Source=%02X EplError=0x%03X",
          __func__,
          paEventArg->m_InternalError.m_EventSource,
          paEventArg->m_InternalError.m_EplError);
#endif
      // check additional argument
      switch (paEventArg->m_InternalError.m_EventSource) {
        case kEplEventSourceEventk:
        case kEplEventSourceEventu: {
          // error occured within event processing
          // either in kernel or in user part
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG(" OrgSource=%02X\n", eventArg->m_InternalError.m_Arg.m_EventSource);
#else
          DEVLOG_DEBUG(" OrgSource=%02X\n", paEventArg->m_InternalError.m_Arg.m_EventSource);
#endif
          break;
        }

        case kEplEventSourceDllk: {
          // error occured within the data link layer (e.g. interrupt processing)
          // the DWORD argument contains the DLL state and the NMT event
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG(" val=%lX\n", eventArg->m_InternalError.m_Arg.m_dwArg);
#else
          DEVLOG_DEBUG(" val=%lX\n", paEventArg->m_InternalError.m_Arg.m_dwArg);
#endif
          break;
        }

        case kEplEventSourceObdk:
        case kEplEventSourceObdu: {
          // error occured within OBD module
          // either in kernel or in user part
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG(" Object=0x%04X/%u\n", eventArg->m_InternalError.m_Arg.m_ObdError.m_uiIndex, eventArg->m_InternalError.m_Arg.m_ObdError.m_uiSubIndex);
#else
          DEVLOG_DEBUG(" Object=0x%04X/%u\n", paEventArg->m_InternalError.m_Arg.m_ObdError.m_uiIndex,
              paEventArg->m_InternalError.m_Arg.m_ObdError.m_uiSubIndex);
#endif
          break;
        }

        default: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("\n");
#else
          DEVLOG_DEBUG("\n");
#endif
          break;
        }
      }
      break;
    }

    case kEplApiEventHistoryEntry: {
      // new history entry
      DEVLOG_DEBUG("[powerlink] %s(HistoryEntry): Type=0x%04X Code=0x%04X (0x%02X %02X %02X %02X %02X %02X %02X %02X)\n",
           __func__,
           paEventArg->m_ErrHistoryEntry.m_wEntryType,
           paEventArg->m_ErrHistoryEntry.m_wErrorCode,
           (WORD) paEventArg->m_ErrHistoryEntry.m_abAddInfo[0],
           (WORD) paEventArg->m_ErrHistoryEntry.m_abAddInfo[1],
           (WORD) paEventArg->m_ErrHistoryEntry.m_abAddInfo[2],
           (WORD) paEventArg->m_ErrHistoryEntry.m_abAddInfo[3],
           (WORD) paEventArg->m_ErrHistoryEntry.m_abAddInfo[4],
           (WORD) paEventArg->m_ErrHistoryEntry.m_abAddInfo[5],
           (WORD) paEventArg->m_ErrHistoryEntry.m_abAddInfo[6],
           (WORD) paEventArg->m_ErrHistoryEntry.m_abAddInfo[7]);
      break;
    }

    case kEplApiEventNode: {
      // check additional argument
      switch (paEventArg->m_Node.m_NodeEvent) {
        case kEplNmtNodeEventCheckConf: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, CheckConf)\n", __func__, eventArg->m_Node.m_uiNodeId);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, CheckConf)\n", __func__, paEventArg->m_Node.m_uiNodeId);
#endif
          break;
        }

        case kEplNmtNodeEventUpdateConf: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, UpdateConf)\n", __func__, eventArg->m_Node.m_uiNodeId);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, UpdateConf)\n", __func__, paEventArg->m_Node.m_uiNodeId);
#endif
          break;
        }

        case kEplNmtNodeEventNmtState: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("%s(Node=0x%X, NmtState=0x%X)\n", __func__, eventArg->m_Node.m_uiNodeId, eventArg->m_Node.m_NmtState);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, NmtState=0x%X)\n", __func__, paEventArg->m_Node.m_uiNodeId,
              paEventArg->m_Node.m_NmtState);
#endif
          if (paEventArg->m_Node.m_NmtState == kEplNmtCsOperational) {
            DEVLOG_INFO("[powerlink] Init finished\n");
            waitingUntilOperational = true;
          }
          break;
        }

        case kEplNmtNodeEventError: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, Error=0x%X)\n", __func__, eventArg->m_Node.m_uiNodeId, eventArg->m_Node.m_wErrorCode);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, Error=0x%X)\n", __func__, paEventArg->m_Node.m_uiNodeId,
              paEventArg->m_Node.m_wErrorCode);
#endif
          break;
        }

        case kEplNmtNodeEventFound: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, Found)\n", __func__, eventArg->m_Node.m_uiNodeId);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, Found)\n", __func__, paEventArg->m_Node.m_uiNodeId);
#endif
          break;
        }

        default: {
          break;
        }
      }
      break;
    }

#if (((EPL_MODULE_INTEGRATION) & (EPL_MODULE_CFM)) != 0)
    case kEplApiEventCfmProgress: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
    DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, CFM-Progress: Object 0x%X/%u, ", __func__, eventArg->m_CfmProgress.m_uiNodeId, eventArg->m_CfmProgress.m_uiObjectIndex, eventArg->m_CfmProgress.m_uiObjectSubIndex);
    DEVLOG_DEBUG("%u/%u Bytes", eventArg->m_CfmProgress.m_dwBytesDownloaded, eventArg->m_CfmProgress.m_dwTotalNumberOfBytes);
#else
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, CFM-Progress: Object 0x%X/%u, ", __func__, paEventArg->m_CfmProgress.m_uiNodeId,
          paEventArg->m_CfmProgress.m_uiObjectIndex, paEventArg->m_CfmProgress.m_uiObjectSubIndex);
      DEVLOG_DEBUG("%u/%u Bytes", paEventArg->m_CfmProgress.m_dwBytesDownloaded,
          paEventArg->m_CfmProgress.m_dwTotalNumberOfBytes);
#endif
      if ((paEventArg->m_CfmProgress.m_dwSdoAbortCode != 0)
        || (paEventArg->m_CfmProgress.m_EplError != kEplSuccessful)) {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
    DEVLOG_DEBUG(" -> SDO Abort=0x%lX, Error=0x%X)\n", eventArg->m_CfmProgress.m_dwSdoAbortCode, eventArg->m_CfmProgress.m_EplError);
#else
        DEVLOG_DEBUG(" -> SDO Abort=0x%lX, Error=0x%X)\n", paEventArg->m_CfmProgress.m_dwSdoAbortCode,
            paEventArg->m_CfmProgress.m_EplError);
#endif
      } else {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
    DEVLOG_DEBUG(")\n");
#else
        DEVLOG_DEBUG(")\n");
#endif
      }
      break;
    }

    case kEplApiEventCfmResult: {
      switch (paEventArg->m_CfmResult.m_NodeCommand) {
        case kEplNmtNodeCommandConfOk: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, ConfOk)\n", __func__, eventArg->m_CfmResult.m_uiNodeId);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, ConfOk)\n", __func__, paEventArg->m_CfmResult.m_uiNodeId);
#endif
          break;
        }

        case kEplNmtNodeCommandConfErr: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, ConfErr)\n", __func__, eventArg->m_CfmResult.m_uiNodeId);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, ConfErr)\n", __func__, paEventArg->m_CfmResult.m_uiNodeId);
#endif
          break;
        }

        case kEplNmtNodeCommandConfReset: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, ConfReset)\n", __func__, eventArg->m_CfmResult.m_uiNodeId);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, ConfReset)\n", __func__, paEventArg->m_CfmResult.m_uiNodeId);
#endif
          break;
        }

        case kEplNmtNodeCommandConfRestored: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, ConfRestored)\n", __func__, eventArg->m_CfmResult.m_uiNodeId);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, ConfRestored)\n", __func__, paEventArg->m_CfmResult.m_uiNodeId);
#endif
          break;
        }

        default: {
#if EPL_DEFINED_STACK_VERSION >= EPL_STACK_VERSION(1, 8, 2)
      DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, CfmResult=0x%X)\n", __func__, eventArg->m_CfmResult.m_uiNodeId, pEventArg_p->m_CfmResult.m_NodeCommand);
#else
          DEVLOG_DEBUG("[powerlink] %s(Node=0x%X, CfmResult=0x%X)\n", __func__, paEventArg->m_CfmResult.m_uiNodeId,
              paEventArg->m_CfmResult.m_NodeCommand);
#endif
          break;
        }
      }
      break;
    }
#endif

    default:
      break;
  }

  return ret;
}

//---------------------------------------------------------------------------
//
// Function:    AppCbSync
//
// Description: sync event callback function called by event module within
//              kernel part (high priority).
//              This function sets the outputs, reads the inputs and runs
//              the control loop.
//
// Parameters:  void
//
// Returns:     tEplKernel      = error code,
//                                kEplSuccessful = no error
//                                otherwise = post error event to API layer
//
// State:
//
//---------------------------------------------------------------------------

tEplKernel PUBLIC appCbSync() {
  tEplKernel ret = EplApiProcessImageExchange(&AppProcessImageCopyJob_g);

  // Loop through callback list and call each FB in the list
  CEplStackWrapper::getInstance().executeAllCallbacks();

  return ret;
}

void CEplStackWrapper::executeAllCallbacks() {
  mSync.lock();
  CSinglyLinkedList<IEplCNCallback *>::Iterator itEnd = mCallbackList.end();
  for (CSinglyLinkedList<IEplCNCallback *>::Iterator it = mCallbackList.begin(); it != itEnd; ++it) {
    it->cnSynchCallback();
  }
  mSync.unlock();
}

#ifndef CONFIG_POWERLINK_USERSTACK

void *powerlinkEventThread(void * arg __attribute__((unused))){
  EplApiProcess();

  return nullptr;
}

void *powerlinkSyncThread(void * arg __attribute__((unused))){
  while(1){
  AppCbSync();
  }
  return nullptr;
}

#endif

// EOF
