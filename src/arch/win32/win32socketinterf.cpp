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

#ifdef __GNUC__
#ifdef __MINGW32__

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0600 //windows vista version, which included InepPton

#endif //__MINGW32__
#endif //__GNUC__

#include <sockhand.h>      //needs to be first pulls in the platform specific includes
#include "win32socketinterf.h"
#include "devlog.h"
#include <string.h>

void CWin32SocketInterface::closeSocket(TSocketDescriptor pa_nSockD){
  closesocket(pa_nSockD);
}

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::openTCPServerConnection(char *pa_acIPAddr, unsigned short pa_nPort){
  TSocketDescriptor nRetVal = INVALID_SOCKET;

  DEVLOG_INFO("CWin32SocketInterface: Opening TCP-Server connection at: %s:%d\n", pa_acIPAddr, pa_nPort);

  TSocketDescriptor nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(INVALID_SOCKET != nSocket){
    struct sockaddr_in stSockAddr = { 0 };
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(pa_nPort);
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int nOptVal = 1;
    if(setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &nOptVal, sizeof(nOptVal)) == -1){
      LPSTR pacErrorMessage = getErrorMessage(WSAGetLastError());
      DEVLOG_ERROR("CWin32SocketInterface: could not set socket option SO_REUSEADDR:  %s\n", pacErrorMessage);
      LocalFree(pacErrorMessage);
    }

    if(0 == bind(nSocket, (struct sockaddr *) &stSockAddr, sizeof(struct sockaddr))){
      if(-1 == listen(nSocket, 1)){ // for the classic IEC 61499 server only one connection at the same time is accepted TODO mayb make this adjustable for future extensions
        int nLastError = WSAGetLastError();
        LPSTR pacErrorMessage = getErrorMessage(nLastError);
        DEVLOG_ERROR("CWin32SocketInterface: listen() failed: %d - %s\n", nLastError, pacErrorMessage);
        LocalFree(pacErrorMessage);
      }
      else{
        nRetVal = nSocket;
      }
    }
    else{
      int nLastError = WSAGetLastError();
      LPSTR pacErrorMessage = getErrorMessage(nLastError);
      DEVLOG_ERROR("CWin32SocketInterface: bind() failed: %d - %s\n", nLastError, pacErrorMessage);
      LocalFree(pacErrorMessage);
    }
  }
  else{
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: Couldn't create socket: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::openTCPClientConnection(char *pa_acIPAddr, unsigned short pa_nPort){
  TSocketDescriptor nRetVal = INVALID_SOCKET;
  TSocketDescriptor nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  DEVLOG_INFO("CWin32SocketInterface: Opening TCP-Client connection at: %s:%d\n", pa_acIPAddr, pa_nPort);

  if(-1 != nSocket){
    struct sockaddr_in stSockAddr = { 0 };
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(pa_nPort);
    if(1 != InetPton(stSockAddr.sin_family, pa_acIPAddr, &(stSockAddr.sin_addr))) {
      DEVLOG_ERROR("CWin32SocketInterface: InetPton() failed: %d - %s\n", stSockAddr.sin_addr.s_addr, pa_acIPAddr);
    }

    if(-1 == connect(nSocket, (struct sockaddr *) &stSockAddr, sizeof(struct sockaddr))){
      int nLastError = WSAGetLastError();
      LPSTR pacErrorMessage = getErrorMessage(nLastError);
      DEVLOG_ERROR("CWin32SocketInterface: connect() failed: %d - %s\n", nLastError, pacErrorMessage);
      LocalFree(pacErrorMessage);
    }
    else{
      nRetVal = nSocket;
    }
  }
  else{
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: Couldn't create socket: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::acceptTCPConnection(TSocketDescriptor pa_nListeningSockD){
  struct sockaddr client_addr;
  int sin_size = sizeof(struct sockaddr);
  TSocketDescriptor nRetVal = accept(pa_nListeningSockD, &client_addr, &sin_size);

  if(INVALID_SOCKET == nRetVal){
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: Couldn't create socket: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

int CWin32SocketInterface::sendDataOnTCP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unSize){
  // This function sends all data in the buffer before it returns!
  int nToSend = pa_unSize;
  int nRetVal = 0;

  while(0 < nToSend){
    //TODO: check if open connection (socket might be closed by peer)
    nRetVal = static_cast<int>(send(pa_nSockD, pa_pcData, nToSend, 0));
    if(nRetVal <= 0){
      LPSTR pacErrorMessage = getErrorMessage(WSAGetLastError());
      DEVLOG_ERROR("TCP-Socket Send failed: %s\n", pacErrorMessage);
      LocalFree(pacErrorMessage);
      break;
    }
    nToSend -= nRetVal;
    pa_pcData += nRetVal;
  }
  return nRetVal;
}

int CWin32SocketInterface::receiveDataFromTCP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unBufSize){
  int nRetVal;
  do{
    nRetVal = static_cast<int>(recv(pa_nSockD, pa_pcData, pa_unBufSize, 0));
  } while((-1 == nRetVal) && (WSAEINTR == h_errno)); // recv got interrupt / recieving again
  if(nRetVal == -1){
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: TCP-Socket recv() failed: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
    if(WSAECONNRESET == nLastError){
      nRetVal = 0; //inform higher levels that the peer closed connection
    }
  }
  return nRetVal;
}

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::openUDPSendPort(char *pa_acIPAddr, unsigned short pa_nPort, TUDPDestAddr *m_ptDestAddr){
  DEVLOG_INFO("CWin32SocketInterface: Opening UDP sending connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  TSocketDescriptor nRetVal = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(-1 != nRetVal){
    m_ptDestAddr->sin_family = AF_INET;
    m_ptDestAddr->sin_port = htons(pa_nPort);
    InetPton(m_ptDestAddr->sin_family, pa_acIPAddr, &(m_ptDestAddr->sin_addr));
    memset(&(m_ptDestAddr->sin_zero), '\0', sizeof(m_ptDestAddr->sin_zero));
  }
  else{
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: UDP-Socket socket() failed: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::openUDPReceivePort(char *pa_acIPAddr, unsigned short pa_nPort){
  DEVLOG_INFO("CWin32SocketInterface: Opening UDP receiving connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  TSocketDescriptor nRetVal = INVALID_SOCKET;
  TSocketDescriptor nSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(-1 != nSocket){
    int nReuseAddrVal = 1;
    if(0 <= setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &nReuseAddrVal, sizeof(nReuseAddrVal))){
      struct sockaddr_in stSockAddr;
      stSockAddr.sin_family = AF_INET;
      stSockAddr.sin_port = htons(pa_nPort);
      stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      memset(&(stSockAddr.sin_zero), '\0', sizeof(stSockAddr.sin_zero));
      if(0 == bind(nSocket, (struct sockaddr *) &stSockAddr, sizeof(struct sockaddr))){
        // setting up multicast group
        struct ip_mreq stMReq;
        InetPton(stSockAddr.sin_family, pa_acIPAddr, &(stMReq.imr_multiaddr));
        stMReq.imr_interface.s_addr = htonl(INADDR_ANY);
        setsockopt(nSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &stMReq, sizeof(stMReq));
        //if this fails we may have given a non multicasting addr. For now we accept this. May need to be changed in the future.

        nRetVal = nSocket;
      }
      else{
        int nLastError = WSAGetLastError();
            LPSTR pacErrorMessage = getErrorMessage(nLastError);
        DEVLOG_ERROR("CWin32SocketInterface: bind() failed: %d - %s\n", nLastError,pacErrorMessage);
        LocalFree(pacErrorMessage);
      }
    }
    else{
      int nLastError = WSAGetLastError();
          LPSTR pacErrorMessage = getErrorMessage(nLastError);
      DEVLOG_ERROR("CWin32SocketInterface: setsockopt(SO_REUSEADDR) failed: %d - %s\n", nLastError, pacErrorMessage);
      LocalFree(pacErrorMessage);
    }
  }
  else{
    int nLastError = WSAGetLastError();
        LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: Couldn't create socket: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

int CWin32SocketInterface::sendDataOnUDP(TSocketDescriptor pa_nSockD, TUDPDestAddr *pa_ptDestAddr, char* pa_pcData, unsigned int pa_unSize){
  // This function sends all data in the buffer before it returns!
  int nToSend = pa_unSize;
  int nRetVal = 0;

  while(0 < nToSend){
    //TODO: check if open connection (socket might be closed by peer)
    nRetVal = static_cast<int>(sendto(pa_nSockD, pa_pcData, nToSend, 0, (struct sockaddr *) pa_ptDestAddr, sizeof(struct sockaddr)));
    if(nRetVal <= 0){
      int nLastError = WSAGetLastError();
      LPSTR pacErrorMessage = getErrorMessage(nLastError);
      DEVLOG_ERROR("CWin32SocketInterface: UDP-Socket Send failed: %d - %s\n", nLastError, pacErrorMessage);
      LocalFree(pacErrorMessage);
      break;
    }
    nToSend -= nRetVal;
    pa_pcData += nRetVal;
  }
  return nRetVal;
}

int CWin32SocketInterface::receiveDataFromUDP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unBufSize){
  int nRetVal;
  do{
    nRetVal = static_cast<int>(recvfrom(pa_nSockD, pa_pcData, pa_unBufSize, 0, 0, 0));
  } while((-1 == nRetVal) && (WSAEINTR == h_errno)); // recv got interrupt / recieving again

  if(nRetVal == -1){ //
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: UDP-Socket recvfrom() failed: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

LPSTR CWin32SocketInterface::getErrorMessage(int pa_nErrorNumber){
  LPSTR pacErrorMessage = NULL;
  FormatMessage(
  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
  NULL, pa_nErrorNumber, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), pacErrorMessage, 0,
  NULL);
  return pacErrorMessage;
}
