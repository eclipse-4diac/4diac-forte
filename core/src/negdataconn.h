/*******************************************************************************
 * Copyright (c) 2025 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *******************************************************************************/
#pragma once

#include "delegdataconn.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions/func_NOT.h"

namespace forte::internal {

  class CNegatingDataConnection : public CDelegatingDataConnection {

    public:
      CNegatingDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, Wrapper paDelegate) :
          CDelegatingDataConnection(paSrcFB, paSrcPortId, paDelegate->getValue()),
          mDelegate(std::move(paDelegate)) {
      }

      void readData(CIEC_ANY &paValue) const override {
        CIEC_BOOL value;
        mDelegate->readData(value);
        paValue.setValue(func_NOT(value));
      }

      void getSourcePortName(TNameIdentifier &paResult) const override;

      CIEC_ANY::EDataTypeID getDataTypeID() const override {
        return CIEC_ANY::e_BOOL;
      }

    private:
      Wrapper mDelegate;
  };
} // namespace forte::internal
