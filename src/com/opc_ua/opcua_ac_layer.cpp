

#include "opcua_ac_layer.h"
#include "../../core/utils/parameterParser.h"
#include "../../core/cominfra/basecommfb.h"
#include "opcua_local_handler.h"
#include "opcua_objectstruct_helper.h"

using namespace forte::com_infra;

const std::string COPC_UA_AC_Layer::scmModeINITMSG = "INITMSG";
const std::string COPC_UA_AC_Layer::scmModeINITUSERTEXT = "INITUSERTEXT";
const std::string COPC_UA_AC_Layer::scmTypeALARM = "Alarm";
const std::string COPC_UA_AC_Layer::scmTypeEVENT = "Event";

COPC_UA_AC_Layer::COPC_UA_AC_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
  CComLayer(paUpperLayer, paComFB), mHandler(nullptr) {
  
}

COPC_UA_AC_Layer::~COPC_UA_AC_Layer() {
}

EComResponse COPC_UA_AC_Layer::openConnection(char *paLayerParameter) {
  EComResponse eRetVal = e_InitTerminated;
  CParameterParser parser(paLayerParameter, ';', scmNumberOfParameters);
  size_t nrOfParams = parser.parseParameters();
  if(nrOfParams == scmNumberOfParameters) {
    std::string mode = parser[Mode];
    if(mode != scmModeINITMSG && mode != scmModeINITUSERTEXT) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong usage of layer parameters! Expected first param: %s or %s, actual: %s\n", scmModeINITMSG.c_str(), scmModeINITUSERTEXT.c_str(), mode.c_str());
      return eRetVal;
    }
    // TODO
    eRetVal = e_InitOk;
  }
  return eRetVal;
}

void COPC_UA_AC_Layer::closeConnection() {
  // TODO
}

EComResponse COPC_UA_AC_Layer::recvData(const void*, unsigned int) {
  return e_ProcessDataOk;
}

EComResponse COPC_UA_AC_Layer::sendData(void*, unsigned int) {
  return e_ProcessDataOk;
}

EComResponse COPC_UA_AC_Layer::processInterrupt() {
  // TODO
  return e_ProcessDataOk;
}