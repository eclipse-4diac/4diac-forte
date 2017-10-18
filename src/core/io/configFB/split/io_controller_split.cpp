/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_controller_split.h"

namespace IO {
namespace ConfigurationFB {

const char * const SplitController::scmFailedToInitParts =
    "Failed to initialize parts.";

TControllerList* SplitController::instances = new TControllerList();
TForteUInt16 SplitController::instancesIncrement = 0;

SplitController::SplitController(const TForteUInt8* const scm_splitAdapter,
    const TForteUInt8 scm_splitAdapter_num, CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    Controller(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId,
        pa_acFBConnData, pa_acFBVarsData), scm_splitAdapter(scm_splitAdapter), scm_splitAdapter_num(
        scm_splitAdapter_num), splitIterator(0) {
  id = instancesIncrement++;
  instances->push_back(this);
}

SplitController* SplitController::getControllerById(TForteUInt16 id) {
  TControllerList::Iterator itEnd = instances->end();
  int i = 0;
  for (TControllerList::Iterator it = instances->begin(); it != itEnd;
      ++it, i++)
    if (id == i && *it != 0)
      return *it;
  return 0;
}

void SplitController::onStartup() {
  splitIterator = 0;

  if (!scm_splitAdapter_num) {
    return Controller::onStartup();
  }

  SplitAdapter *cur =
      static_cast<SplitAdapter*>(m_apoAdapters[scm_splitAdapter[splitIterator]]);

  while (!cur || cur->getPeer() == 0) {
    splitIterator++;

    if (splitIterator == scm_splitAdapter_num) {
      return Controller::onStartup();
    }

    cur =
        static_cast<SplitAdapter*>(m_apoAdapters[scm_splitAdapter[splitIterator]]);
  }

  cur->MasterId() = id;
  cur->QI() = true;
  sendAdapterEvent(scm_splitAdapter[splitIterator],
      SplitAdapter::scm_nEventINITID);
}

void SplitController::onStop() {
  splitIterator = 0;

  if (!scm_splitAdapter_num) {
    return Controller::onStop();
  }

  SplitAdapter *cur =
      static_cast<SplitAdapter*>(m_apoAdapters[scm_splitAdapter[splitIterator]]);

  while (cur->getPeer() == 0) {
    splitIterator++;

    if (splitIterator == scm_splitAdapter_num) {
      return Controller::onStop();
    }

    cur =
        static_cast<SplitAdapter*>(m_apoAdapters[scm_splitAdapter[splitIterator]]);
  }

  cur->QI() = false;
  sendAdapterEvent(scm_splitAdapter[splitIterator],
      SplitAdapter::scm_nEventINITID);
}

void SplitController::executeEvent(int pa_nEIID) {
  Controller::executeEvent(pa_nEIID);

  if (splitIterator < scm_splitAdapter_num) {
    SplitAdapter *cur =
        static_cast<SplitAdapter*>(m_apoAdapters[scm_splitAdapter[splitIterator]]);
    if (cur->INITO() == pa_nEIID) {

      QO() = cur->QO();

      splitIterator++;

      if (splitIterator == scm_splitAdapter_num || (cur->QI() && !cur->QO())) {
        if (cur->QI() == true) {
          if (cur->QO() == true) {
            Controller::onStartup();
          } else {
            started(scmFailedToInitParts);
          }
        } else {
          Controller::onStop();
        }
      } else {
        SplitAdapter *next =
            static_cast<SplitAdapter*>(m_apoAdapters[scm_splitAdapter[splitIterator]]);

        while (!next || next->getPeer() == 0) {
          splitIterator++;

          if (splitIterator == scm_splitAdapter_num) {
            if (cur->QO() == true) {
              return Controller::onStartup();
            } else {
              return Controller::onStop();
            }
          }

          next =
              static_cast<SplitAdapter*>(m_apoAdapters[scm_splitAdapter[splitIterator]]);
        }

        next->MasterId() = id;
        next->QI() = cur->QI();

        sendAdapterEvent(scm_splitAdapter[splitIterator],
            SplitAdapter::scm_nEventINITID);
      }
    }
  }
}

} /* namespace ConfigurationFB */
} /* namespace IO */
