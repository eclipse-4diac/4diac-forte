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
#include "opcua_handler_abstract.h"
#include <forte_printer.h>
#include <devlog.h>
#include <parameterParser.h>

//we tried to use the logLevelNames and logCategoryNames as extern from open62541 but it failed when using with shared libray
const char *LogLevelNames[6] = { "trace", "debug", "info", "warning", "error", "fatal" };
const char *LogCategoryNames[7] = { "network", "channel", "session", "server", "client", "userland" };

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
const UA_Logger COPC_UA_HandlerAbstract::UA_Forte_logger = {UA_Log_Forte, 0, UA_Log_Forte_clear};
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH

COPC_UA_HandlerAbstract::COPC_UA_HandlerAbstract(CDeviceExecution& paDeviceExecution) :
    CExternalEventHandler(paDeviceExecution) {
}

COPC_UA_HandlerAbstract::~COPC_UA_HandlerAbstract() {
  //do nothing
}

void COPC_UA_HandlerAbstract::triggerNewEvent(CEventSourceFB &paLayer) {
  startNewEventChain(&paLayer);
}

void COPC_UA_HandlerAbstract::setPriority(int) {
  //currently we are doing nothing here.
}

int COPC_UA_HandlerAbstract::getPriority() const {
  //the same as for setPriority
  return 0;
}

UA_Logger COPC_UA_HandlerAbstract::getLogger() {
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  return UA_Forte_logger;
#else //FORTE_COM_OPC_UA_MASTER_BRANCH
  return UA_Log_Forte;
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
}

void COPC_UA_HandlerAbstract::UA_Log_Forte(
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    void*,
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
    UA_LogLevel paLevel, UA_LogCategory paCategory, const char *paMsg, va_list paArgs) {

  char tmpStr[mMaxLogLenght];
  forte_snprintf(tmpStr, mMaxLogLenght, "[OPC UA LOGGER] %s/%s\t", LogLevelNames[paLevel], LogCategoryNames[paCategory]);
  char *start = &tmpStr[strlen(tmpStr)];

  forte_vsnprintf(start, mMaxLogLenght, paMsg, paArgs);

  size_t len = strlen(tmpStr);
  tmpStr[len] = '\n';
  tmpStr[len + 1] = '\0';

  switch(paLevel){
    case UA_LOGLEVEL_TRACE:
    case UA_LOGLEVEL_DEBUG:
      DEVLOG_DEBUG(tmpStr);
      break;
    case UA_LOGLEVEL_INFO:
      DEVLOG_INFO(tmpStr);
      break;
    case UA_LOGLEVEL_WARNING:
      DEVLOG_WARNING(tmpStr);
      break;
    case UA_LOGLEVEL_ERROR:
    case UA_LOGLEVEL_FATAL:
      DEVLOG_ERROR(tmpStr);
      break;
    default:
      break;
  }
}

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
void COPC_UA_HandlerAbstract::UA_Log_Forte_clear(void *) {
  //do nothing
}
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH


