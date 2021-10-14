/*******************************************************************************
 * Copyright (c) 2010 - 2015 ACIN, Profactor GmbH, AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny, Gerhard Ebenhofer, Thomas Strasser
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <sockhand.h>      //needs to be first pulls in the platform specific includes
#include "bsdsocketinterf.h"
#include "devlog.h"
#include <string.h>

void CBSDSocketInterface::closeSocket(TSocketDescriptor pa_nSockD){
#if defined(NET_OS)
  closesocket(pa_nSockD);
#else
  close(pa_nSockD);
#endif
}

CBSDSocketInterface::TSocketDescriptor CBSDSocketInterface::openTCPServerConnection(
    char *pa_acIPAddr, unsigned short pa_nPort){
  TSocketDescriptor nRetVal = -1;

#ifndef LOGINFO
  (void)pa_acIPAddr;
#else
  DEVLOG_INFO("CBSDSocketInterface: Opening TCP-Server connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
#endif


  TSocketDescriptor nSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(-1 != nSocket){
    struct sockaddr_in stSockAddr;
    memset(&(stSockAddr), '\0', sizeof(sockaddr_in));
    stSockAddr.sin_family = AF_INET;
#if VXWORKS
    stSockAddr.sin_port = static_cast<unsigned short>(htons(pa_nPort));
#else
    stSockAddr.sin_port = htons(pa_nPort);
#endif
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY );

    int nOptVal = 1;
    if (setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&nOptVal, sizeof(nOptVal)) == -1){
      DEVLOG_ERROR("CBSDSocketInterface: could not set socket option SO_REUSEADDR:  %s\n", strerror(errno));
    }

    if(0 == bind(nSocket, (struct sockaddr *) &stSockAddr, sizeof(struct sockaddr))){
      if(-1 == listen(nSocket, 1)){ // for the classic IEC 61499 server only one connection at the same time is accepted TODO mayb make this adjustable for future extensions
        DEVLOG_ERROR("CBSDSocketInterface: listen() failed: %s\n", strerror(errno));
      }
      else{
        nRetVal = nSocket;
      }
    }
    else{
      DEVLOG_ERROR("CBSDSocketInterface: bind() failed: %s\n", strerror(errno));
    }
    if(-1 == nRetVal){
      close(nSocket);
    }
  }
  else{
    DEVLOG_ERROR("CBSDSocketInterface: Couldn't create socket: %s\n", strerror(errno));
  }
  return nRetVal;
}

CBSDSocketInterface::TSocketDescriptor CBSDSocketInterface::openTCPClientConnection(
    char *pa_acIPAddr, unsigned short pa_nPort){
  TSocketDescriptor nRetVal = -1;
  TSocketDescriptor nSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  DEVLOG_INFO("CBSDSocketInterface: Opening TCP-Client connection at: %s:%d\n", pa_acIPAddr, pa_nPort);

  if(-1 != nSocket){
    struct sockaddr_in stSockAddr;
    stSockAddr.sin_family = AF_INET;
#if VXWORKS
    stSockAddr.sin_port = static_cast<unsigned short>(htons(pa_nPort));
#else
    stSockAddr.sin_port = htons(pa_nPort);
#endif
    stSockAddr.sin_addr.s_addr = inet_addr(pa_acIPAddr);
    memset(&(stSockAddr.sin_zero), '\0', sizeof(stSockAddr.sin_zero));

    if(-1 == connect(nSocket, (struct sockaddr *) &stSockAddr, sizeof(struct sockaddr))){
      close(nSocket);
      DEVLOG_ERROR("CBSDSocketInterface: connect() failed: %s\n", strerror(errno));
    }
    else{
      nRetVal = nSocket;
    }
  }
  else{
    DEVLOG_ERROR("CBSDSocketInterface: Couldn't create socket: %s\n", strerror(errno));
  }
  return nRetVal;
}

CBSDSocketInterface::TSocketDescriptor CBSDSocketInterface::acceptTCPConnection(
    TSocketDescriptor pa_nListeningSockD){
  struct sockaddr client_addr;
  int sin_size = sizeof(struct sockaddr);
  TSocketDescriptor nRetVal;

#if defined(NET_OS) || defined (VXWORKS)
  nRetVal = accept(pa_nListeningSockD, &client_addr, &sin_size);
#else
  nRetVal = accept(pa_nListeningSockD, &client_addr, (socklen_t*) &sin_size);
#endif
  return nRetVal;
}

int CBSDSocketInterface::sendDataOnTCP(TSocketDescriptor pa_nSockD, char* pa_pcData,
    unsigned int pa_unSize){
  // This function sends all data in the buffer before it returns!
  int nToSend = pa_unSize;
  int nRetVal = 0;

  while(0 < nToSend){
    //TODO: check if open connection (socket might be closed by peer)
    nRetVal = static_cast<int>(send(pa_nSockD, pa_pcData, nToSend, 0));
    if(nRetVal <= 0){
      DEVLOG_ERROR("TCP-Socket Send failed: %s\n", strerror(errno));
      break;
    }
    nToSend -= nRetVal;
    pa_pcData += nRetVal;
  }
  return nRetVal;
}

int CBSDSocketInterface::receiveDataFromTCP(TSocketDescriptor pa_nSockD, char* pa_pcData,
    unsigned int pa_unBufSize){
  int nRetVal;
  do{
    nRetVal = static_cast<int>(recv(pa_nSockD, pa_pcData, pa_unBufSize, 0));
  } while((-1 == nRetVal) && (EINTR == errno)); // recv got interrupt / recieving again

  if(nRetVal == -1){
    DEVLOG_ERROR("CBSDSocketInterface: TCP-Socket recv() failed: %s\n", strerror(errno));
  }
  return nRetVal;
}

CBSDSocketInterface::TSocketDescriptor CBSDSocketInterface::openUDPSendPort(char *pa_acIPAddr,
    unsigned short pa_nPort, TUDPDestAddr *m_ptDestAddr){
  DEVLOG_INFO("CBSDSocketInterface: Opening UDP sending connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  TSocketDescriptor nRetVal;

  nRetVal = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(-1 != nRetVal){
    m_ptDestAddr->sin_family = AF_INET;
#if VXWORKS
    m_ptDestAddr->sin_port = static_cast<unsigned short>(htons(pa_nPort));
#else
    m_ptDestAddr->sin_port = htons(pa_nPort);
#endif
    m_ptDestAddr->sin_addr.s_addr = inet_addr(pa_acIPAddr);
    memset(&(m_ptDestAddr->sin_zero), '\0', sizeof(m_ptDestAddr->sin_zero));

#ifdef NET_OS
    /* following is typedef void TM_fAR * in treck/include/trsocket.h */
    int nIfCount;
    int nRunner = 0;
    unsigned long nLocalInAddr;
    ttUserInterface *pIfList;
    ttUserInterface stInterfaceHandle; /* gets recast as above */

    /*get interfaces */
    pIfList = tfGetInterfaceList(&nIfCount);
    while (nIfCount--){
      stInterfaceHandle = pIfList[nRunner++];
      if (tfGetIpAddress(stInterfaceHandle, &nLocalInAddr, 0) == 0){ //get IPv4 Address of interface
        /* bind socket to all available IPv4 addresses for multicast */
        setsockopt (nRetVal, IPPROTO_IP, IP_MULTICAST_IF, (char *) &nLocalInAddr, sizeof (nLocalInAddr));
      }
    }
