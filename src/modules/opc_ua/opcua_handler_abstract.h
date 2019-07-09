/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/
#ifndef SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_
#define SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_

#include <extevhan.h>
#include <esfb.h>
#include <opcua_action_info.h>


class COPC_UA_HandlerAbstract : public CExternalEventHandler {
  public:

    explicit COPC_UA_HandlerAbstract(CDeviceExecution& paDeviceExecution);

    virtual ~COPC_UA_HandlerAbstract();

    void triggerNewEvent(CEventSourceFB &paLayer);

    virtual UA_StatusCode initializeAction(CActionInfo& paActionInfo) = 0;

    virtual UA_StatusCode executeAction(CActionInfo& paActionInfo) = 0;

    virtual UA_StatusCode uninitializeAction(CActionInfo& paActionInfo) = 0;

    void setPriority(int paPriority);

    int getPriority() const;

    static UA_Logger getLogger();

  private:

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    static void UA_Log_Forte_clear(void *paLogContext);
    static const UA_Logger UA_Forte_logger;
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH

    static void UA_Log_Forte(
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
        void*,
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
        UA_LogLevel paLevel, UA_LogCategory paCategory, const char *paMsg, va_list paArgs);

    static const size_t mMaxLogLenght = 400;
};

#endif /* SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_ */
