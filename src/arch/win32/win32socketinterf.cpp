/*******************************************************************************
 * Copyright (c) 2010, 2021 ACIN, Profactor GmbH, AIT, fortiss GmbH, OFFIS e.V., HIT robot group
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny, Gerhard Ebenhofer, Thomas Strasser
 *     - initial API and implementation and/or initial documentation
 *  Jörg Walter - Windows XP compatibility
 *  Zhao Xin - fix socket resource leakage
 *******************************************************************************/

#include <sockhand.h>      //needs to be first pulls in the platform specific includes
#include "win32socketinterf.h"
#include "devlog.h"
#include <string.h>

void CWin32SocketInterface::closeSocket(TSocketDescriptor paSockD){
  closesocket(paSockD);
}

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::openTCPServerConnection(char *paIPAddr, unsigned short paPort){
  TSocketDescriptor nRetVal = INVALID_SOCKET;

  DEVLOG_INFO("CWin32SocketInterface: Opening TCP-Server connection at: %s:%d\n", paIPAddr, paPort);

  TSocketDescriptor nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(INVALID_SOCKET != nSocket){
    struct sockaddr_in stSockAddr = { };
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(paPort);
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int nOptVal = 1;
    if(SOCKET_ERROR == setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &nOptVal, sizeof(nOptVal))) {
      LPSTR pacErrorMessage = getErrorMessage(WSAGetLastError());
      DEVLOG_ERROR("CWin32SocketInterface: could not set socket option SO_REUSEADDR:  %s\n", pacErrorMessage);
      LocalFree(pacErrorMessage);
    }

    if(0 == bind(nSocket, (struct sockaddr *) &stSockAddr, sizeof(struct sockaddr))){
      if(SOCKET_ERROR == listen(nSocket, 1)) { // for the classic IEC 61499 server only one connection at the same time is accepted TODO mayb make this adjustable for future extensions
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

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::openTCPClientConnection(char *paIPAddr, unsigned short paPort){
  TSocketDescriptor nRetVal = INVALID_SOCKET;
  TSocketDescriptor nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  DEVLOG_INFO("CWin32SocketInterface: Opening TCP-Client connection at: %s:%d\n", paIPAddr, paPort);

  if(INVALID_SOCKET != nSocket) {
    sockaddr_in stSockAddr = {};
    int stSockAddrSz = sizeof(stSockAddr);
    if(WSAStringToAddressA(paIPAddr, AF_INET, nullptr, (LPSOCKADDR)&stSockAddr, &stSockAddrSz)) {
      DEVLOG_ERROR("CWin32SocketInterface: WSAStringToAddressA() failed: %d - %s\n", stSockAddr.sin_addr.s_addr, paIPAddr);
    }
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(paPort);

    if(SOCKET_ERROR == connect(nSocket, (struct sockaddr*) &stSockAddr, sizeof(struct sockaddr))) {
      int nLastError = WSAGetLastError();
      LPSTR pacErrorMessage = getErrorMessage(nLastError);
      DEVLOG_ERROR("CWin32SocketInterface: connect() failed: %d - %s\n", nLastError, pacErrorMessage);
      LocalFree(pacErrorMessage);
      closeSocket(nSocket);
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

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::acceptTCPConnection(TSocketDescriptor paListeningSockD){
  struct sockaddr client_addr;
  int sin_size = sizeof(struct sockaddr);
  TSocketDescriptor nRetVal = accept(paListeningSockD, &client_addr, &sin_size);

  if(INVALID_SOCKET == nRetVal){
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: Couldn't create socket: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

int CWin32SocketInterface::sendDataOnTCP(TSocketDescriptor paSockD, const char* paData, unsigned int paSize){
  // This function sends all data in the buffer before it returns!
  int nToSend = paSize;
  int nRetVal = 0;

  while(0 < nToSend){
    //TODO: check if open connection (socket might be closed by peer)
    nRetVal = static_cast<int>(send(paSockD, paData, nToSend, 0));
    if(nRetVal <= 0){
      LPSTR pacErrorMessage = getErrorMessage(WSAGetLastError());
      DEVLOG_ERROR("TCP-Socket Send failed: %s\n", pacErrorMessage);
      LocalFree(pacErrorMessage);
      break;
    }
    nToSend -= nRetVal;
    paData += nRetVal;
  }
  return nRetVal;
}

int CWin32SocketInterface::receiveDataFromTCP(TSocketDescriptor paSockD, char* paData, unsigned int paBufSize){
  int nRetVal;
  do{
    nRetVal = recv(paSockD, paData, paBufSize, 0);
  } while((SOCKET_ERROR == nRetVal) && (WSAEINTR == h_errno)); // recv got interrupt / recieving again
  if(SOCKET_ERROR == nRetVal) {
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

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::openUDPSendPort(char *paIPAddr, unsigned short paPort, TUDPDestAddr *mDestAddr){
  DEVLOG_INFO("CWin32SocketInterface: Opening UDP sending connection at: %s:%d\n", paIPAddr, paPort);
  TSocketDescriptor nRetVal = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(INVALID_SOCKET != nRetVal) {
    *mDestAddr = TUDPDestAddr();
    int mDestAddrSz = sizeof(*mDestAddr);
    if(WSAStringToAddressA(paIPAddr, AF_INET, nullptr, (LPSOCKADDR)mDestAddr, &mDestAddrSz)) {
      DEVLOG_ERROR("CWin32SocketInterface: WSAStringToAddressA() failed: %d - %s\n", mDestAddr->sin_addr.s_addr, paIPAddr);
    }
    mDestAddr->sin_family = AF_INET;
    mDestAddr->sin_port = htons(paPort);
 }
  else{
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: UDP-Socket socket() failed: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

CWin32SocketInterface::TSocketDescriptor CWin32SocketInterface::openUDPReceivePort(char *paIPAddr, unsigned short paPort){
  DEVLOG_INFO("CWin32SocketInterface: Opening UDP receiving connection at: %s:%d\n", paIPAddr, paPort);
  TSocketDescriptor nRetVal = INVALID_SOCKET;
  TSocketDescriptor nSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(INVALID_SOCKET != nSocket) {
    int nReuseAddrVal = 1;
    if(0 <= setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &nReuseAddrVal, sizeof(nReuseAddrVal))){
      struct sockaddr_in stSockAddr = {};
      stSockAddr.sin_family = AF_INET;
      stSockAddr.sin_port = htons(paPort);
      stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      if(0 == bind(nSocket, (struct sockaddr *) &stSockAddr, sizeof(struct sockaddr))){
        // setting up multicast group
        sockaddr_in stMCastAddr = {};
        int stMCastAddrSz = sizeof(stMCastAddr);
        if(WSAStringToAddressA(paIPAddr, AF_INET, nullptr, (LPSOCKADDR)&stMCastAddr, &stMCastAddrSz)) {
          DEVLOG_ERROR("CWin32SocketInterface: WSAStringToAddressA() failed: %d - %s\n", stMCastAddr.sin_addr.s_addr, paIPAddr);
        }

        struct ip_mreq stMReq = {};
        stMReq.imr_multiaddr = stMCastAddr.sin_addr;
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

int CWin32SocketInterface::sendDataOnUDP(TSocketDescriptor paSockD, TUDPDestAddr *paDestAddr, char* paData, unsigned int paSize){
  // This function sends all data in the buffer before it returns!
  int nToSend = paSize;
  int nRetVal = 0;

  while(0 < nToSend){
    //TODO: check if open connection (socket might be closed by peer)
    nRetVal = static_cast<int>(sendto(paSockD, paData, nToSend, 0, (struct sockaddr *) paDestAddr, sizeof(struct sockaddr)));
    if(nRetVal <= 0){
      int nLastError = WSAGetLastError();
      LPSTR pacErrorMessage = getErrorMessage(nLastError);
      DEVLOG_ERROR("CWin32SocketInterface: UDP-Socket Send failed: %d - %s\n", nLastError, pacErrorMessage);
      LocalFree(pacErrorMessage);
      break;
    }
    nToSend -= nRetVal;
    paData += nRetVal;
  }
  return nRetVal;
}

int CWin32SocketInterface::receiveDataFromUDP(TSocketDescriptor paSockD, char* paData, unsigned int paBufSize){
  int nRetVal;
  do{
    nRetVal = recvfrom(paSockD, paData, paBufSize, 0, 0, 0);
  } while((SOCKET_ERROR == nRetVal) && (WSAEINTR == h_errno)); // recv got interrupt / recieving again

  if(SOCKET_ERROR == nRetVal) {
    int nLastError = WSAGetLastError();
    LPSTR pacErrorMessage = getErrorMessage(nLastError);
    DEVLOG_ERROR("CWin32SocketInterface: UDP-Socket recvfrom() failed: %d - %s\n", nLastError, pacErrorMessage);
    LocalFree(pacErrorMessage);
  }
  return nRetVal;
}

LPSTR CWin32SocketInterface::getErrorMessage(int paErrorNumber){
  LPSTR pacErrorMessage = nullptr;
  FormatMessage(
  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
  nullptr, paErrorNumber, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), pacErrorMessage, 0,
  nullptr);
  return pacErrorMessage;
}