#endif

  }
  return nRetVal;
}

CBSDSocketInterface::TSocketDescriptor CBSDSocketInterface::openUDPReceivePort(char *pa_acIPAddr,
    unsigned short pa_nPort){
  DEVLOG_INFO("CBSDSocketInterface: Opening UDP receiving connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  TSocketDescriptor nRetVal = -1;
  TSocketDescriptor nSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(-1 != nSocket){
    int nReuseAddrVal = 1;
    if(0
        <= setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &nReuseAddrVal, sizeof(nReuseAddrVal))){

#ifdef __APPLE__
        if(0 > setsockopt(nSocket, SOL_SOCKET, SO_REUSEPORT, (char *) &nReuseAddrVal, sizeof(nReuseAddrVal))){
            DEVLOG_ERROR("CBSDSocketInterface: setsockopt(SO_REUSEPORT) failed: %s\n", strerror(errno));
            return nRetVal;
        }
#endif

      struct sockaddr_in stSockAddr;
      stSockAddr.sin_family = AF_INET;
#if VXWORKS
      stSockAddr.sin_port = static_cast<unsigned short>(htons(pa_nPort));
#else
      stSockAddr.sin_port = htons(pa_nPort);
#endif
      stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      memset(&(stSockAddr.sin_zero), '\0', sizeof(stSockAddr.sin_zero));
      if(0 == bind(nSocket, (struct sockaddr *) &stSockAddr, sizeof(struct sockaddr))){
        // setting up multicast group
        struct ip_mreq stMReq;
        stMReq.imr_multiaddr.s_addr = inet_addr(pa_acIPAddr);
        stMReq.imr_interface.s_addr = htonl(INADDR_ANY);
        if(0 > setsockopt(nSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &stMReq, sizeof(stMReq))){
          //if this fails we may have given a non multicasting addr. For now we accept this. May need to be changed in the future.
          DEVLOG_WARNING("CBSDSocketInterface: setsockopt(IP_ADD_MEMBERSHIP) failed: %s\n", strerror(errno));
        }

        nRetVal = nSocket;
      }
      else{
        DEVLOG_ERROR("CBSDSocketInterface: bind() failed: %s\n", strerror(errno));
      }
    }
    else{
      DEVLOG_ERROR("CBSDSocketInterface: setsockopt(SO_REUSEADDR) failed: %s\n", strerror(errno));
    }
  }
  else{
    DEVLOG_ERROR("CBSDSocketInterface: Couldn't create socket: %s\n", strerror(errno));
  }
  return nRetVal;
}

int CBSDSocketInterface::sendDataOnUDP(TSocketDescriptor pa_nSockD, TUDPDestAddr *pa_ptDestAddr,
    char* pa_pcData, unsigned int pa_unSize){
  // This function sends all data in the buffer before it returns!
  int nToSend = pa_unSize;
  int nRetVal = 0;

  while(0 < nToSend){
    //TODO: check if open connection (socket might be closed by peer)
    nRetVal = static_cast<int>(
        sendto(pa_nSockD, pa_pcData, nToSend, 0, (struct sockaddr *) pa_ptDestAddr, sizeof(struct sockaddr)));
    if(nRetVal <= 0){
      DEVLOG_ERROR("CBSDSocketInterface: UDP-Socket Send failed: %s\n", strerror(errno));
      break;
    }
    nToSend -= nRetVal;
    pa_pcData += nRetVal;
  }
  return nRetVal;
}

int CBSDSocketInterface::receiveDataFromUDP(TSocketDescriptor pa_nSockD, char* pa_pcData,
    unsigned int pa_unBufSize){
  int nRetVal;
  do{
    nRetVal = static_cast<int>(recvfrom(pa_nSockD, pa_pcData, pa_unBufSize, 0, 0, 0));
  } while((-1 == nRetVal) && (EINTR == errno)); // recv got interrupt / recieving again

  if(nRetVal == -1){ //
    DEVLOG_ERROR("CBSDSocketInterface: UDP-Socket recvfrom() failed: %s\n", strerror(errno));
  }
  return nRetVal;
}
