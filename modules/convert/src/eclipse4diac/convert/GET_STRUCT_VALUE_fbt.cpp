/*******************************************************************************
 * Copyright (c) 2018, 2023 fortiss GmbH, Primetals Technologies Austria GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Alois Zoitl - reworked to make helpers usable for SET_STRUCT_VALUE
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "forte/eclipse4diac/convert/GET_STRUCT_VALUE_fbt.h"

using namespace forte::literals;

namespace forte::eclipse4diac::convert {
  namespace {
    const auto cDataInputNames = std::array{"in_struct"_STRID, "member"_STRID};

    const auto cDataOutputNames = std::array{"QO"_STRID, "output"_STRID};

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};

    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_GET_STRUCT_VALUE, "eclipse4diac::convert::GET_STRUCT_VALUE"_STRID)

  FORTE_GET_STRUCT_VALUE::FORTE_GET_STRUCT_VALUE(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_in_struct(CIEC_ANY_VARIANT()),
      var_member("s"_STRING),
      var_QO(false_BOOL),
      var_output(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_in_struct(nullptr),
      conn_member(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_output(*this, 1, var_output) {};

  CIEC_ANY *FORTE_GET_STRUCT_VALUE::lookForMember(CIEC_STRUCT &paWhereToLook, char *paMemberName) {
    char *nameSeparator = strchr(paMemberName, '.');
    if (nameSeparator != nullptr) {
      *nameSeparator = '\0';
    }
    CIEC_ANY *member = paWhereToLook.getMemberNamed(paMemberName);
    if (nameSeparator != nullptr && member != nullptr) {
      if (member->getDataTypeID() == CIEC_ANY::e_STRUCT) {
        member = lookForMember(static_cast<CIEC_STRUCT &>(*member), nameSeparator + 1);
      } else {
        member = nullptr;
      }
    }
    return member;
  }

  void FORTE_GET_STRUCT_VALUE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        if (std::holds_alternative<CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>>(var_in_struct)) {
          auto &inStruct = std::get<CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>>(var_in_struct);
          std::string memberName(var_member.getStorage()); // will be modified by lookForMember
          CIEC_ANY *member = lookForMember(*inStruct, memberName.data());
          if (nullptr != member) {
            var_output.setValue(*member);
            var_QO = true_BOOL;
          } else {
            DEVLOG_ERROR("[GET_STRUCT_VALUE]: In instance %s, member %s was not found\n", getInstanceName(),
                         var_member.getStorage().c_str());
            var_QO = false_BOOL;
          }
        } else {
          DEVLOG_ERROR(
              "[GET_STRUCT_VALUE]: In instance %s, the input structure is not of type structure but of type %d\n",
              getInstanceName(), var_in_struct.unwrap().getDataTypeID());
          var_QO = false_BOOL;
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_GET_STRUCT_VALUE::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(1, var_member, conn_member);
        readData(0, var_in_struct, conn_in_struct);
        break;
      }
      default: break;
    }
  }

  void FORTE_GET_STRUCT_VALUE::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_output, conn_output);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_GET_STRUCT_VALUE::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_in_struct;
      case 1: return &var_member;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_GET_STRUCT_VALUE::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_output;
    }
    return nullptr;
  }

  CEventConnection *FORTE_GET_STRUCT_VALUE::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_GET_STRUCT_VALUE::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_in_struct;
      case 1: return &conn_member;
    }
    return nullptr;
  }

  CDataConnection *FORTE_GET_STRUCT_VALUE::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_output;
    }
    return nullptr;
  }

  void FORTE_GET_STRUCT_VALUE::setInitialValues() {
    var_in_struct.reset();
    var_member = "s"_STRING;
    var_QO = false_BOOL;
    var_output.reset();
  }

} // namespace forte::eclipse4diac::convert
