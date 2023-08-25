/*******************************************************************************
 * Copyright (c) 2010 - 2014 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef BSDSOCKETINTERF_H_
#define BSDSOCKETINTERF_H_


class CWin32SocketInterface {
  public:
    typedef SOCKET TSocketDescriptor;
    typedef struct sockaddr_in TUDPDestAddr;

    static void closeSocket(TSocketDescriptor paSockD);
    static TSocketDescriptor openTCPServerConnection(char *paIPAddr, unsigned short paPort);
    static TSocketDescriptor openTCPClientConnection(char *paIPAddr, unsigned short paPort);
    static TSocketDescriptor acceptTCPConnection(TSocketDescriptor paListeningSockD);
    static int sendDataOnTCP(TSocketDescriptor paSockD, const char* paData, unsigned int paSize);
    static int receiveDataFromTCP(TSocketDescriptor paSockD, char* paData, unsigned int paBufSize);

    static TSocketDescriptor openUDPSendPort(char *paIPAddr, unsigned short paPort, TUDPDestAddr *mDestAddr);
    static TSocketDescriptor openUDPReceivePort(char *paIPAddr, unsigned short paPort);
    static int sendDataOnUDP(TSocketDescriptor paSockD, TUDPDestAddr *paDestAddr, char* paData, unsigned int paSize);
    static int receiveDataFromUDP(TSocketDescriptor paSockD, char* paData, unsigned int paBufSize);

  private:
    CWin32SocketInterface(); //this function is not implemented as we don't want instances of this class

    static LPSTR getErrorMessage(int paErrorNumber);
};

#endif /* BSDSOCKETINTERF_H_ */
