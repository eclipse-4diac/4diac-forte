/*************************************************************************
 * Copyright (c) 2026 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Mayr - initial implementation
 *******************************************************************************/
#pragma once

#include "forte/cominfra/comlayer.h"

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace forte {
  namespace com_infra {

    class CJSONComLayer : public CComLayer {

      public:
        CJSONComLayer(CComLayer *paUpperLayer, CBaseCommFB *paFB);
        ~CJSONComLayer() override;
        EComResponse sendData(void *paData, unsigned int paSize) override;
        EComResponse recvData(const void *paData, unsigned int paSize) override;

      protected:
        EComResponse openConnection(char *paLayerParameter) override;
        void closeConnection() override;

      private:
        bool serialize(const CIEC_ANY **paData, unsigned int paSize, std::string &paResult);
        bool serializeValue(nlohmann::ordered_json &output, const std::string &name, const CIEC_ANY &value);
        bool deserialize(const std::string &paInput);
        bool deserializeValue(const nlohmann::ordered_json &input, CIEC_ANY &value);
        std::string getFieldName(unsigned int paIndex) const;
        std::vector<std::string> mFieldNames;
    };

  } // namespace com_infra
} // namespace forte
