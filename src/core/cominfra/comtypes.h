/*******************************************************************************
 * Copyright (c) 2010-2013 TU Wien ACIN.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fix typo in forte::com_infra::EComConnectionState
 *******************************************************************************/
#ifndef _COMTYPES_H_
#define _COMTYPES_H_

#include <datatype.h>

namespace forte {
  //TODO maybe add core namespace depending on namespace support of older c++ compilers
  namespace com_infra {

    static const unsigned int scg_unINIT = 0x10;
    static const unsigned int scg_unProcessData = 0x20;
    static const unsigned int scg_unComNegative = 0x40;

    enum EComResponseType{
      e_InitPositive = scg_unINIT, e_ProcessDataPositive = scg_unProcessData, e_InitNegative = scg_unINIT | scg_unComNegative, e_ProcessDataNegative = scg_unProcessData | scg_unComNegative
    };

    enum EComResponseMsgIds{
        e_Ok = 0,
        e_InvalidId = 1,
        e_Terminated = 2,
        e_InvalidObject = 3,
        e_DataTypeError = 4,
        e_Inhibited = 5,
        e_NoSocket = 6,
        e_SendFailed = 7,
        e_RecvFailed = 8
    };

    enum EComResponse{
      e_Nothing = 0,
      e_InitOk = e_InitPositive | e_Ok,
      e_ProcessDataOk = e_ProcessDataPositive | e_Ok,
      e_InitInvalidId = e_InitNegative | e_InvalidId,
      e_InitTerminated = e_InitNegative | e_Terminated,
      e_ProcessDataInvalidObject = e_ProcessDataNegative | e_InvalidObject,
      e_ProcessDataDataTypeError = e_ProcessDataNegative | e_DataTypeError,
      e_ProcessDataInhibited = e_ProcessDataNegative | e_Inhibited,
      e_ProcessDataNoSocket = e_ProcessDataNegative | e_NoSocket,
      e_ProcessDataSendFailed = e_ProcessDataNegative | e_SendFailed,
      e_ProcessDataRecvFaild = e_ProcessDataNegative | e_RecvFailed
   };

  /*!\brief Enum for different Communication FB types
   * Only lower nibble is used, 1st bit 1 = Data inputs, 2nd bit 1 = Data outputs, 3rd bit 0 = Responder,
   * 3rd bit 1 = Requester
   */
  enum EComParameterTypes{
    e_Responder = 0, //b00000000,
    e_DataInputs = 1, // b00000001,
    e_DataOutputs = 2,// b00000010,
    e_Requester = 4 //b00000100
  };

  enum EComServiceType{
    e_Publisher = e_Requester | e_DataInputs, //!< e_Publisher
    e_Subscriber = e_Responder | e_DataOutputs, //!< e_Subscriber
    e_Server = e_Responder | e_DataInputs | e_DataOutputs, //!< e_Server
    e_Client = e_Requester | e_DataInputs | e_DataOutputs
    //!< e_Client
  };

  enum EComConnectionState{
    e_Disconnected, e_Listening, e_ConnectedAndListening, e_Connected
  };

  struct SRetBuffers{
    TForteUInt32 m_unTopGetDataSize;
    TForteUInt32 m_unBottomGetDataSize;
  };

}
}
#endif
