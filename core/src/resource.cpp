/*******************************************************************************
 * Copyright (c) 2005, 2023 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Johannes Kepler University,
 *                          Primetals Technologies Austria GmbH,
 *                          Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Thomas Strasser,
 *    Martin Melik Merkumians,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - modify for removed implicit constructors
 *         for primitive types
 *    Martin Jobst - add CTF tracing integration
 *    Fabio Gandolfi - send also subapps on requested resources
 *******************************************************************************/
#include "forte/resource.h"
#include "resource_internal.h"

#include <string_view>
#include "forte/conn.h"
#include "forte/datatypes/forte_any_string.h"
#include "forte/funcbloc.h"
#include "forte/mgmcmd.h"
#include "forte/adapterconn.h"
#include "forte/ecetfactory.h"
#include "forte/util/string_utils.h"
#include "monitoring.h"
#include "typelib_internal.h"

using namespace forte::literals;

#ifdef FORTE_DYNAMIC_TYPE_LOAD
#include "core/lua/luaadaptertypeentry.h"
#include "core/lua/luabfbtypeentry.h"
#include "core/lua/luacfbtypeentry.h"
#include "core/lua/luaengine.h"
#endif

#include <string>

using namespace std::string_literals;

namespace forte {
  namespace {

    template<typename T>
    void appendTypeNameList(std::string &paValue, const std::vector<T *> &paTypeEntries) {
      if (!paTypeEntries.empty()) {
        for (T *entry : paTypeEntries) {
          paValue += entry->getTypeName();
          paValue += ", ";
        }
        paValue.erase(paValue.size() - 2);
      }
    }

    EMGMResponse queryAllFBTypes(std::string &paValue) {
      appendTypeNameList(paValue, getFBTypeEntries());
      return EMGMResponse::Ready;
    }

    EMGMResponse queryAllAdapterTypes(std::string &paValue) {
      appendTypeNameList(paValue, getAdapterTypeEntries());
      return EMGMResponse::Ready;
    }

    EMGMResponse queryAllGlobalConstTypes(std::string &paValue) {
      appendTypeNameList(paValue, getGlobalConstEntries());
      return EMGMResponse::Ready;
    }

    void createConnectionResponseMessage(const CConnection &paConn,
                                         const CFunctionBlock &paDstFb,
                                         const StringId paDstId,
                                         std::string &paReqResult) {
      paReqResult += "<Connection Source=\""s;

      TNameIdentifier srcNameList;
      paConn.getSourceId().getFB().getFullQualifiedApplicationInstanceName(srcNameList);
      paConn.getSourcePortName(srcNameList);
      util::join(srcNameList.cbegin(), srcNameList.cend(), {}, '.', paReqResult);

      paReqResult.append("\" Destination=\""s);

      paReqResult += paDstFb.getFullQualifiedApplicationInstanceName('.');
      paReqResult += "."s;
      paReqResult += paDstId;

      paReqResult.append("\"/>\n"s);
    }

    void createEOConnectionResponse(const CFunctionBlock &paFb, std::string &paReqResult) {
      const SFBInterfaceSpec &spec(paFb.getFBInterfaceSpec());
      for (size_t i = 0; i < spec.getNumEOs(); i++) {
        const CEventConnection *eConn = paFb.getEOConnection(spec.mEONames[i]);
        if (eConn) {
          for (const auto &it : eConn->getDestinationList()) {
            createConnectionResponseMessage(*eConn, it.getFB(),
                                            it.getFB().getFBInterfaceSpec().mEINames[it.getPortId()], paReqResult);
          }
        }
      }
    }

    void createDOConnectionResponse(const CFunctionBlock &paFb, std::string &paReqResult) {
      const SFBInterfaceSpec &spec(paFb.getFBInterfaceSpec());
      for (size_t i = 0; i < spec.getNumDIs(); i++) {
        const CDataConnection *const dConn = paFb.getDIConnection(spec.mDINames[i]);
        if (dConn != nullptr) {
          createConnectionResponseMessage(*dConn, paFb, spec.mDINames[i], paReqResult);
        }
      }
    }

    void createAOConnectionResponse(const CFunctionBlock &paFb, std::string &paReqResult) {
      const SFBInterfaceSpec &spec(paFb.getFBInterfaceSpec());
      for (auto it : spec.mSocketNames) {
        const ISocketPin *const skt = paFb.getSocketPin(it);
        if (skt != nullptr && skt->getAdapterCon() != nullptr) {
          createConnectionResponseMessage(*skt->getAdapterCon(), paFb, it, paReqResult);
        }
      }
    }

