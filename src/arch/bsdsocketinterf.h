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

#include <sockhand.h>

class CBSDSocketInterface{
  public:
    typedef FORTE_SOCKET_TYPE TSocketDescriptor;
    typedef struct sockaddr_in TUDPDestAddr;

    static void closeSocket(TSocketDescriptor pa_nSockD);
    static TSocketDescriptor openTCPServerConnection(char *pa_acIPAddr, unsigned short pa_nPort);
    static TSocketDescriptor openTCPClientConnection(char *pa_acIPAddr, unsigned short pa_nPort);
    static TSocketDescriptor acceptTCPConnection(TSocketDescriptor pa_nListeningSockD);
    static int sendDataOnTCP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unSize);
    static int receiveDataFromTCP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unBufSize);

    static TSocketDescriptor openUDPSendPort(char *pa_acIPAddr, unsigned short pa_nPort, TUDPDestAddr *m_ptDestAddr);
    static TSocketDescriptor openUDPReceivePort(char *pa_acIPAddr, unsigned short pa_nPort);
    static int sendDataOnUDP(TSocketDescriptor pa_nSockD, TUDPDestAddr *pa_ptDestAddr, char* pa_pcData, unsigned int pa_unSize);
    static int receiveDataFromUDP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unBufSize);

  private:
    CBSDSocketInterface(); //this function is not implemented as we don't want instances of this class
};

#endif /* BSDSOCKETINTERF_H_ */
