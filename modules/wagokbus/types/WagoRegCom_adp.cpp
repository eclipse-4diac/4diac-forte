/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.100.qualifier!
 ***
 *** Name: WagoRegCom
 *** Description: Register communication for Wago modules
 *** Version:
 ***     3.0: 2026-07-21/Monika Wenger -  -
 *************************************************************************/

#include "WagoRegCom_adp.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::io::wago {
  namespace {
    const auto cEventInputNames = std::array{"Opened"_STRID, "CNF"_STRID, "Closed"_STRID, "Error"_STRID};
    const auto cEventOutputNames = std::array{"Open"_STRID, "Read"_STRID, "Write"_STRID, "Close"_STRID};
    const auto cDataInputNames = std::array{"STATUS"_STRID, "counter"_STRID, "REG_D0"_STRID, "REG_D1"_STRID};
    const auto cDataOutputNames = std::array{"autoPsw"_STRID, "cmd"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpecSocket = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

    const SFBInterfaceSpec cFBInterfaceSpecPlug = {
        .mEINames = cEventOutputNames,
        .mEITypeNames = {},
        .mEONames = cEventInputNames,
        .mEOTypeNames = {},
        .mDINames = cDataOutputNames,
        .mDONames = cDataInputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  }

  DEFINE_ADAPTER_TYPE(FORTE_WagoRegCom, "eclipse4diac::io::wago::WagoRegCom"_STRID)


  FORTE_WagoRegCom::FORTE_WagoRegCom(CFBContainer &paContainer,
                               const SFBInterfaceSpec &paInterfaceSpec,
                               const StringId paInstanceNameId,
                               TForteUInt8 paParentAdapterlistID) :
      CAdapter(paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
      var_STATUS(u""_WSTRING),
      var_counter(0_USINT),
      var_REG_D0(0_BYTE),
      var_REG_D1(0_BYTE),
      var_autoPsw(true_BOOL),
      var_cmd(CIEC_WagoRegComCmd()) {
  }

  void FORTE_WagoRegCom::setInitialValues() {
    forte::CAdapter::setInitialValues();
    var_STATUS = u""_WSTRING;
    var_counter = 0_USINT;
    var_REG_D0 = 0_BYTE;
    var_REG_D1 = 0_BYTE;
    var_autoPsw = true_BOOL;
    var_cmd = CIEC_WagoRegComCmd();
  }


  FORTE_WagoRegCom_Plug::FORTE_WagoRegCom_Plug(StringId paInstanceNameId,
                                           CFBContainer &paContainer,
                                           TForteUInt8 paParentAdapterlistID) :
      FORTE_WagoRegCom(paContainer, cFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
      conn_Opened(*this, 0),
      conn_CNF(*this, 1),
      conn_Closed(*this, 2),
      conn_Error(*this, 3),
      conn_autoPsw(nullptr),
      conn_cmd(nullptr),
      conn_STATUS(*this, 0, var_STATUS),
      conn_counter(*this, 1, var_counter),
      conn_REG_D0(*this, 2, var_REG_D0),
      conn_REG_D1(*this, 3, var_REG_D1) {
  }

  void FORTE_WagoRegCom_Plug::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventOpenID: {
        readData(4, var_autoPsw, conn_autoPsw);
        if(auto peer = static_cast<FORTE_WagoRegCom_Socket *>(getPeer()); peer) {
          peer->var_autoPsw = var_autoPsw;
        }
        break;
      }
      case scmEventReadID: {
        readData(5, var_cmd, conn_cmd);
        if(auto peer = static_cast<FORTE_WagoRegCom_Socket *>(getPeer()); peer) {
          peer->var_cmd = var_cmd;
        }
        break;
      }
      case scmEventWriteID: {
        readData(5, var_cmd, conn_cmd);
        if(auto peer = static_cast<FORTE_WagoRegCom_Socket *>(getPeer()); peer) {
          peer->var_cmd = var_cmd;
        }
        break;
      }
      default:
        break;
    }
  }

  void FORTE_WagoRegCom_Plug::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventOpenedID: {
        writeData(0, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventCNFID: {
        writeData(1, var_counter, conn_counter);
        writeData(2, var_REG_D0, conn_REG_D0);
        writeData(3, var_REG_D1, conn_REG_D1);
        writeData(0, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventClosedID: {
        writeData(0, var_STATUS, conn_STATUS);
        writeData(1, var_counter, conn_counter);
        break;
      }
      case scmEventErrorID: {
        writeData(0, var_STATUS, conn_STATUS);
        break;
      }
      default:
        break;
    }
  }
  CIEC_ANY *FORTE_WagoRegCom_Plug::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_autoPsw;
      case 1: return &var_cmd;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_WagoRegCom_Plug::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_STATUS;
      case 1: return &var_counter;
      case 2: return &var_REG_D0;
      case 3: return &var_REG_D1;
    }
    return nullptr;
  }

  CEventConnection *FORTE_WagoRegCom_Plug::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_Opened;
      case 1: return &conn_CNF;
      case 2: return &conn_Closed;
      case 3: return &conn_Error;
    }
    return nullptr;
  }

  CDataConnection **FORTE_WagoRegCom_Plug::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_autoPsw;
      case 1: return &conn_cmd;
    }
    return nullptr;
  }

  CDataConnection *FORTE_WagoRegCom_Plug::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_STATUS;
      case 1: return &conn_counter;
      case 2: return &conn_REG_D0;
      case 3: return &conn_REG_D1;
    }
    return nullptr;
  }


  FORTE_WagoRegCom_Socket::FORTE_WagoRegCom_Socket(StringId paInstanceNameId,
                                           CFBContainer &paContainer,
                                           TForteUInt8 paParentAdapterlistID) :
      FORTE_WagoRegCom(paContainer, cFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
      conn_Open(*this, 0),
      conn_Read(*this, 1),
      conn_Write(*this, 2),
      conn_Close(*this, 3),
      conn_STATUS(nullptr),
      conn_counter(nullptr),
      conn_REG_D0(nullptr),
      conn_REG_D1(nullptr),
      conn_autoPsw(*this, 0, var_autoPsw),
      conn_cmd(*this, 1, var_cmd) {
  }

  void FORTE_WagoRegCom_Socket::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventOpenedID: {
        readData(0, var_STATUS, conn_STATUS);
        if(auto peer = static_cast<FORTE_WagoRegCom_Plug *>(getPeer()); peer) {
          peer->var_STATUS = var_STATUS;
        }
        break;
      }
      case scmEventCNFID: {
        readData(1, var_counter, conn_counter);
        readData(2, var_REG_D0, conn_REG_D0);
        readData(3, var_REG_D1, conn_REG_D1);
        readData(0, var_STATUS, conn_STATUS);
        if(auto peer = static_cast<FORTE_WagoRegCom_Plug *>(getPeer()); peer) {
          peer->var_counter = var_counter;
          peer->var_REG_D0 = var_REG_D0;
          peer->var_REG_D1 = var_REG_D1;
          peer->var_STATUS = var_STATUS;
        }
        break;
      }
      case scmEventClosedID: {
        readData(0, var_STATUS, conn_STATUS);
        readData(1, var_counter, conn_counter);
        if(auto peer = static_cast<FORTE_WagoRegCom_Plug *>(getPeer()); peer) {
          peer->var_STATUS = var_STATUS;
          peer->var_counter = var_counter;
        }
        break;
      }
      case scmEventErrorID: {
        readData(0, var_STATUS, conn_STATUS);
        if(auto peer = static_cast<FORTE_WagoRegCom_Plug *>(getPeer()); peer) {
          peer->var_STATUS = var_STATUS;
        }
        break;
      }
      default:
        break;
    }
  }

  void FORTE_WagoRegCom_Socket::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventOpenID: {
        writeData(4, var_autoPsw, conn_autoPsw);
        break;
      }
      case scmEventReadID: {
        writeData(5, var_cmd, conn_cmd);
        break;
      }
      case scmEventWriteID: {
        writeData(5, var_cmd, conn_cmd);
        break;
      }
      default:
        break;
    }
  }
  CIEC_ANY *FORTE_WagoRegCom_Socket::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_STATUS;
      case 1: return &var_counter;
      case 2: return &var_REG_D0;
      case 3: return &var_REG_D1;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_WagoRegCom_Socket::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_autoPsw;
      case 1: return &var_cmd;
    }
    return nullptr;
  }

  CEventConnection *FORTE_WagoRegCom_Socket::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_Open;
      case 1: return &conn_Read;
      case 2: return &conn_Write;
      case 3: return &conn_Close;
    }
    return nullptr;
  }

  CDataConnection **FORTE_WagoRegCom_Socket::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_STATUS;
      case 1: return &conn_counter;
      case 2: return &conn_REG_D0;
      case 3: return &conn_REG_D1;
    }
    return nullptr;
  }

  CDataConnection *FORTE_WagoRegCom_Socket::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_autoPsw;
      case 1: return &conn_cmd;
    }
    return nullptr;
  }

}