    void createFBResponseMessage(const CFunctionBlock &paFb, const std::string &fullName, std::string &paValue) {
      paValue += "<FB name=\""s;
      paValue += fullName;
      paValue += "\" type=\""s;
      paValue += paFb.getFBTypeName();
      paValue += "\"/>\n"s;
    }

    EMGMResponse queryFBs(std::string &paValue, const CFBContainer &container, const std::string prefix) {
      for (auto itRunner : container.getChildren()) {
        if (itRunner->isFB()) {
          const CFunctionBlock &fb = static_cast<const CFunctionBlock &>(*itRunner);
          createFBResponseMessage(fb, prefix + fb.getInstanceName(), paValue);
        } else {
          queryFBs(paValue, *itRunner, prefix + itRunner->getInstanceName() + "."s);
        }
      }
      return EMGMResponse::Ready;
    }

    EMGMResponse queryConnections(std::string &paReqResult, const CFBContainer &container) {
      for (auto itRunner : container.getChildren()) {
        if (itRunner->isFB()) {
          const CFunctionBlock &fb = static_cast<const CFunctionBlock &>(*itRunner);
          createEOConnectionResponse(fb, paReqResult);
          createDOConnectionResponse(fb, paReqResult);
          createAOConnectionResponse(fb, paReqResult);
        } else {
          queryConnections(paReqResult, *itRunner);
        }
      }
      return EMGMResponse::Ready;
    }

    template<class T>
    void buildQueryTypeRepsonse(T *paTypeEntry, std::string &paReqResult, std::string_view paResponsePrefix) {
      paReqResult = '<';
      paReqResult += paResponsePrefix;
      paReqResult += " Name=\""s;
      paReqResult += paTypeEntry->getTypeName();
      paReqResult += '#';
      paReqResult += paTypeEntry->getTypeHash();
      paReqResult += "\"/>"s;
    }

    template<class T>
    EMGMResponse createQueryTypeResponseMessage(T *paTypeEntry,
                                                std::string_view paTypeHash,
                                                std::string &paReqResult,
                                                std::string_view paResponsePrefix) {
      if (paTypeEntry == nullptr) {
        paReqResult.clear();
        return EMGMResponse::UnsupportedType;
      }

      if (paTypeHash.empty()) {
        // return type hash
        buildQueryTypeRepsonse(paTypeEntry, paReqResult, paResponsePrefix);
      } else {
        // we can only clear paReqResult after this type hash check
        if (paTypeHash != paTypeEntry->getTypeHash()) {
          // the type hash is different return it to the sender for checks
          buildQueryTypeRepsonse(paTypeEntry, paReqResult, paResponsePrefix);
          return EMGMResponse::UnsupportedType;
        }
        paReqResult.clear();
      }

      return EMGMResponse::Ready;
    }

    EMGMResponse
    createFBTypeResponseMessage(const StringId paTypeNameId, std::string_view paTypeHash, std::string &paReqResult) {
      return createQueryTypeResponseMessage(getFBTypeEntry(paTypeNameId), paTypeHash, paReqResult, "FBType");
    }

    EMGMResponse
    createDataTypeResponseMessage(const StringId paTypeNameId, std::string_view paTypeHash, std::string &paReqResult) {
      return createQueryTypeResponseMessage(getDataTypeEntry(paTypeNameId), paTypeHash, paReqResult, "DataType");
    }

    EMGMResponse createAdapterTypeResponseMessage(const StringId paTypeNameId,
                                                  std::string_view paTypeHash,
                                                  std::string &paReqResult) {
      return createQueryTypeResponseMessage(getAdapterTypeEntry(paTypeNameId), paTypeHash, paReqResult, "AdapterType");
    }

    EMGMResponse createGlobalConstTypeResponseMessage(const StringId paTypeNameId,
                                                      std::string_view paTypeHash,
                                                      std::string &paReqResult) {
      return createQueryTypeResponseMessage(getGlobalConstTypeEntry(paTypeNameId), paTypeHash, paReqResult,
                                            "GlobalConstType");
    }

  } // namespace

#ifdef FORTE_DYNAMIC_TYPE_LOAD

