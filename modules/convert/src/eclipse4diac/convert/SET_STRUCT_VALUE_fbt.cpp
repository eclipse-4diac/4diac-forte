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
 *   Alois Zoitl - adapted this FB from the code in GET_STRUCT_VALUE
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "forte/eclipse4diac/convert/SET_STRUCT_VALUE_fbt.h"

using namespace forte::literals;

namespace forte::eclipse4diac::convert {
  namespace {
    const auto cDataInputNames = std::array{"in_struct"_STRID, "member"_STRID, "element_value"_STRID};
    const auto cDataOutputNames = std::array{"out_struct"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_SET_STRUCT_VALUE, "eclipse4diac::convert::SET_STRUCT_VALUE"_STRID)

  FORTE_SET_STRUCT_VALUE::FORTE_SET_STRUCT_VALUE(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_in_struct(CIEC_ANY_VARIANT()),
      var_member(""_STRING),
      var_element_value(CIEC_ANY_VARIANT()),
      var_out_struct(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_in_struct(nullptr),
      conn_member(nullptr),
      conn_element_value(nullptr),
      conn_out_struct(*this, 0, var_out_struct) {};

  CIEC_ANY *FORTE_SET_STRUCT_VALUE::lookForMember(CIEC_STRUCT &paWhereToLook, char *paMemberName) {
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

  void FORTE_SET_STRUCT_VALUE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_out_struct = var_in_struct;
        if (std::holds_alternative<CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>>(var_out_struct)) {
          auto &outStruct = std::get<CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>>(var_out_struct);
          std::string memberName(var_member.getStorage()); // will be modified by lookForMember
          CIEC_ANY *member = lookForMember(*outStruct, memberName.data());
          if (nullptr != member) {
            if (member->getDataTypeID() == var_element_value.unwrap().getDataTypeID()) {
              member->setValue(var_element_value.unwrap());
            } else {
              DEVLOG_ERROR("[SET_STRUCT_VALUE]: In instance %s, the member %s was found but it doesn't match the given "
                           "type %d\n",
                           getInstanceName(), var_member.getStorage().c_str(),
                           var_element_value.unwrap().getDataTypeID());
            }
          } else {
            DEVLOG_ERROR("[SET_STRUCT_VALUE]: In instance %s, member %s was not found\n", getInstanceName(),
                         var_member.getStorage().c_str());
          }
        } else {
          DEVLOG_ERROR(
              "[SET_STRUCT_VALUE]: In instance %s, the input structure is not of type structure but of type %d\n",
              getInstanceName(), var_out_struct.unwrap().getDataTypeID());
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_SET_STRUCT_VALUE::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(1, var_member, conn_member);
        readData(0, var_in_struct, conn_in_struct);
        readData(2, var_element_value, conn_element_value);
        break;
      }
      default: break;
    }
  }

  void FORTE_SET_STRUCT_VALUE::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_out_struct, conn_out_struct);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SET_STRUCT_VALUE::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_in_struct;
      case 1: return &var_member;
      case 2: return &var_element_value;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SET_STRUCT_VALUE::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_out_struct;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SET_STRUCT_VALUE::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SET_STRUCT_VALUE::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_in_struct;
      case 1: return &conn_member;
      case 2: return &conn_element_value;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SET_STRUCT_VALUE::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_out_struct;
    }
    return nullptr;
  }

  void FORTE_SET_STRUCT_VALUE::setInitialValues() {
    var_in_struct.reset();
    var_member = ""_STRING;
    var_element_value.reset();
    var_out_struct.reset();
  }

} // namespace forte::eclipse4diac::convert
