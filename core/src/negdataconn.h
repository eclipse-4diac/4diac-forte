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
#include "forte/iec61131_functions.h"

namespace forte::internal {

  class CNegatingDataConnection : public CDelegatingDataConnection<CIEC_BOOL> {

    public:
      CNegatingDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, CIEC_BOOL &paValue) :
          CDelegatingDataConnection<CIEC_BOOL>(paSrcFB, paSrcPortId, paValue) {
      }

      void readData(CIEC_ANY &paValue) const override {
        static_cast<CIEC_BOOL &>(paValue) = func_NOT(getValue());
      }

      void getSourcePortName(TNameIdentifier &paResult) const override;
  };
} // namespace forte::internal