  namespace {

    EMGMResponse createFBTypeFromLua(forte::StringId typeNameId, const std::string &paLuaScriptAsString) {
      EMGMResponse retVal = EMGMResponse::UnsupportedType;

      if (paLuaScriptAsString.find("internalFBs") != std::string::npos) { // CFBType
        if (CLuaCFBTypeEntry::createLuaFBTypeEntry(typeNameId, paLuaScriptAsString) != nullptr) {
          retVal = EMGMResponse::Ready;
        } else {
          retVal = EMGMResponse::InvalidOperation;
        }
      } else { // BFBType
        if (CLuaBFBTypeEntry::createLuaFBTypeEntry(typeNameId, paLuaScriptAsString) != nullptr) {
          retVal = EMGMResponse::Ready;
        } else {
          retVal = EMGMResponse::InvalidOperation;
        }
      }
      return retVal;
    }

    EMGMResponse createAdapterTypeFromLua(forte::StringId typeNameId, const std::string &paLuaScriptAsString) {
      EMGMResponse retVal = EMGMResponse::UnsupportedType;
      if (CLuaAdapterTypeEntry::createLuaAdapterTypeEntry(typeNameId, paLuaScriptAsString) != nullptr) {
        retVal = EMGMResponse::Ready;
      } else {
        retVal = EMGMResponse::InvalidOperation;
      }
      return retVal;
    }

  } // namespace

#endif // FORTE_DYNAMIC_TYPE_LOAD

  CResource::CResource(CFBContainer &paDevice,
                       const SFBInterfaceSpec &paInterfaceSpec,
                       const StringId paInstanceNameId) :
      CFunctionBlock(paDevice, paInterfaceSpec, paInstanceNameId),
      mInternal(std::make_unique<ResourceInternal>(*this)),
      mResourceEventExecution(EcetFactory::create()) {
  }

  CResource::CResource(const SFBInterfaceSpec &paInterfaceSpec, const StringId paInstanceNameId) :
      CFunctionBlock(*this, paInterfaceSpec, paInstanceNameId),
      mInternal(std::make_unique<ResourceInternal>(*this)),
      mResourceEventExecution(nullptr) {
  }

  CResource::~CResource() = default;

  EMGMResponse CResource::executeMGMCommand(SManagementCMD &paCommand) {
    EMGMResponse retVal = EMGMResponse::InvalidDst;

    if (!paCommand.mDestination) {
      switch (paCommand.mCMD) {
        case EMGMCommandType::CreateFBInstance: {
          retVal = createFB(paCommand.mFirstParam, paCommand.mSecondParam.front(), paCommand.mAdditionalParams);
        } break;
        case EMGMCommandType::CreateFBType:
#ifdef FORTE_DYNAMIC_TYPE_LOAD
          retVal = createFBTypeFromLua(paCommand.mFirstParam.front(), paCommand.mAdditionalParams);
#else
          retVal = EMGMResponse::UnsupportedCmd;
#endif
          break;
        case EMGMCommandType::CreateAdapterType:
#ifdef FORTE_DYNAMIC_TYPE_LOAD
          retVal = createAdapterTypeFromLua(paCommand.mFirstParam.front(), paCommand.mAdditionalParams);
#else
          retVal = EMGMResponse::UnsupportedCmd;
#endif
          break;
        case EMGMCommandType::DeleteFBInstance: {
          retVal = deleteFB(paCommand.mFirstParam);
        } break;
        case EMGMCommandType::CreateConnection:
          retVal = createConnection(paCommand.mFirstParam, paCommand.mSecondParam);
          break;
        case EMGMCommandType::DeleteConnection:
          retVal = deleteConnection(paCommand.mFirstParam, paCommand.mSecondParam);
          break;
        case EMGMCommandType::Read: retVal = readValue(paCommand.mFirstParam, paCommand.mAdditionalParams); break;
        case EMGMCommandType::Write: retVal = writeValue(paCommand.mFirstParam, paCommand.mAdditionalParams); break;
        case EMGMCommandType::Start:
        case EMGMCommandType::Stop:
        case EMGMCommandType::Kill:
        case EMGMCommandType::Reset: retVal = handleExecutionStateCmd(paCommand.mCMD, paCommand.mFirstParam); break;
        case EMGMCommandType::QueryFBTypes: retVal = queryAllFBTypes(paCommand.mAdditionalParams); break;
        case EMGMCommandType::QueryAdapterTypes: retVal = queryAllAdapterTypes(paCommand.mAdditionalParams); break;
        case EMGMCommandType::QueryGlobalConstTypes:
          retVal = queryAllGlobalConstTypes(paCommand.mAdditionalParams);
          break;
        case EMGMCommandType::QueryFB: retVal = queryFBs(paCommand.mAdditionalParams, *this, ""); break;
        case EMGMCommandType::QueryFBType:
          retVal = createFBTypeResponseMessage(paCommand.mFirstParam.front(), paCommand.mAdditionalParams,
                                               paCommand.mAdditionalParams);
          break;
        case EMGMCommandType::QueryAdapterType:
          retVal = createAdapterTypeResponseMessage(paCommand.mFirstParam.front(), paCommand.mAdditionalParams,
                                                    paCommand.mAdditionalParams);
          break;
        case EMGMCommandType::QueryDataType:
          retVal = createDataTypeResponseMessage(paCommand.mFirstParam.front(), paCommand.mAdditionalParams,
                                                 paCommand.mAdditionalParams);
          break;
        case EMGMCommandType::QueryGlobalConstType:
          retVal = createGlobalConstTypeResponseMessage(paCommand.mFirstParam.front(), paCommand.mAdditionalParams,
                                                        paCommand.mAdditionalParams);
          break;
        case EMGMCommandType::QueryConnection: retVal = queryConnections(paCommand.mAdditionalParams, *this); break;
        default: retVal = getInternal().getMonitoringHandler().executeMonitoringCommand(paCommand); break;
      }
    }
    return retVal;
  }

