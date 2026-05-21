/*******************************************************************************
 * Copyright (c) 2026 Thomas Mayr
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Mayr - Initial implementation.
 *******************************************************************************/

#include "forte/eclipse4diac/convert/ToJSON_fbt.h"
#include "forte/forte_st_util.h"
#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_string.h"
#include "forte/forte_st_util.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions/func_ANY_AS_STRING.h"


using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::convert {
  
  namespace {

  std::string escapeJSONString(const std::string &input) {
          std::string result;
          for (char c : input) {
              switch(c) {
                  case '"': result += "\\\""; break;
                  case '\\': result += "\\\\"; break;
                  case '\b': result += "\\b"; break;
                  case '\f': result += "\\f"; break;
                  case '\n': result += "\\n"; break;
                  case '\r': result += "\\r"; break;
                  case '\t': result += "\\t"; break;
                  default:
                      if (static_cast<unsigned char>(c) < 0x20) {
                          char buf[7];
                          snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
                          result += buf;
                      } else {
                          result += c;
                      }
              }
          }
          return result;
      }

    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"JSONIn"_STRID, "Value"_STRID, "FieldName"_STRID};
    const auto cDataOutputNames = std::array{"JSONOut"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  }

  DEFINE_FIRMWARE_FB(FORTE_ToJSON, "eclipse4diac::convert::ToJSON"_STRID, TypeHash)

  FORTE_ToJSON::FORTE_ToJSON(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_JSONIn(""_STRING),
      var_Value(CIEC_ANY_VARIANT()),
      var_FieldName(""_STRING),
      var_JSONOut(""_STRING),
      conn_CNF(*this, 0),
      conn_JSONIn(nullptr),
      conn_Value(nullptr),
      conn_FieldName(nullptr),
      conn_JSONOut(*this, 0, var_JSONOut) {
  };

  void FORTE_ToJSON::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_JSONIn = ""_STRING;
    var_Value = CIEC_ANY_VARIANT();
    var_FieldName = ""_STRING;
    var_JSONOut = ""_STRING;
  }

  void FORTE_ToJSON::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch(paEIID) {
        case scmEventREQID: {
            alg_REQ();
            sendOutputEvent(scmEventCNFID, paECET);
            break;
        }
        default:
            break;
    }
  }

  void FORTE_ToJSON::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_JSONIn, conn_JSONIn);
        readData(1, var_Value, conn_Value);
        readData(2, var_FieldName, conn_FieldName);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_ToJSON::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(3, var_JSONOut, conn_JSONOut);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_ToJSON::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_JSONIn;
      case 1: return &var_Value;
      case 2: return &var_FieldName;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ToJSON::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_JSONOut;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ToJSON::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ToJSON::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_JSONIn;
      case 1: return &conn_Value;
      case 2: return &conn_FieldName;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ToJSON::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_JSONOut;
    }
    return nullptr;
  }

void FORTE_ToJSON::alg_REQ(void){
    const CIEC_STRING &strValue = func_ANY_AS_STRING(var_Value);
    std::string tmp = strValue.getStorage();
    if(!tmp.empty() && tmp.front() == '\'') tmp.erase(0,1);
    if(!tmp.empty() && tmp.back() == '\'') tmp.pop_back();

    std::string valueStr;

    if(tmp == "TRUE") {
        valueStr = "true";
    } else if(tmp == "FALSE") {
        valueStr = "false";
    } else {
        bool isNumber = true;
        bool hasDot = false;
        for(char c : tmp){
            if((c >= '0' && c <= '9') || c=='-' || c=='+') continue;
            if(c=='.' && !hasDot){ hasDot = true; continue; }
            isNumber = false;
            break;
        }
        if(isNumber) {
            valueStr = tmp;
        } else {
            valueStr = "\"" + escapeJSONString(tmp) + "\"";
        }
    }

    std::string JSONOutStd;
    if(var_JSONIn == ""){
        JSONOutStd = "{\"" + std::string(var_FieldName.c_str(), var_FieldName.length()) + "\":" + valueStr + "}";
    } else {
        JSONOutStd = std::string(var_JSONIn.c_str(), var_JSONIn.length());
        if(!JSONOutStd.empty() && JSONOutStd.back() == '}'){
            JSONOutStd.pop_back();
            JSONOutStd += ", \"" + std::string(var_FieldName.c_str(), var_FieldName.length()) + "\":" + valueStr + "}";
        }
    }
    var_JSONOut = CIEC_STRING(JSONOutStd);
}

} // namespace forte::eclipse4diac::convert