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

#include <singlet.h>
#include <extevhan.h>
#include <fortelist.h>
#include "fmuComLayer.h"
#include <forte_sync.h>
#include <forte_string.h>
#include <map>
#include <vector>

class fmuHandler : public CExternalEventHandler {
    DECLARE_HANDLER(fmuHandler)
public:
    enum RegisterLayerReturnCodes{
      eRegisterLayerSucceeded, eWrongLayer
    };
    int registerLayer(fmuComLayer* paLayer);

    void unregisterLayer(fmuComLayer* paLayer);

    void fmuMessageProcessed(void);

    virtual void enableHandler(void);
    /*!\brief Disable this event source
     */
    virtual void disableHandler(void);
    /*!\brief Sets the priority of the event source
     *
     * \param pa_nPriority new priority of the event source
     */
    virtual void setPriority(int pa_nPriority);
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority(void) const;
    static void fmuMessageArrived(void *pa_value);

    std::map<CFunctionBlock*, std::vector<fmuValueContainer*>*>* getOutputMap(){
      return &outputMap;
    }

    std::map<CFunctionBlock*, std::vector<fmuValueContainer*>*>* getInputMap(){
      return &inputMap;
    }

  private:

    static CSinglyLinkedList<fmuComLayer*> mlayers;
    std::map<CFunctionBlock*, std::vector<fmuValueContainer*>*> outputMap;
    std::map<CFunctionBlock*, std::vector<fmuValueContainer*>*> inputMap;

};

#endif /* fmuHANDLER_H_ */

