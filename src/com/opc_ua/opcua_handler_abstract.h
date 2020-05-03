/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/
#ifndef SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_
#define SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_

#include <extevhan.h>
#include <esfb.h>
#include <opcua_action_info.h>

/**
 * Base class for the local and remote handler. It defines the three major function to be executed in an action (initialize, execute, uninitialize)
 */
class COPC_UA_HandlerAbstract : public CExternalEventHandler {
  public:

    /**
     * Constructor of the class
     */
    explicit COPC_UA_HandlerAbstract(CDeviceExecution &paDeviceExecution);

    /**
     * Destructor of the class
     */
    virtual ~COPC_UA_HandlerAbstract();

    /**
     * Trigger a new incoming event in the FB
     * @param paLayer The communication FB of the layer which received data
     */
    void triggerNewEvent(CEventSourceFB &paLayer);

    /**
     * Perform all the needed initialization of the action
     * @param paActionInfo Action to be initialized
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    virtual UA_StatusCode initializeAction(CActionInfo &paActionInfo) = 0;

    /**
     * Execute the action
     * @param paActionInfo Action to be executed
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    virtual UA_StatusCode executeAction(CActionInfo &paActionInfo) = 0;

    /**
     * Uninitialize the action
     * @param paActionInfo Action to be Uninitialized
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    virtual UA_StatusCode uninitializeAction(CActionInfo &paActionInfo) = 0;

    /**
     * Not used
     */
    void setPriority(int paPriority);

    /**
     * Not used
     */
    int getPriority() const;

    /**
     * Get the logger used by the the OPC UA stack
     * @return Logger used by the OPC UA stack
     */
    static UA_Logger getLogger();

  private:

    static void UA_Log_Forte_clear(void *paLogContext);
    static const UA_Logger UA_Forte_logger;

    static void UA_Log_Forte(void*, UA_LogLevel paLevel, UA_LogCategory paCategory, const char *paMsg, va_list paArgs);

    /**
     * Maximum size of the logging buffer
     */
    static const size_t mMaxLogLength = 400;
};

#endif /* SRC_MODULES_OPC_UA_OPCUA_HANDLER_ABSTRACT_H_ */
