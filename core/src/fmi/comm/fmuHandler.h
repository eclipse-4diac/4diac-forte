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

#ifndef FMUHANDLER_H_
#define FMUHANDLER_H_

#include "forte/util/singlet.h"
#include "forte/extevhan.h"
#include "core/fmi/comm/fmuComLayer.h"
#include "forte/arch/forte_sync.h"
#include "forte/datatypes/forte_string.h"
#include <map>
#include <vector>

class fmuHandler : public CExternalEventHandler, public RegisterExternalEventHandler<fmuHandler> {
  public:
    explicit fmuHandler(CDeviceExecution &paDeviceExecution);
    ~fmuHandler() override;

    enum RegisterLayerReturnCodes { eRegisterLayerSucceeded, eWrongLayer };
    int registerLayer(fmuComLayer *paLayer);

    void unregisterLayer(fmuComLayer *paLayer);

    void fmuMessageProcessed();

    void enableHandler() override;
    /*!\brief Disable this event source
     */
    void disableHandler() override;

    static void fmuMessageArrived(void *pa_value);

    std::map<CFunctionBlock *, std::vector<fmuValueContainer *> *> *getOutputMap() {
      return &outputMap;
    }

    std::map<CFunctionBlock *, std::vector<fmuValueContainer *> *> *getInputMap() {
      return &inputMap;
    }

  private:
    static std::vector<fmuComLayer *> mlayers;
    std::map<CFunctionBlock *, std::vector<fmuValueContainer *> *> outputMap;
    std::map<CFunctionBlock *, std::vector<fmuValueContainer *> *> inputMap;
};

#endif /* fmuHANDLER_H_ */
