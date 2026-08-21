/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.100.qualifier!
 ***
 *** Name: WagoRegComCmd
 *** Description: Write register communication
 *** Version:
 ***     3.0: 2026-07-21/Monika Wenger -  -
 *************************************************************************/

#pragma once

#include "forte/datatypes/forte_struct.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_usint.h"


namespace forte::eclipse4diac::io::wago {
  class CIEC_WagoRegComCmd final : public CIEC_STRUCT {
    DECLARE_FIRMWARE_DATATYPE(WagoRegComCmd)

    public:
      CIEC_WagoRegComCmd();

      CIEC_WagoRegComCmd(const CIEC_USINT &paRegNr, const CIEC_BYTE &paREG_D0, const CIEC_BYTE &paREG_D1);

      CIEC_USINT var_RegNr;
      CIEC_BYTE var_REG_D0;
      CIEC_BYTE var_REG_D1;

      size_t getStructSize() const override {
        return 3;
      }

      const StringId* elementNames() const override {
        return scmElementNames;
      }

      StringId getStructTypeNameID() const override;

      void setValue(const CIEC_ANY &paValue) override;

      CIEC_ANY *getMember(size_t) override;
      const CIEC_ANY *getMember(size_t) const override;

    private:
      static const StringId scmElementNames[];

  };
}

