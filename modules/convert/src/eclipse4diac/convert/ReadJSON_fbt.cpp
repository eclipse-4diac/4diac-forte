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

#include "forte/eclipse4diac/convert/ReadJSON_fbt.h"

#include "forte/forte_st_util.h"
#include <string>
#include <regex>

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::convert {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"JSONIn"_STRID, "FieldName"_STRID};
    const auto cDataOutputNames = std::array{"Value"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_ReadJSON, "eclipse4diac::convert::ReadJSON"_STRID, TypeHash)

  FORTE_ReadJSON::FORTE_ReadJSON(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_JSONIn(""_STRING),
      var_FieldName(""_STRING),
      var_Value(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_JSONIn(nullptr),
      conn_FieldName(nullptr),
      conn_Value(*this, 0, var_Value) {
  };

  void FORTE_ReadJSON::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_JSONIn = ""_STRING;
    var_FieldName = ""_STRING;
    var_Value = CIEC_ANY_VARIANT();
  }

void FORTE_ReadJSON::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch(paEIID) {
        case scmEventREQID: {
            alg_REQ();
            writeOutputData(scmEventCNFID);
            sendOutputEvent(scmEventCNFID, paECET);
            break;
        }
        default:
            break;
    }
}

  void FORTE_ReadJSON::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_JSONIn, conn_JSONIn);
        readData(1, var_FieldName, conn_FieldName);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_ReadJSON::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(2, var_Value, conn_Value);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_ReadJSON::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_JSONIn;
      case 1: return &var_FieldName;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ReadJSON::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_Value;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ReadJSON::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ReadJSON::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_JSONIn;
      case 1: return &conn_FieldName;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ReadJSON::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_Value;
    }
    return nullptr;
  }

void FORTE_ReadJSON::alg_REQ(void){
    std::string JSONInStr = var_JSONIn.c_str();
    std::string field = var_FieldName.c_str();

    std::string pattern = "\""+field+"\"\\s*:\\s*(\"[^\"]*\"|\\{[^}]*\\}|\\d+\\.?\\d*|true|false|null)";
    std::regex re(pattern);
    std::smatch match;

    if(std::regex_search(JSONInStr, match, re)) {
        std::string valueStr = match[1].str();

if(valueStr == "true" || valueStr == "false") {
    CIEC_BOOL tmpBool(valueStr == "true");
    var_Value = CIEC_ANY_VARIANT(tmpBool);
        } else {
            bool isString = !valueStr.empty() && valueStr.front() == '"' && valueStr.back() == '"';
            if(isString) {
                valueStr = valueStr.substr(1, valueStr.size()-2);
            }
            CIEC_STRING tmpString;
            tmpString.assign(valueStr.c_str(), static_cast<TForteUInt16>(valueStr.size()));
            var_Value = CIEC_ANY_VARIANT(tmpString);
        }

    } else {
        var_Value = CIEC_ANY_VARIANT();
    }
}

} // namespace forte::eclipse4diac::convert