  EMGMResponse CResource::changeExecutionState(EMGMCommandType paCommand) {
    EMGMResponse retVal = CFunctionBlock::changeExecutionState(paCommand);

    if (retVal == EMGMResponse::Ready) {
      if (paCommand == EMGMCommandType::Start) { // on start, sample inputs
        for (TPortId i = 0; i < getFBInterfaceSpec().getNumDIs(); ++i) {
          CDataConnection *conn = *getDIConUnchecked(i);
          if (conn != nullptr) {
            conn->readData(*getDI(i));
          }
        }
      } else if (paCommand == EMGMCommandType::Reset) {
        setInitialValues();
      }
      if (mResourceEventExecution != nullptr) {
        // if we have a mResourceEventExecution handle it
        mResourceEventExecution->changeExecutionState(paCommand);
      }
    }
    return retVal;
  }

  EMGMResponse CResource::handleExecutionStateCmd(EMGMCommandType paCMD, TNameIdentifier &paTarget) {
    EMGMResponse retVal = EMGMResponse::NoSuchObject;
    CFunctionBlock *fb = this;

    if (!paTarget.empty()) {
      auto itRunner = paTarget.cbegin();
      fb = getFB(itRunner, paTarget.cend());
    }

    if (nullptr != fb) {
      retVal = fb->changeExecutionState(paCMD);
    }
    return retVal;
  }

  EMGMResponse CResource::createConnection(SManagementCMD &paCommand) {
    return createConnection(paCommand.mFirstParam, paCommand.mSecondParam);
  }

  EMGMResponse CResource::createConnection(TNameIdentifier &paSrcNameList, TNameIdentifier &paDstNameList) {
    if (paSrcNameList.empty() || paDstNameList.empty()) {
      return EMGMResponse::NoSuchObject;
    }

    CConnection::Wrapper con = getOutputConnection(paSrcNameList);
    if (!con) {
      return EMGMResponse::NoSuchObject;
    }

    auto runner = paDstNameList.cbegin();
    CFunctionBlock *dstFB = getFB(runner, paDstNameList.cend() - 1);
    if (!dstFB) {
      return EMGMResponse::NoSuchObject;
    }
    EMGMResponse retVal = con->connect(*dstFB, {runner, paDstNameList.cend()});
    if (retVal == EMGMResponse::Ready) {
      con.release();
    }
    return retVal;
  }

