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

#pragma once

#include "forte/adapter.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_wstring.h"
#include "WagoRegComCmd_dtp.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::io::wago {
  class FORTE_WagoRegCom : public forte::CAdapter {
      DECLARE_ADAPTER_TYPE(FORTE_WagoRegCom)

    private:
      void setInitialValues() override;

    public:
      static const TEventID scmEventOpenID = 0;
      static const TEventID scmEventReadID = 1;
      static const TEventID scmEventWriteID = 2;
      static const TEventID scmEventCloseID = 3;

      static const TEventID scmEventOpenedID = 0;
      static const TEventID scmEventCNFID = 1;
      static const TEventID scmEventClosedID = 2;
      static const TEventID scmEventErrorID = 3;

      CIEC_WSTRING var_STATUS;
      CIEC_USINT var_counter;
      CIEC_BYTE var_REG_D0;
      CIEC_BYTE var_REG_D1;

      CIEC_BOOL var_autoPsw;
      CIEC_WagoRegComCmd var_cmd;

      TEventID evt_Opened() {
        return getParentAdapterListEventID() + scmEventOpenedID;
      }

      TEventID evt_CNF() {
        return getParentAdapterListEventID() + scmEventCNFID;
      }

      TEventID evt_Closed() {
        return getParentAdapterListEventID() + scmEventClosedID;
      }

      TEventID evt_Error() {
        return getParentAdapterListEventID() + scmEventErrorID;
      }

      TEventID evt_Open() {
        return getParentAdapterListEventID() + scmEventOpenID;
      }

      TEventID evt_Read() {
        return getParentAdapterListEventID() + scmEventReadID;
      }

      TEventID evt_Write() {
        return getParentAdapterListEventID() + scmEventWriteID;
      }

      TEventID evt_Close() {
        return getParentAdapterListEventID() + scmEventCloseID;
      }

      ~FORTE_WagoRegCom() override = default;

    protected:
      FORTE_WagoRegCom(CFBContainer &paContainer,
                    const SFBInterfaceSpec &paInterfaceSpec,
                    const StringId paInstanceNameId,
                    TForteUInt8 paParentAdapterlistID);
  };

  class FORTE_WagoRegCom_Plug final : public FORTE_WagoRegCom {
    public:
      FORTE_WagoRegCom_Plug(StringId paInstanceNameId,
                          CFBContainer &paContainer,
                          TForteUInt8 paParentAdapterlistID);
      ~FORTE_WagoRegCom_Plug() override = default;

      CEventConnection conn_Opened;
      CEventConnection conn_CNF;
      CEventConnection conn_Closed;
      CEventConnection conn_Error;

      CDataConnection *conn_autoPsw;
      CDataConnection *conn_cmd;

      COutDataConnection<CIEC_WSTRING> conn_STATUS;
      COutDataConnection<CIEC_USINT> conn_counter;
      COutDataConnection<CIEC_BYTE> conn_REG_D0;
      COutDataConnection<CIEC_BYTE> conn_REG_D1;

    private:
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };

  class FORTE_WagoRegCom_Socket final : public FORTE_WagoRegCom {
    public:
      FORTE_WagoRegCom_Socket(StringId paInstanceNameId,
                          CFBContainer &paContainer,
                          TForteUInt8 paParentAdapterlistID);
      ~FORTE_WagoRegCom_Socket() override = default;

      CEventConnection conn_Open;
      CEventConnection conn_Read;
      CEventConnection conn_Write;
      CEventConnection conn_Close;

      CDataConnection *conn_STATUS;
      CDataConnection *conn_counter;
      CDataConnection *conn_REG_D0;
      CDataConnection *conn_REG_D1;

      COutDataConnection<CIEC_BOOL> conn_autoPsw;
      COutDataConnection<CIEC_WagoRegComCmd> conn_cmd;

    private:
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
}
