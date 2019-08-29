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


#ifndef FMUCOMLAYER_H_
#define FMUCOMLAYER_H_


#include "../../cominfra/comlayer.h"
#include "../../fortelist.h"
#include "../processinterface.h"
#include <vector>



using namespace forte::com_infra;

class fmuComLayer: public forte::com_infra::CComLayer{
public:
  fmuComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poFB);
  virtual ~fmuComLayer();

  EComResponse sendData(void* pa_pvData, unsigned int pa_unSize);

  EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);

  std::vector<fmuValueContainer*>* getOutputs() const {
    return m_outputs;
  }

  EComResponse processInterrupt();

protected:
  void closeConnection();

private:
  //Inputs and outputs of the FB, not of the FMU interface. Inputs of the FB are outputs to the FMI
  std::vector<fmuValueContainer*>* m_outputs;
  std::vector<fmuValueContainer*>* m_inputs;

  //FMI Inputs arrived
  std::vector<bool> m_inputsArrived;


  bool m_someValueChanged;

  EComResponse m_eInterruptResp;

  EComResponse openConnection(char* pa_acLayerParameter);

};

#endif /* fmuCOMLAYER_H_ */