  EMGMResponse CResource::deleteConnection(TNameIdentifier &paSrcNameList, TNameIdentifier &paDstNameList) {

    if (paSrcNameList.empty() || paDstNameList.empty()) {
      return EMGMResponse::NoSuchObject;
    }

    auto dstIt = paDstNameList.cbegin();
    CFunctionBlock *dstFB = getFB(dstIt, paDstNameList.cend() - 1);
    if (!dstFB) {
      return EMGMResponse::NoSuchObject;
    }

    // first check if the destination has an input connection with the given name
    if (CConnection *dstCon = getInputConnection(paDstNameList)) {
      TNameIdentifier dstConSourceNameList;
      dstCon->getSourceId().getFB().getFullQualifiedApplicationInstanceName(dstConSourceNameList);
      dstCon->getSourcePortName(dstConSourceNameList);
      if (dstConSourceNameList != paSrcNameList) {
        return EMGMResponse::NoSuchObject;
      }
      const EMGMResponse retVal = dstCon->disconnect(*dstFB, {dstIt, paDstNameList.cend()});
      if (retVal == EMGMResponse::Ready && dstCon->isDelegating()) {
        delete dstCon;
      }
      return retVal;
    }

    // otherwise check if the source has an output connection with the given name
    if (const CConnection::Wrapper srcCon = getOutputConnection(paSrcNameList)) {
      return srcCon->disconnect(*dstFB, {dstIt, paDstNameList.cend()});
    }

    return EMGMResponse::NoSuchObject;
  }

  EMGMResponse CResource::writeValue(TNameIdentifier &paNameList, const std::string &paValue, bool paForce) {
    CIEC_ANY *const var = getVar(paNameList);
    if (var == nullptr) {
      return EMGMResponse::NoSuchObject;
    }

    // 0 is not supported in the fromString method
    if (paValue.empty() || var->fromString(paValue.c_str()) != static_cast<int>(paValue.length())) {
      // if we cannot parse the full value the value is not valid
      return EMGMResponse::BadParams;
    }

    if (paForce) {
      if (!setForced(paNameList, true)) {
        return EMGMResponse::NoSuchObject;
      }
    } else if (const auto conn = getOutputConnection(paNameList);
               conn && conn->isInOut() && !getInputConnection(paNameList)) {
      static_cast<CInOutDataConnection *>(conn.get())->getValue().setValue(var->unwrap());
    }
    if (getState() == E_FBStates::Idle) {
      mInitialValues.emplace(paNameList, paValue);
    }
    return EMGMResponse::Ready;
  }

  void CResource::setInitialValues() {
    for (const auto &[name, value] : mInitialValues) {
      if (CIEC_ANY *var = getVar(name)) {
        var->fromString(value.c_str());
      }
    }
  }

  EMGMResponse CResource::readValue(TNameIdentifier &paNameList, std::string &paValue) {
    CIEC_ANY *const var = getVar(paNameList);
    if (var == nullptr) {
      return EMGMResponse::NoSuchObject;
    }

    auto start = paValue.size();
    switch (var->getDataTypeID()) {
      case CIEC_ANY::e_STRING:
      case CIEC_ANY::e_WSTRING:
        static_cast<CIEC_ANY_STRING &>(*var).toUTF8(paValue, false);
        if (start != paValue.size()) {
          util::transformNonEscapedToEscapedXMLText(paValue, start);
        }
        break;
      case CIEC_ANY::e_CHAR:
      case CIEC_ANY::e_WCHAR:
      case CIEC_ANY::e_ARRAY:
      case CIEC_ANY::e_STRUCT:
        var->toString(paValue);
        if (start != paValue.size()) {
          util::transformNonEscapedToEscapedXMLText(paValue, start);
        }
        break;
      default: var->toString(paValue); break;
    }
    return EMGMResponse::Ready;
  }

  CConnection::Wrapper CResource::getOutputConnection(const std::span<const StringId> paSrcNameList) {
    if (paSrcNameList.empty()) {
      return CConnection::Wrapper();
    }
    StringId name = paSrcNameList.front();
    if (const auto conn = getResIf2InConnection(name); conn) {
      return conn->getDelegatingConnection(paSrcNameList.subspan(1));
    }
    return CFunctionBlock::getOutputConnection(paSrcNameList);
  }

  CConnection *CResource::getResIf2InConnection(StringId paResInput) {
    TPortId inPortId = getFBInterfaceSpec().getDIID(paResInput);
    return (inPortId != cgInvalidPortId) ? getResIf2InConnectionUnchecked(inPortId) : nullptr;
  }
} // namespace forte
