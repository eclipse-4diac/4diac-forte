/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <string.h>
#include "forte/util/criticalregion.h"
#include "forte/cominfra/commfb.h"
#include "core/fmi/comm/fmuHandler.h"
#include "core/fmi/fmuInstance.h"

std::vector<fmuComLayer *> fmuHandler::mlayers;

fmuHandler::fmuHandler(CDeviceExecution &paDeviceExecution) : CExternalEventHandler(paDeviceExecution) {
}

fmuHandler::~fmuHandler() {
}

void fmuHandler::fmuMessageArrived(void *pa_value) {

  if (0 != pa_value) {
    for (fmuComLayer *layer : mlayers) {
      bool found = false;
      for (std::vector<fmuValueContainer *>::iterator itInputs = (*layer->getOutputs()).begin();
           itInputs != (*layer->getOutputs()).end(); ++itInputs) {
        if (pa_value == (*itInputs)) {
          found = true;
          if (forte::com_infra::e_Nothing != layer->recvData(pa_value, 0)) {
            ::getExtEvHandler<fmuHandler>(*layer->getCommFB()).startNewEventChain(layer->getCommFB());
          }
          break;
        }
      }
      if (found) {
        break;
      }
    }
  }
}

int fmuHandler::registerLayer(fmuComLayer *paLayer) {
  if (paLayer != 0) {
    mlayers.push_back(paLayer);
    return eRegisterLayerSucceeded;
  }
  return eWrongLayer;
}

void fmuHandler::unregisterLayer(fmuComLayer *paLayer) {
  mlayers.erase(std::remove(mlayers.begin(), mlayers.end(), paLayer), mlayers.end());
}

void fmuHandler::enableHandler() {
  // TODO: Should also work empty
}

void fmuHandler::disableHandler() {
  // TODO: Should also work empty
}
