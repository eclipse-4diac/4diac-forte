/*******************************************************************************
 * Copyright (c) 2015 Florian Froschermeier <florian.froschermeier@tum.de>,
 *                    fortiss GmbH, Primetals Technologies Austria GmbH,
 *                    HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol
 *    Stefan Profanter
 *      - refactoring and adaption to new concept
 *    Jose Cabral:
 *      - refactoring to cleaner architecture
 *    Martin Melik Merkumians:
 *      - Change CIEC_STRING to std::string
 *    Markus Meingast:
 *      - Add support for Object Structs
 *    Franz Höpfinger
 *      - Add optional OPC UA over WebSocket (insecure and secure transports) server endpoint
 *******************************************************************************/

#include "opcua_local_handler.h"

#include <open62541/server.h>
#include <open62541/server_config_default.h>
#ifdef FORTE_COM_OPC_UA_WEBSOCKET
#include <open62541/plugin/eventloop.h>
#ifdef FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER
#include "ws_connection_manager.h"
#endif // FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER
#endif // FORTE_COM_OPC_UA_WEBSOCKET

#include "forte/iec61131_functions.h"
#include "forte/cominfra/basecommfb.h"
#include "forte/util/parameterParser.h"
#include "forte/util/string_utils.h"
#include "forte/util/mainparam_utils.h"
#include "forte/arch/forte_printer.h"

#include "forte/com/opc_ua/opcua_nodesets.h"

#include "struct_action_info.h"
#ifdef FORTE_COM_OPC_UA_MULTICAST
#include "detail/lds_me_handler.h"
#endif // FORTE_COM_OPC_UA_MULTICAST
#include <string>
#include <cstdint>
#include <limits>

#ifndef FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
#include "forte/arch/sockhand.h"
#endif

using namespace std::string_literals;

namespace forte::com_infra::opc_ua {
  namespace {
    class OpcuaServerPortOption final
        : public util::CommandLineParser::IntOptionImpl<TForteUInt16,
                                                        "op",
                                                        "opc_ua-listen-port",
                                                        "<port>",
                                                        "Set the listening port for the OPC UA connection"> {
      public:
        bool setOption(const TForteUInt16 paArgument) override {
          mArgument = paArgument;
          return true;
        }

        TForteUInt16 mArgument = FORTE_COM_OPC_UA_PORT;
    };

    OpcuaServerPortOption gOpcuaServerPort;

    TForteUInt16 gOpcuaServerMaxIterationInterval = FORTE_COM_OPC_UA_SERVER_MAX_ITERATION_INTERVAL;
  } // namespace

  const char *const COPC_UA_Local_Handler::mEnglishLocaleForNodes = "en-US";
  const char *const COPC_UA_Local_Handler::mDefaultDescriptionForVariableNodes = "Digital port of Function Block";

  COPC_UA_Local_Handler::COPC_UA_Local_Handler(CDeviceExecution &paDeviceExecution) :
      COPC_UA_HandlerAbstract(paDeviceExecution) {
  }

  COPC_UA_Local_Handler::~COPC_UA_Local_Handler() {
    stopServer();

    for (auto iter = mNodesReferences.begin(); iter != mNodesReferences.end(); ++iter) {
      UA_NodeId_delete(const_cast<UA_NodeId *>(iter->mNodeId));
    }
    mNodesReferences.clear();
  }

  void COPC_UA_Local_Handler::enableHandler() {
    startServer();
  }

  void COPC_UA_Local_Handler::disableHandler() {
    stopServer();
  }

  namespace {
    constexpr std::uint32_t scmWsPortOffset = 1U;
    constexpr std::uint32_t scmWssPortOffset = 2U;

    constexpr std::uint32_t getInsecureWsPort(std::uint32_t paTcpPort) {
      return paTcpPort + scmWsPortOffset;
    }

    constexpr std::uint32_t getSecureWsPort(std::uint32_t paTcpPort) {
      return paTcpPort + scmWssPortOffset;
    }
  } // namespace

  void COPC_UA_Local_Handler::run() {
    UA_ServerConfig config{
        .logging = &getLogger(),
    };

    // set default config
    if (const UA_StatusCode status = UA_ServerConfig_setDefault(&config); status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Error setting server configuration. Error: %s\n", UA_StatusCode_name(status));
      mServerStarted.inc(); // this will avoid locking startServer() for all cases where the starting of server failed
      return;
    }

    UA_ServerStrings serverStrings;
    generateServerStrings(gOpcuaServerPort.mArgument, serverStrings);
    configureUAServer(serverStrings, config);

    // Built from the endpoints configureUAServer() actually registered (e.g. it drops the
    // unencrypted WebSocket endpoint if the WebSocket ConnectionManager failed to register,
    // and the secure one if no certificate/private key were configured), not from the raw
    // port and compile-time flags. Only logged below once the server has actually started
    // listening.
    std::string startupLogMsg = "[OPC UA LOCAL]: Starting OPC UA Server";
    for (size_t i = 0; i < config.serverUrlsSize; ++i) {
      startupLogMsg += (i == 0) ? ": " : ", ";
      startupLogMsg +=
          std::string(reinterpret_cast<const char *>(config.serverUrls[i].data), config.serverUrls[i].length);
    }
    startupLogMsg += "\n";

    mUaServer = UA_Server_newWithConfig(&config);
    if (mUaServer) {
      if (initializeNodesets(*mUaServer)) {
        UA_StatusCode retVal = UA_Server_run_startup(mUaServer);
        if (UA_STATUSCODE_GOOD == retVal) {
          DEVLOG_INFO("%s", startupLogMsg.c_str());
          {
#ifdef FORTE_COM_OPC_UA_MULTICAST
            // the extra curly brace on top is to limit the lifetime of this object.
            // it was avoided to have inside this ifdef to also avoid the closing brace
            // with the exta ifdef making it cleaner this way
            auto mLdsMeHandler = forte::com_infra::opc_ua::detail::LdsMeHandler(*mUaServer);
#endif // FORTE_COM_OPC_UA_MULTICAST
            mServerStarted.inc();
            while (isAlive()) {
              UA_UInt16 timeToSleepMs;
              {
                util::CCriticalRegion criticalRegion(mServerAccessMutex);
#if defined(FORTE_COM_OPC_UA_WEBSOCKET) && defined(FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER) &&                    \
    !defined(UA_ENABLE_LWS)
                // Must run on the same thread as UA_Server_run_iterate(), see
                // ws_connection_manager.h: open62541 has no locking on this
                // architecture, so all UA_Server access has to stay on this thread.
                //
                // The !defined(UA_ENABLE_LWS) guard matters here even though
                // FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER is meant for platforms
                // without it: if both end up defined for the same build, the #ifdef/#elif
                // in configureUAServer() below still gives UA_ENABLE_LWS priority, so
                // mWsConnectionManager is an LWS-backed UA_ConnectionManager, not a
                // WsConnectionManager -- calling WsConnectionManager_poll() on it would be
                // undefined behavior.
                if (mWsConnectionManager) {
                  WsConnectionManager_poll(mWsConnectionManager);
                }
#endif // FORTE_COM_OPC_UA_WEBSOCKET && FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER && !UA_ENABLE_LWS
                timeToSleepMs = UA_Server_run_iterate(mUaServer, false);
              }

              if (timeToSleepMs < scmMinimumIterationWaitTime) {
                timeToSleepMs = scmMinimumIterationWaitTime;
              } else if (timeToSleepMs > gOpcuaServerMaxIterationInterval) {
                timeToSleepMs = gOpcuaServerMaxIterationInterval;
              }

              mServerNeedsIteration.timedWait(static_cast<TForteUInt64>(timeToSleepMs) * 1000000ULL);
            }
          }
          retVal = UA_Server_run_shutdown(mUaServer);
          if (UA_STATUSCODE_GOOD == retVal) {
            DEVLOG_INFO("[OPC UA LOCAL]: Server successfully stopped\n");
          } else {
            DEVLOG_ERROR("[OPC UA LOCAL]: Error stopping up the server. Error: %s\n", UA_StatusCode_name(retVal));
          }
        } else {
          DEVLOG_ERROR("[OPC UA LOCAL]: Error starting up the server. Error: %s\n", UA_StatusCode_name(retVal));
        }
      } else {
        DEVLOG_ERROR("[OPC UA LOCAL]: Couldn't initialize Nodesets\n", gOpcuaServerPort.mArgument);
      }
      UA_Server_delete(mUaServer);
      mUaServer = nullptr;
#ifdef FORTE_COM_OPC_UA_WEBSOCKET
      // mWsConnectionManager was torn down as part of the EventLoop that
      // UA_Server_delete() just freed; drop the now-dangling pointer.
      mWsConnectionManager = nullptr;
#endif // FORTE_COM_OPC_UA_WEBSOCKET
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: Couldn't initialize server\n");
      UA_ServerConfig_clear(&config);
#ifdef FORTE_COM_OPC_UA_WEBSOCKET
      // Same as above: torn down together with the eventLoop that
      // UA_ServerConfig_clear() just cleared.
      mWsConnectionManager = nullptr;
#endif // FORTE_COM_OPC_UA_WEBSOCKET
    }
    mServerStarted.inc(); // this will avoid locking startServer() for all cases where the starting of server failed
  }

  void COPC_UA_Local_Handler::startServer() {
    if (!isAlive()) {
      start();
      mServerStarted.waitIndefinitely();
      mServerStarted.inc(); // in case two threads get into this block at the same time
    }
  }

  void COPC_UA_Local_Handler::stopServer() {
    end();
  }

  void COPC_UA_Local_Handler::generateServerStrings(TForteUInt16 paUAServerPort,
                                                    UA_ServerStrings &paServerStrings) const {
    char helperBuffer[scmMaxServerNameLength + 1];
    forte_snprintf(helperBuffer, scmMaxServerNameLength, "forte_%d", paUAServerPort);

#ifdef FORTE_COM_OPC_UA_MULTICAST
    paServerStrings.mMdnsServerName = std::string(helperBuffer);
#endif // FORTE_COM_OPC_UA_MULTICAST

    paServerStrings.mAppURI = "org.eclipse.4diac."s;
    std::string serverName;
#ifdef FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
    serverName = std::string(FORTE_COM_OPC_UA_CUSTOM_HOSTNAME) + "-" + helperBuffer;
    paServerStrings.mAppURI.append("opc.tcp://" + serverName);
#else
    paServerStrings.mAppURI.append(helperBuffer);
#endif
    // serverName stays empty here when FORTE_COM_OPC_UA_CUSTOM_HOSTNAME is not defined --
    // that's intentional, not a lost host identifier: it matches this function's
    // pre-WebSocket behavior (mHostname was always "opc.tcp://" + ":" + port in that case,
    // helperBuffer only ever went into mAppURI), and an empty host makes open62541 listen
    // on all interfaces, which is the desired default. See the WebSocket hostname comment
    // below for the same rationale.
    paServerStrings.mHostname = "opc.tcp://" + serverName;
    paServerStrings.mHostname.append(":");
    paServerStrings.mHostname.append(std::to_string(paUAServerPort));

#ifdef FORTE_COM_OPC_UA_WEBSOCKET
    const auto makeWebSocketHostname = [&](const char *paScheme, const std::uint32_t paPort) {
      std::string hostname(paScheme);
      hostname.append(serverName);
      hostname.append(":");
      hostname.append(std::to_string(paPort));
      return hostname;
    };
#if defined(FORTE_COM_OPC_UA_WEBSOCKET_SECURE)
    constexpr std::uint32_t maxAllowedPort = std::numeric_limits<TForteUInt16>::max() - scmWssPortOffset;
#else
    constexpr std::uint32_t maxAllowedPort = std::numeric_limits<TForteUInt16>::max() - scmWsPortOffset;
#endif
    if (paUAServerPort <= maxAllowedPort) {
#ifdef FORTE_COM_OPC_UA_WEBSOCKET_INSECURE
      // Mirrors mHostname above: an empty hostname makes open62541 listen on
      // all interfaces, which is the default here (no FORTE_COM_OPC_UA_CUSTOM_HOSTNAME).
      // Intentional unencrypted-WebSocket support (builder-configurable, off when
      // INSECURE=OFF): a dev/test transport, never enabled by default.
      paServerStrings.mWsHostname = makeWebSocketHostname(
          "opc.ws://",
          getInsecureWsPort(paUAServerPort)); // nosemgrep: javascript.lang.security.detect-insecure-websocket
#endif // FORTE_COM_OPC_UA_WEBSOCKET_INSECURE

#ifdef FORTE_COM_OPC_UA_WEBSOCKET_SECURE
      paServerStrings.mWssHostname = makeWebSocketHostname("opc.wss://", getSecureWsPort(paUAServerPort));
#endif // FORTE_COM_OPC_UA_WEBSOCKET_SECURE
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: OPC UA server port %u is too high to derive WebSocket ports (max %u)\n",
                   paUAServerPort, maxAllowedPort);
    }
#endif // FORTE_COM_OPC_UA_WEBSOCKET
  }

#if defined(FORTE_COM_OPC_UA_WEBSOCKET) &&                                                                             \
    (defined(UA_ENABLE_LWS) ||                                                                                         \
     (defined(FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER) && defined(FORTE_COM_OPC_UA_WEBSOCKET_INSECURE)))
  namespace {
    // Registers paConnectionManager (already created by the caller) with the EventLoop.
    // On any failure, logs, frees it, and nulls it out; on success, marks webSocketEnabled.
    // Shared by both the UA_ENABLE_LWS and platform-ConnectionManager paths below, which
    // otherwise differ only in how the ConnectionManager itself gets created. Guarded to
    // match exactly the condition under which it's actually called (both call sites live
    // in configureUAServer(), guarded the same way) so it isn't defined-but-unused when
    // neither path applies.
    // Frees paConnectionManager's event source, if the ConnectionManager provides a free
    // callback at all (defensive -- not all implementations necessarily set one).
    void freeWsEventSource(UA_ConnectionManager *paConnectionManager) {
      if (paConnectionManager->eventSource.free) {
        paConnectionManager->eventSource.free(&paConnectionManager->eventSource);
      }
    }

    void registerWsConnectionManager(UA_ConnectionManager *&paConnectionManager, UA_ServerConfig &paUaServerConfig) {
      if (!paConnectionManager) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not create the WebSocket ConnectionManager\n");
        return;
      }
      if (paUaServerConfig.eventLoop == nullptr) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not register WebSocket ConnectionManager: eventLoop is null\n");
        // registerEventSource() was never called here, so the event source was never
        // linked into the EventLoop -- safe to free directly.
        freeWsEventSource(paConnectionManager);
        paConnectionManager = nullptr;
        return;
      }
      const UA_StatusCode wsRetVal = paUaServerConfig.eventLoop->registerEventSource(paUaServerConfig.eventLoop,
                                                                                     &paConnectionManager->eventSource);
      if (wsRetVal != UA_STATUSCODE_GOOD) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not register the WebSocket ConnectionManager: %s\n",
                     UA_StatusCode_name(wsRetVal));
        // registerEventSource() links the event source into the EventLoop's internal list
        // *before* attempting to start it, so on failure (a failed start()) it is still
        // linked there even though the overall call reports failure -- confirmed against
        // both the UA_ENABLE_LWS and platform ConnectionManager start() implementations,
        // neither of which touch eventSource.state on a failure path, leaving it at the
        // STOPPED state registerEventSource() set just before calling start(). Deregister
        // it first so the EventLoop doesn't keep a dangling pointer to the memory freed
        // below.
        const UA_StatusCode deregisterStatus = paUaServerConfig.eventLoop->deregisterEventSource(
            paUaServerConfig.eventLoop, &paConnectionManager->eventSource);
        if (deregisterStatus != UA_STATUSCODE_GOOD) {
          // Could not safely unlink it from the EventLoop (e.g. still mid-start) -- freeing
          // it now would leave the EventLoop holding a dangling pointer, so leak it instead
          // of risking a use-after-free.
          DEVLOG_ERROR("[OPC UA LOCAL]: Could not deregister the failed WebSocket ConnectionManager: %s -- "
                       "leaking it to avoid a dangling EventLoop reference\n",
                       UA_StatusCode_name(deregisterStatus));
          paConnectionManager = nullptr;
          return;
        }
        freeWsEventSource(paConnectionManager);
        paConnectionManager = nullptr;
        return;
      }
      paUaServerConfig.webSocketEnabled = true;
    }
  } // namespace
#endif // FORTE_COM_OPC_UA_WEBSOCKET && (UA_ENABLE_LWS || (FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER &&
       // FORTE_COM_OPC_UA_WEBSOCKET_INSECURE))

  void COPC_UA_Local_Handler::configureUAServer(UA_ServerStrings &paServerStrings,
                                                UA_ServerConfig &paUaServerConfig) const {
#ifdef FORTE_COM_OPC_UA_MULTICAST
    forte::com_infra::opc_ua::detail::LdsMeHandler::configureServer(paUaServerConfig, paServerStrings.mMdnsServerName);
#endif // FORTE_COM_OPC_UA_MULTICAST

    UA_Array_delete(paUaServerConfig.serverUrls, paUaServerConfig.serverUrlsSize, &UA_TYPES[UA_TYPES_STRING]);
    paUaServerConfig.serverUrls = nullptr;
    paUaServerConfig.serverUrlsSize = 0;

#ifdef FORTE_COM_OPC_UA_WEBSOCKET
    // WebSocket support (both insecure and secure transports): UA_ENABLE_WEBSOCKET_TRANSPORT
    // (the generic server-side handling for both) looks up a UA_ConnectionManager named
    // "websocket" in the EventLoop. On platforms built with UA_ENABLE_LWS (libwebsockets,
    // POSIX-only), open62541 provides that ConnectionManager via
    // UA_ConnectionManager_new_LWS_WebSocket. Platforms without libwebsockets (RTOS targets
    // such as FreeRTOS/Zephyr, e.g. ESP-IDF builds) never define UA_ENABLE_LWS; there
    // webSocketEnabled is set directly and the unencrypted transport has to come from a
    // ConnectionManager registered by that platform's own code.
    bool webSocketTransportAvailable = false;
    paUaServerConfig.webSocketEnabled = false;
#ifdef UA_ENABLE_LWS
    mWsConnectionManager = UA_ConnectionManager_new_LWS_WebSocket(UA_STRING_STATIC("websocket"));
    registerWsConnectionManager(mWsConnectionManager, paUaServerConfig);
    if (mWsConnectionManager) {
      webSocketTransportAvailable = true;
#ifdef FORTE_COM_OPC_UA_WEBSOCKET_INSECURE
      paUaServerConfig.webSocketAllowUnencrypted = true;
#endif
    }
#elif defined(FORTE_COM_OPC_UA_WEBSOCKET_CONNECTIONMANAGER) && defined(FORTE_COM_OPC_UA_WEBSOCKET_INSECURE)
    // No libwebsockets on this platform (e.g. ESP-IDF/FreeRTOS+lwIP): use the
    // platform-provided "websocket" ConnectionManager instead (unencrypted transport
    // only, WsCM_openConnection() rejects useSSL, so the secure one stays disabled).
    mWsConnectionManager = WsConnectionManager_new();
    registerWsConnectionManager(mWsConnectionManager, paUaServerConfig);
    if (mWsConnectionManager) {
      webSocketTransportAvailable = true;
      paUaServerConfig.webSocketAllowUnencrypted = true;
    }
#else
    // No ConnectionManager is created here when libwebsockets is unavailable.
#endif // UA_ENABLE_LWS
#endif // FORTE_COM_OPC_UA_WEBSOCKET

    std::vector<UA_String> serverUrls;
    serverUrls.push_back(UA_STRING(&paServerStrings.mHostname[0]));
#ifdef FORTE_COM_OPC_UA_WEBSOCKET_INSECURE
    if (webSocketTransportAvailable && !paServerStrings.mWsHostname.empty()) {
      serverUrls.push_back(UA_STRING(&paServerStrings.mWsHostname[0]));
    }
#endif // FORTE_COM_OPC_UA_WEBSOCKET_INSECURE
#ifdef FORTE_COM_OPC_UA_WEBSOCKET_SECURE
    const bool hasWebSocketCredentials =
        paUaServerConfig.webSocketCertificate.length > 0 && paUaServerConfig.webSocketCertificate.data != nullptr &&
        paUaServerConfig.webSocketPrivateKey.length > 0 && paUaServerConfig.webSocketPrivateKey.data != nullptr;
    if (!paServerStrings.mWssHostname.empty()) {
      if (webSocketTransportAvailable && hasWebSocketCredentials) {
        serverUrls.push_back(UA_STRING(&paServerStrings.mWssHostname[0]));
      } else if (!hasWebSocketCredentials) {
        // Forte itself never populates webSocketCertificate/webSocketPrivateKey -- there is
        // no built-in configuration path for them yet, so with a stock (unpatched)
        // UA_ServerConfig_setDefault() this branch is always taken and opc.wss:// can never
        // actually come up. Logged as an error, not a warning: FORTE_COM_OPC_UA_WEBSOCKET_SECURE
        // defaulting to ON creates an explicit expectation of an encrypted endpoint that this
        // build cannot meet, which the operator needs to notice.
        DEVLOG_ERROR("[OPC UA LOCAL]: opc.wss:// endpoint requires webSocketCertificate and webSocketPrivateKey "
                     "credentials, which forte does not currently provide a configuration path for; disabling "
                     "opc.wss://\n");
      } else {
        DEVLOG_WARNING("[OPC UA LOCAL]: opc.wss:// endpoint configured with valid webSocketCertificate and "
                       "webSocketPrivateKey, but WebSocket transport is unavailable; disabling opc.wss://\n");
      }
    }
#endif // FORTE_COM_OPC_UA_WEBSOCKET_SECURE

    UA_StatusCode retVal = UA_Array_copy(serverUrls.data(), serverUrls.size(), (void **) &paUaServerConfig.serverUrls,
                                         &UA_TYPES[UA_TYPES_STRING]);
    if (retVal != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Could not set the server URLs. Error: %s\n", UA_StatusCode_name(retVal));
      return;
    }
    paUaServerConfig.serverUrlsSize = serverUrls.size();

    // delete pre-initialized values
    UA_LocalizedText_clear(&paUaServerConfig.applicationDescription.applicationName);
    UA_String_clear(&paUaServerConfig.applicationDescription.applicationUri);

    paUaServerConfig.applicationDescription.applicationUri = UA_String_fromChars(paServerStrings.mAppURI.c_str());
    paUaServerConfig.applicationDescription.applicationName.locale = UA_STRING_NULL;
    paUaServerConfig.applicationDescription.applicationName.text =
        UA_String_fromChars(paServerStrings.mHostname.c_str());
    paUaServerConfig.publishingIntervalLimits.min = FORTE_COM_OPC_UA_SERVER_PUB_INTERVAL;

    for (size_t i = 0; i < paUaServerConfig.endpointsSize; i++) {
      UA_String_clear(&paUaServerConfig.endpoints[i].server.applicationUri);
      UA_LocalizedText_clear(&paUaServerConfig.endpoints[i].server.applicationName);
      UA_String_copy(&paUaServerConfig.applicationDescription.applicationUri,
                     &paUaServerConfig.endpoints[i].server.applicationUri);
      UA_LocalizedText_copy(&paUaServerConfig.applicationDescription.applicationName,
                            &paUaServerConfig.endpoints[i].server.applicationName);
    }
  }

  void COPC_UA_Local_Handler::referencedNodesIncrement(const std::vector<UA_NodeId *> &paNodes,
                                                       CActionInfo &paActionInfo) {
    for (auto node : paNodes) {
      bool found = false;
      for (auto &reference : mNodesReferences) {
        if (UA_NodeId_equal(reference.mNodeId, node)) {
          found = true;
          reference.mActionsReferencingIt.push_back(&paActionInfo);
          break;
        }
      }
      if (!found) {
        UA_NodeId *newNode = UA_NodeId_new();
        UA_NodeId_copy(node, newNode);
        mNodesReferences.emplace_back(newNode, std::vector<CActionInfo *>{&paActionInfo});
      }
    }
  }

  void COPC_UA_Local_Handler::referencedNodesDecrement(const CActionInfo &paActionInfo) {
    std::vector<const UA_NodeId *> nodesReferencedByAction;
    getNodesReferencedByAction(paActionInfo, nodesReferencedByAction);

    for (auto iterNode = nodesReferencedByAction.begin(); iterNode != nodesReferencedByAction.end(); ++iterNode) {
      auto nodeReferencedToDelete = mNodesReferences.end();
      for (auto iterRef = mNodesReferences.begin(); iterRef != mNodesReferences.end(); ++iterRef) {
        if (UA_NodeId_equal(iterRef->mNodeId, (*iterNode))) {

          bool stillSomethingThere = true;
          while (stillSomethingThere) {
            auto itActionToDelete = iterRef->mActionsReferencingIt.end();
            for (auto itAction = iterRef->mActionsReferencingIt.begin();
                 itAction != iterRef->mActionsReferencingIt.end(); ++itAction) {
              if ((*itAction) == &paActionInfo) {
                itActionToDelete = itAction;
                break;
              }
            }

            if (iterRef->mActionsReferencingIt.end() == itActionToDelete) {
              stillSomethingThere = false;
            } else {
              iterRef->mActionsReferencingIt.erase(itActionToDelete);
            }
          }

          if (iterRef->mActionsReferencingIt.empty()) {
            nodeReferencedToDelete = iterRef;
            if (0 != iterRef->mNodeId->namespaceIndex && mUaServer) {
              UA_Server_deleteNode(mUaServer, *iterRef->mNodeId, UA_TRUE);
            }
          }
          break;
        }
      }
      if (mNodesReferences.end() != nodeReferencedToDelete) {
        UA_NodeId_delete(const_cast<UA_NodeId *>(nodeReferencedToDelete->mNodeId));
        mNodesReferences.erase(nodeReferencedToDelete);
      }
    }
  }

  void COPC_UA_Local_Handler::getNodesReferencedByAction(const CActionInfo &paActionInfo,
                                                         std::vector<const UA_NodeId *> &paNodes) const {
    for (auto iterRef = mNodesReferences.begin(); iterRef != mNodesReferences.end(); ++iterRef) {
      for (auto iterAction = iterRef->mActionsReferencingIt.begin(); iterAction != iterRef->mActionsReferencingIt.end();
           ++iterAction) {
        if ((*iterAction) == &paActionInfo) {
          paNodes.push_back(iterRef->mNodeId);
          break;
        }
      }
    }
  }

  UA_StatusCode COPC_UA_Local_Handler::initializeAction(CActionInfo &paActionInfo) {
    enableHandler();
    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
    if (mUaServer) { // if the server failed at starting, nothing will be initialized
      // other thread may currently create nodes, thus mutex
      util::CCriticalRegion criticalRegion(mServerAccessMutex);
      switch (paActionInfo.getAction()) {
        case CActionInfo::eRead: retVal = initializeVariable(paActionInfo, false); break;
        case CActionInfo::eWrite: retVal = initializeVariable(paActionInfo, true); break;
        case CActionInfo::eCreateMethod: retVal = initializeCreateMethod(paActionInfo); break;
        case CActionInfo::eCreateObject:
        case CActionInfo::eCreateVariable: retVal = initializeCreateNode(paActionInfo); break;
        case CActionInfo::eDeleteObject:
        case CActionInfo::eDeleteVariable: retVal = initializeDeleteNode(paActionInfo); break;
        case CActionInfo::eCallMethod:
        case CActionInfo::eSubscribe:
          DEVLOG_ERROR("[OPC UA LOCAL]: Cannot perform action %s locally. Initialization failed\n",
                       CActionInfo::mActionNames[paActionInfo.getAction()]);
          break;
        default: DEVLOG_ERROR("[OPC UA LOCAL]: Unknown action %d to be initialized\n", paActionInfo.getAction()); break;
      }
      mServerNeedsIteration.inc();
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::initializeActionForObjectStruct(std::shared_ptr<CActionInfo> &paActionInfo,
                                                                       CIEC_ANY &paMember) {
    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
    if (mUaServer) {
      util::CCriticalRegion criticalRegion(mServerAccessMutex);
      if (paActionInfo->getAction() == CActionInfo::eWrite) {
        retVal = initializeObjectStructMemberVariable(paActionInfo, &paMember, true);
      } else if (paActionInfo->getAction() == CActionInfo::eRead) {
        retVal = initializeObjectStructMemberVariable(paActionInfo, &paMember, false);
      }
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::executeAction(CActionInfo &paActionInfo) {
    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

    util::CCriticalRegion criticalRegion(mServerAccessMutex);
    switch (paActionInfo.getAction()) {
      case CActionInfo::eWrite: retVal = executeWrite(paActionInfo); break;
      case CActionInfo::eCreateMethod: retVal = executeCreateMethod(paActionInfo); break;
      case CActionInfo::eCreateObject: retVal = executeCreateObject(paActionInfo); break;
      case CActionInfo::eCreateVariable: retVal = executeCreateVariable(paActionInfo); break;
      case CActionInfo::eDeleteObject:
      case CActionInfo::eDeleteVariable: retVal = executeDeleteObject(paActionInfo); break;
      default: // eCallMethod, eSubscribe will never reach here since they weren't initialized. eRead is a Subscribe FB
        DEVLOG_ERROR("[OPC UA LOCAL]: Action %d to be executed is unknown or invalid\n", paActionInfo.getAction());
        break;
    }

    mServerNeedsIteration.inc();

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::executeStructAction(CActionInfo &paActionInfo, CIEC_ANY &paMember) {
    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

    util::CCriticalRegion criticalRegion(mServerAccessMutex);
    switch (paActionInfo.getAction()) {
      case CActionInfo::eWrite: retVal = executeStructWrite(paActionInfo, paMember); break;
      default: // eCallMethod, eSubscribe will never reach here since they weren't initialized. eRead is a Subscribe FB
        DEVLOG_ERROR("[OPC UA LOCAL]: Struct Action %d to be executed is unknown or invalid\n",
                     paActionInfo.getAction());
        break;
    }

    mServerNeedsIteration.inc();

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::uninitializeAction(CActionInfo &paActionInfo) {
    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
    util::CCriticalRegion criticalRegion(mServerAccessMutex);
    switch (paActionInfo.getAction()) {
      case CActionInfo::eRead:
      case CActionInfo::eWrite:
      case CActionInfo::eCreateMethod:
      case CActionInfo::eCreateObject:
      case CActionInfo::eCreateVariable:
      case CActionInfo::eDeleteObject:
      case CActionInfo::eDeleteVariable:
        referencedNodesDecrement(paActionInfo);
        retVal = UA_STATUSCODE_GOOD;
        break;
      default:
        DEVLOG_ERROR("[OPC UA LOCAL]: Action %d to be uninitialized is unknown or invalid\n", paActionInfo.getAction());
        break;
    }

    mServerNeedsIteration.inc();

    return retVal;
  }

  bool COPC_UA_Local_Handler::isOPCUAObjectPresent(CActionInfo::CNodePairInfo &paNodePair) {
    enableHandler();
    return isNodePresent(paNodePair);
  }

  UA_StatusCode COPC_UA_Local_Handler::initializeVariable(CActionInfo &paActionInfo, bool paWrite) {
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;

    size_t indexOfNodePair = 0;
    std::vector<UA_NodeId *> referencedNodes;
    const CIEC_ANY *const *variables = paWrite ? paActionInfo.getDataToSend() : paActionInfo.getDataToReceive();
    for (auto itMain = paActionInfo.getNodePairInfo().begin();
         itMain != paActionInfo.getNodePairInfo().end() && UA_STATUSCODE_GOOD == retVal; ++itMain, indexOfNodePair++) {

      std::vector<UA_NodeId *> presentNodes;
      bool nodeExists = false;
      retVal = getNode(*itMain, presentNodes, &nodeExists);

      if (UA_STATUSCODE_GOOD == retVal) {
        if (nodeExists) {
          retVal = handleExistingVariable(paActionInfo, *itMain, *variables[indexOfNodePair], indexOfNodePair, paWrite);

          handlePresentNodes(presentNodes, referencedNodes, UA_STATUSCODE_GOOD != retVal);
        } else { // node does not exist
          // presentNodes shouldn't have any allocated NodeId at this point
          retVal = handleNonExistingVariable(paActionInfo, *itMain, *variables[indexOfNodePair], indexOfNodePair,
                                             referencedNodes, paWrite);
        }
      }
    }

    // we add the references first even if it fails, since some nodes might have been created,
    // and/or some might have been already there, so deleting them will be taken care of by
    // the referencedNodesDecrement function later
    referencedNodesIncrement(referencedNodes, paActionInfo);

    if (UA_STATUSCODE_GOOD != retVal) {
      referencedNodesDecrement(paActionInfo);
    }

    for (auto referenceNode : referencedNodes) {
      UA_NodeId_delete(referenceNode);
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::initializeObjectStructMemberVariable(std::shared_ptr<CActionInfo> &paActionInfo,
                                                                            CIEC_ANY *paMember,
                                                                            bool paWrite) {
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;

    size_t indexOfNodePair = 0;
    std::vector<UA_NodeId *> referencedNodes;
    for (auto itMain = paActionInfo->getNodePairInfo().begin();
         itMain != paActionInfo->getNodePairInfo().end() && UA_STATUSCODE_GOOD == retVal; ++itMain, indexOfNodePair++) {

      std::vector<UA_NodeId *> presentNodes;
      bool nodeExists = false;
      retVal = getNode(*itMain, presentNodes, &nodeExists);

      if (UA_STATUSCODE_GOOD == retVal) {
        if (nodeExists) {
          retVal = handleExistingVariable(*paActionInfo, *itMain, paMember[indexOfNodePair], indexOfNodePair, paWrite);

          handlePresentNodes(presentNodes, referencedNodes, UA_STATUSCODE_GOOD != retVal);
        } else { // node does not exist
          // presentNodes shouldn't have any allocated NodeId at this point
          retVal = handleNonExistingVariable(*paActionInfo, *itMain, paMember[indexOfNodePair], indexOfNodePair,
                                             referencedNodes, paWrite);
        }
      }
    }
    referencedNodesIncrement(referencedNodes, *paActionInfo);
    if (UA_STATUSCODE_GOOD != retVal) {
      referencedNodesDecrement(*paActionInfo);
    }
    for (auto referenceNode : referencedNodes) {
      UA_NodeId_delete(referenceNode);
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::handleExistingVariable(CActionInfo &paActionInfo,
                                                              CActionInfo::CNodePairInfo &paNodePairInfo,
                                                              const CIEC_ANY &paVariable,
                                                              size_t paIndexOfNodePair,
                                                              bool paWrite) {

    UA_NodeId outDataType;
    UA_StatusCode retVal = UA_Server_readDataType(mUaServer, *paNodePairInfo.getNodeId(), &outDataType);

    if (UA_STATUSCODE_GOOD == retVal) {
      if (UA_NodeId_equal(&outDataType, &COPC_UA_Helper::getOPCUATypeFromAny(paVariable)->typeId)) {
        if (!paWrite) { // If we are reading a variable, it should be writable from the outside
          retVal = addWritePermission(*paNodePairInfo.getNodeId());
          if (UA_STATUSCODE_GOOD == retVal) {
            void *handle = nullptr;
            retVal = UA_Server_getNodeContext(mUaServer, *paNodePairInfo.getNodeId(), &handle);
            if (UA_STATUSCODE_GOOD == retVal) {
              if (!handle) {
                retVal = registerVariableCallBack(*paNodePairInfo.getNodeId(), paActionInfo, paIndexOfNodePair);
              } else {
                DEVLOG_ERROR(
                    "[OPC UA LOCAL]: At FB %s RD_%d the node %s has already a FB who is reading from it. Cannot "
                    "add another one\n",
                    paActionInfo.getLayer().getCommFB()->getInstanceName(), paIndexOfNodePair,
                    paNodePairInfo.getBrowsePath().c_str());
                retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
              }
            } else {
              DEVLOG_ERROR("[OPC UA LOCAL]: At FB %s RD_%d the node %s could not retrieve context. Error: %s\n",
                           paActionInfo.getLayer().getCommFB()->getInstanceName(), paIndexOfNodePair,
                           paNodePairInfo.getBrowsePath().c_str(), UA_StatusCode_name(retVal));
            }
          } else {
            DEVLOG_ERROR("[OPC UA LOCAL]: Cannot set write permission of node for port %d. Error: %s\n",
                         paIndexOfNodePair, UA_StatusCode_name(retVal));
          }
        }
      } else {
        DEVLOG_ERROR(
            "[OPC UA LOCAL]: At FB %s index %d there was the type of the existing node doesn't match the new one\n",
            paActionInfo.getLayer().getCommFB()->getInstanceName(), paIndexOfNodePair);
        retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
      }
    } else {
      DEVLOG_ERROR(
          "[OPC UA LOCAL]: At FB %s index %d there was a problem reading the type of the existing node. Error: %s\n",
          paActionInfo.getLayer().getCommFB()->getInstanceName(), paIndexOfNodePair, UA_StatusCode_name(retVal));
    }

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::handleNonExistingVariable(CActionInfo &paActionInfo,
                                                                 CActionInfo::CNodePairInfo &paNodePairInfo,
                                                                 const CIEC_ANY &paVariable,
                                                                 size_t paIndexOfNodePair,
                                                                 std::vector<UA_NodeId *> &paReferencedNodes,
                                                                 bool paWrite) {

    std::string nodeName;
    UA_StatusCode retVal = splitAndCreateFolders(paNodePairInfo.getBrowsePath(), nodeName, paReferencedNodes);
    if (UA_STATUSCODE_GOOD == retVal) {
      CCreateVariableInfo variableInformation;
      initializeCreateInfo(nodeName, paNodePairInfo, paReferencedNodes.empty() ? nullptr : paReferencedNodes.back(),
                           variableInformation);

      variableInformation.mTypeConvert = COPC_UA_Helper::getOPCUATypeFromAny(paVariable);
      variableInformation.mInitData = &paVariable;
      variableInformation.mAllowWrite = !paWrite; // write FB here means that from the outside should not be possible to
      // write and the other way around for read
      UA_NodeId baseDataVarType = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
      variableInformation.mVariableTypeNodeId =
          &baseDataVarType; // we are creating a BaseDataVariable for eRead, eWrite

      retVal = createVariableNode(variableInformation);
      if (UA_STATUSCODE_GOOD == retVal) {
        UA_NodeId *tmp = UA_NodeId_new();
        UA_NodeId_copy(variableInformation.mReturnedNodeId, tmp);
        paReferencedNodes.push_back(tmp);
        if (!paWrite) {
          retVal = registerVariableCallBack(*variableInformation.mReturnedNodeId, paActionInfo, paIndexOfNodePair);
        }
        if (UA_STATUSCODE_GOOD == retVal && paNodePairInfo.getNodeId() == nullptr) {
          paNodePairInfo.setNodeId(UA_NodeId_new());
          UA_NodeId_copy(variableInformation.mReturnedNodeId, paNodePairInfo.getNodeId());
        }
      }
    }

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::createVariableNode(const CCreateVariableInfo &paCreateVariableInfo) {

    UA_NodeId requestedNodeId;
    if (paCreateVariableInfo.mRequestedNodeId) {
      UA_NodeId_copy(paCreateVariableInfo.mRequestedNodeId, &requestedNodeId);
    } else {
      requestedNodeId = UA_NODEID_NUMERIC(
          paCreateVariableInfo.mBrowseName->namespaceIndex,
          0); // if not provided, let the server generate the NodeId using the namespace from the browsename
    }
    UA_NodeId parentNodeId;
    if (paCreateVariableInfo.mParentNodeId) {
      UA_NodeId_copy(paCreateVariableInfo.mParentNodeId, &parentNodeId);
    } else {
      parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
      DEVLOG_WARNING(
          "[OPC UA LOCAL]: You are creating a variable %.*s in the /Root folder. This is not a good practice. "
          "Try to create them in the /Root/Objects folder\n",
          paCreateVariableInfo.mBrowseName->name.length,
          reinterpret_cast<const char *>(paCreateVariableInfo.mBrowseName->name.data));
    }

    // create variable attributes
    UA_VariableAttributes variableAttributes;
    UA_VariableAttributes_init(&variableAttributes);

    void *paVarValue = UA_new(paCreateVariableInfo.mTypeConvert);

    UA_init(paVarValue, paCreateVariableInfo.mTypeConvert);
    if (nullptr != paCreateVariableInfo.mInitData) {
      COPC_UA_Helper::convertToOPCUAType(*paCreateVariableInfo.mInitData, paVarValue);
    }

    variableAttributes.dataType = paCreateVariableInfo.mTypeConvert->typeId;
    variableAttributes.valueRank = -1; // value is a scalar
    variableAttributes.displayName.locale = UA_STRING_ALLOC(mEnglishLocaleForNodes);
    UA_String_copy(&paCreateVariableInfo.mBrowseName->name, &variableAttributes.displayName.text);
    variableAttributes.description =
        UA_LOCALIZEDTEXT_ALLOC(mEnglishLocaleForNodes, mDefaultDescriptionForVariableNodes);
    variableAttributes.userAccessLevel = UA_ACCESSLEVELMASK_READ;
    if (paCreateVariableInfo.mAllowWrite) {
      variableAttributes.userAccessLevel |= UA_ACCESSLEVELMASK_WRITE;
    }
    variableAttributes.accessLevel = variableAttributes.userAccessLevel;
    UA_Variant_setScalar(&variableAttributes.value, paVarValue, paCreateVariableInfo.mTypeConvert);

    UA_StatusCode retVal = UA_Server_addVariableNode(
        mUaServer, // server
        requestedNodeId, // requestedNewNodeId
        parentNodeId, // parentNodeId
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), // referenceTypeId
        *paCreateVariableInfo.mBrowseName, // browseName
        *paCreateVariableInfo
             .mVariableTypeNodeId, // typeDefinition Reference to the type definition for the variable node
        variableAttributes, // Variable attributes
        nullptr, // instantiation callback
        paCreateVariableInfo.mReturnedNodeId); // return Node Id

    if (UA_STATUSCODE_GOOD != retVal) {
      DEVLOG_ERROR("[OPC UA LOCAL]: AddressSpace adding Variable Node %.*s failed. Error: %s\n",
                   paCreateVariableInfo.mBrowseName->name.length, paCreateVariableInfo.mBrowseName->name.data,
                   UA_StatusCode_name(retVal));
    }
    UA_NodeId_clear(&parentNodeId);
    UA_NodeId_clear(&requestedNodeId);
    UA_VariableAttributes_clear(&variableAttributes);
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::updateNodeValue(const UA_NodeId &paNodeId, const CIEC_ANY *paData) {
    UA_Variant *nodeValue = UA_Variant_new();
    UA_Variant_init(nodeValue);

    const UA_DataType *dataType = COPC_UA_Helper::getOPCUATypeFromAny(*paData);
    void *varValue = UA_new(dataType);
    COPC_UA_Helper::convertToOPCUAType(*paData, varValue);
    UA_Variant_setScalarCopy(nodeValue, varValue, dataType);
    UA_StatusCode retVal = UA_Server_writeValue(mUaServer, paNodeId, *nodeValue);
    UA_delete(varValue, dataType);
    UA_Variant_delete(nodeValue);
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::registerVariableCallBack(const UA_NodeId &paNodeId,
                                                                CActionInfo &paActionInfo,
                                                                size_t paPortIndex) {

    const UA_ValueCallback writeCallback = {nullptr, CUA_LocalCallbackFunctions::onWrite};
    UA_StatusCode retVal = UA_Server_setVariableNode_valueCallback(mUaServer, paNodeId, writeCallback);
    if (UA_STATUSCODE_GOOD == retVal) {

      auto nodeContext = std::make_unique<UA_VariableContext_Handle>(paActionInfo, paPortIndex);

      retVal = UA_Server_setNodeContext(mUaServer, paNodeId, nodeContext.get());
      if (UA_STATUSCODE_GOOD != retVal) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not set callback context for node. Error: %s\n",
                     UA_StatusCode_name(retVal));
        retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
      } else {
        mNodeCallbackHandles.emplace_back(std::move(nodeContext));
      }
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: Could not set callback function for node. Error: %s\n", UA_StatusCode_name(retVal));
      retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::addWritePermission(const UA_NodeId &paNodeId) {
    UA_StatusCode retVal =
        UA_Server_writeAccessLevel(mUaServer, paNodeId, UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE);
    if (UA_STATUSCODE_GOOD != retVal) {
      DEVLOG_WARNING("[OPC UA LOCAL]: Cannot set write permission of node. Error: %s\n", UA_StatusCode_name(retVal));
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::initializeCreateMethod(CActionInfo &paActionInfo) {

    std::vector<UA_NodeId *> referencedNodes;
    std::vector<UA_NodeId *> presentNodes;

    auto itMethodNodePairInfo = paActionInfo.getNodePairInfo().begin();

    bool nodeExists = false;
    UA_StatusCode retVal = getNode(*itMethodNodePairInfo, presentNodes, &nodeExists);

    if (UA_STATUSCODE_GOOD == retVal) {
      if (nodeExists) {

        UA_NodeId parent = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);

        // look for parent object
        if (presentNodes.size() > 1) {
          parent = *presentNodes[presentNodes.size() - 2];
        }

        retVal = handleExistingMethod(paActionInfo, parent);

        handlePresentNodes(presentNodes, referencedNodes, UA_STATUSCODE_GOOD != retVal);
      } else { // node does not exist
        // presentNodes shouldn't have any allocated NodeId at this point

        std::string nodeName;
        retVal = splitAndCreateFolders(itMethodNodePairInfo->getBrowsePath(), nodeName, referencedNodes);
        if (UA_STATUSCODE_GOOD == retVal) {
          CCreateMethodInfo methodInformation(static_cast<CLocalMethodInfo &>(paActionInfo));
          initializeCreateInfo(nodeName, *itMethodNodePairInfo,
                               referencedNodes.empty() ? nullptr : referencedNodes.back(), methodInformation);

          methodInformation.mOutputSize = paActionInfo.getSendSize();
          methodInformation.mInputSize = paActionInfo.getReceiveSize();

          createMethodArguments(paActionInfo, methodInformation);

          UA_NodeId *result = nullptr;
          retVal = createMethodNode(methodInformation, &result);
          if (UA_STATUSCODE_GOOD == retVal) {
            itMethodNodePairInfo->setNodeId(result);
            UA_NodeId *tmp = UA_NodeId_new();
            UA_NodeId_copy(methodInformation.mReturnedNodeId, tmp);
            referencedNodes.push_back(tmp);
          }
        }
      }
    }

    // we add the references first even if it fails, since some nodes might have been created,
    // and/or some might have been already there, so deleting them will be taken care of by
    // the referencedNodesDecrement function later
    referencedNodesIncrement(referencedNodes, paActionInfo);

    if (UA_STATUSCODE_GOOD != retVal) {
      referencedNodesDecrement(paActionInfo);
    }

    for (auto referenceNode : referencedNodes) {
      UA_NodeId_delete(referenceNode);
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::handleExistingMethod(CActionInfo &paActionInfo, const UA_NodeId &paParentNode) {
    auto it = paActionInfo.getNodePairInfo().begin();
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;

    DEVLOG_INFO("[OPC UA LOCAL]: Adding a callback for an existing method at %s\n", it->getBrowsePath().c_str());

    // check if the method was already referenced by another FB
    for (auto &methodContext : mMethodsContexts) {
      if (UA_NodeId_equal(&paParentNode, methodContext.mParentNodeId) &&
          UA_NodeId_equal(it->getNodeId(), methodContext.mMethodNodeId)) {
        DEVLOG_ERROR("[OPC UA LOCAL]: The FB %s is trying to reference a local method at %s which has already a FB who "
                     "is referencing it. Cannot add another one\n",
                     paActionInfo.getLayer().getCommFB()->getInstanceName(), it->getBrowsePath().c_str());
        retVal = UA_STATUSCODE_BADINTERNALERROR;
        break;
      }
    }

    // TODO: check types of existing method to this layer

    if (UA_STATUSCODE_GOOD == retVal) {
      retVal = UA_Server_setMethodNode_callback(mUaServer, *it->getNodeId(),
                                                COPC_UA_Local_Handler::CUA_LocalCallbackFunctions::onServerMethodCall);
      if (UA_STATUSCODE_GOOD == retVal) {
        retVal = UA_Server_setNodeContext(mUaServer, *it->getNodeId(), this);
        if (UA_STATUSCODE_GOOD == retVal) {
          mMethodsContexts.emplace_back(paParentNode, it->getNodeId(), static_cast<CLocalMethodInfo &>(paActionInfo));
        } else {
          DEVLOG_ERROR("[OPC UA LOCAL]: Could not set context function for method at %s. Error: %s\n",
                       paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
        }
      } else {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not set callback function for method at %s. Error: %s\n",
                     paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
      }
    }
    return retVal;
  }

  void COPC_UA_Local_Handler::createMethodArguments(CActionInfo &paActionInfo,
                                                    CCreateMethodInfo &paCreateMethodInfo) const {
    const SFBInterfaceSpec &interfaceFB(paActionInfo.getLayer().getCommFB()->getFBInterfaceSpec());

    const CIEC_ANY *const *dataToSend = paActionInfo.getDataToSend();
    const CIEC_ANY *const *dataToReceive = paActionInfo.getDataToReceive();

    paCreateMethodInfo.mOutputArguments =
        static_cast<UA_Argument *>(UA_Array_new(paCreateMethodInfo.mOutputSize, &UA_TYPES[UA_TYPES_ARGUMENT]));
    paCreateMethodInfo.mInputArguments =
        static_cast<UA_Argument *>(UA_Array_new(paCreateMethodInfo.mInputSize, &UA_TYPES[UA_TYPES_ARGUMENT]));

    for (size_t i = 0; i < paCreateMethodInfo.mOutputSize + paCreateMethodInfo.mInputSize; i++) {
      UA_Argument *arg;
      if (i < paCreateMethodInfo.mOutputSize) {
        arg = &(paCreateMethodInfo.mOutputArguments)[i];
        UA_Argument_init(arg);
        arg->name =
            UA_STRING_ALLOC(interfaceFB.mDINames[i + 2].data()); // we store the names of the SDs/RDs as names for the
        // arguments names. Not so nice. + 2 skips the QI and ID
        arg->dataType = COPC_UA_Helper::getOPCUATypeFromAny(*dataToSend[i])->typeId;
      } else {
        arg = &(paCreateMethodInfo.mInputArguments)[i - paCreateMethodInfo.mOutputSize];
        UA_Argument_init(arg);
        arg->name = UA_STRING_ALLOC(
            interfaceFB.mDONames[i - paCreateMethodInfo.mOutputSize + 2].data()); // + 2 skips the QO and STATUS
        arg->dataType = COPC_UA_Helper::getOPCUATypeFromAny(*dataToReceive[i - paCreateMethodInfo.mOutputSize])->typeId;
      }

      arg->arrayDimensionsSize = 0;
      arg->arrayDimensions = nullptr;
      arg->description = UA_LOCALIZEDTEXT_ALLOC(mEnglishLocaleForNodes, "Method parameter");
      arg->valueRank = -1;
    }
  }

  UA_StatusCode COPC_UA_Local_Handler::createMethodNode(CCreateMethodInfo &paCreateMethodInfo, UA_NodeId **paNodeId) {

    UA_NodeId requestedNodeId;
    if (paCreateMethodInfo.mRequestedNodeId) {
      UA_NodeId_copy(paCreateMethodInfo.mRequestedNodeId, &requestedNodeId);
    } else {
      requestedNodeId = UA_NODEID_NUMERIC(
          paCreateMethodInfo.mBrowseName->namespaceIndex,
          0); // if not provided, let the server generate the NodeId using the namespace from the browsename
    }

    UA_NodeId parentNodeId;
    if (paCreateMethodInfo.mParentNodeId) {
      UA_NodeId_copy(paCreateMethodInfo.mParentNodeId, &parentNodeId);
    } else {
      parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
      DEVLOG_WARNING("[OPC UA LOCAL]: You are creating a Method %.*s in the /Root folder. This is not a good practice. "
                     "Try to create them in the /Root/Objects folder\n",
                     paCreateMethodInfo.mBrowseName->name.length,
                     reinterpret_cast<const char *>(paCreateMethodInfo.mBrowseName->name.data));
    }

    UA_MethodAttributes methodAttributes;
    UA_MethodAttributes_init(&methodAttributes);
    methodAttributes.description = UA_LOCALIZEDTEXT_ALLOC(mEnglishLocaleForNodes, "Method which can be called");
    methodAttributes.executable = true;
    methodAttributes.userExecutable = true;
    methodAttributes.displayName.locale = UA_STRING_ALLOC(mEnglishLocaleForNodes);
    UA_String_copy(&paCreateMethodInfo.mBrowseName->name, &methodAttributes.displayName.text);

    UA_StatusCode retVal = UA_Server_addMethodNode(
        mUaServer, requestedNodeId, parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        *paCreateMethodInfo.mBrowseName, methodAttributes, CUA_LocalCallbackFunctions::onServerMethodCall,
        paCreateMethodInfo.mInputSize, paCreateMethodInfo.mInputArguments, paCreateMethodInfo.mOutputSize,
        paCreateMethodInfo.mOutputArguments, this, paCreateMethodInfo.mReturnedNodeId);

    if (UA_STATUSCODE_GOOD == retVal) {
      if (!*paNodeId) {
        *paNodeId = UA_NodeId_new();
        UA_NodeId_copy(paCreateMethodInfo.mReturnedNodeId, *paNodeId);
      }

      mMethodsContexts.emplace_back(parentNodeId, *paNodeId, paCreateMethodInfo.mLocalMethodInfo);
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: OPC UA could not create method at %s. Error: %s\n",
                   paCreateMethodInfo.mLocalMethodInfo.getLayer().getCommFB()->getInstanceName(),
                   UA_StatusCode_name(retVal));
    }

    UA_NodeId_clear(&parentNodeId);
    UA_NodeId_clear(&requestedNodeId);
    UA_MethodAttributes_clear(&methodAttributes);
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::initializeCreateNode(CActionInfo &paActionInfo) const {
    // The main process is done in the execution
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;

    auto &nodePair = paActionInfo.getNodePairInfo().back();

    if (nodePair.getBrowsePath().empty()) { // the browsename of the instance is mandatory
      retVal = UA_STATUSCODE_BADINTERNALERROR;
      DEVLOG_ERROR(
          "[OPC UA LOCAL]: The BrowsePath of the instance is mandatory for creating a node at FB %s. Error: %s\n",
          paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::initializeDeleteNode(const CActionInfo &) const {
    // nothing to do here
    return UA_STATUSCODE_GOOD;
  }

  UA_StatusCode COPC_UA_Local_Handler::executeWrite(CActionInfo &paActionInfo) {
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;
    const CIEC_ANY *const *dataToSend = paActionInfo.getDataToSend();
    size_t indexOfNodePair = 0;
    for (auto it = paActionInfo.getNodePairInfo().begin(); it != paActionInfo.getNodePairInfo().end();
         ++it, indexOfNodePair++) {

      retVal = updateNodeValue(*it->getNodeId(), dataToSend[indexOfNodePair]);
      if (UA_STATUSCODE_GOOD != retVal) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not convert value to write for port %d at FB %s. Error: %s\n",
                     indexOfNodePair, paActionInfo.getLayer().getCommFB()->getInstanceName(),
                     UA_StatusCode_name(retVal));
        break;
      }
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::executeStructWrite(CActionInfo &paActionInfo, CIEC_ANY &paMember) {
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;

    if (paMember.getDataTypeID() == CIEC_ANY::e_STRUCT) {
      CIEC_STRUCT &structType = static_cast<CIEC_STRUCT &>(paMember);
      CStructActionInfo &structActionInfo = static_cast<CStructActionInfo &>(paActionInfo);
      std::vector<std::shared_ptr<CActionInfo>> memberActionInfos = structActionInfo.getMemberActionInfos();
      for (size_t i = 0; i < memberActionInfos.size(); i++) {
        std::shared_ptr<CActionInfo> memberActionInfo = memberActionInfos[i];
        CIEC_ANY *member = structType.getMember(i);
        retVal = executeStructWrite(*memberActionInfo, *member);
        if (retVal != UA_STATUSCODE_GOOD) {
          return retVal;
        }
      }
    } else {
      auto it = paActionInfo.getNodePairInfo().begin();
      retVal = updateNodeValue(*it->getNodeId(), &paMember);
      if (UA_STATUSCODE_GOOD != retVal) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not convert value to write for node %s at FB %s. Error: %s\n",
                     (*paActionInfo.getNodePairInfo().begin()).getBrowsePath().c_str(),
                     paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
        return retVal;
      }
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::executeCreateMethod(CActionInfo &paActionInfo) {
    // This is the return of a local method call, when RSP is triggered

    UA_StatusCode retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
    CLocalMethodCall *localMethodCall = getLocalMethodCall(static_cast<CLocalMethodInfo &>(paActionInfo));
    if (localMethodCall) {
      const CIEC_ANY *const *dataToSend = paActionInfo.getDataToSend();
      // copy SD values to output
      for (size_t i = 0; i < localMethodCall->mSendHandle->mData.size(); i++) {
        COPC_UA_Helper::fillVariant(*localMethodCall->mSendHandle->mData[i], *dataToSend[i]);
      }

      localMethodCall->mActionInfo->getResultReady().inc();
      retVal = UA_STATUSCODE_GOOD;
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: The method being returned hasn't been called before of FB %s\n",
                   paActionInfo.getLayer().getCommFB()->getInstanceName());
    }

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::executeCreateObject(CActionInfo &paActionInfo) {
    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

    auto itTypeNodePairInfo = paActionInfo.getNodePairInfo().begin();

    // look for type first
    if (isNodePresent(*itTypeNodePairInfo)) {

      auto itInstance = paActionInfo.getNodePairInfo().begin();
      ++itInstance;

      std::vector<UA_NodeId *> referencedNodes;
      bool nodeExists = false;
      // check if an instance is already present
      retVal = getNode(*itInstance, referencedNodes, &nodeExists);

      if (UA_STATUSCODE_GOOD == retVal) {
        if (!nodeExists) {

          std::string nodeName;
          retVal = splitAndCreateFolders(itInstance->getBrowsePath(), nodeName, referencedNodes);
          if (UA_STATUSCODE_GOOD == retVal) {
            CCreateObjectInfo createInformation;

            initializeCreateInfo(nodeName, *itInstance, referencedNodes.empty() ? nullptr : referencedNodes.back(),
                                 createInformation);
            createInformation.mTypeNodeId = itTypeNodePairInfo->getNodeId();

            retVal = createObjectNode(createInformation);

            if (UA_STATUSCODE_GOOD == retVal) {
              UA_NodeId *tmp = UA_NodeId_new();
              UA_NodeId_copy(createInformation.mReturnedNodeId, tmp);
              referencedNodes.push_back(tmp);
            }
          }
        } else {
          DEVLOG_ERROR("[OPC UA LOCAL]: You are trying to create an object which already exists at FB %s\n",
                       paActionInfo.getLayer().getCommFB()->getInstanceName());
          retVal = UA_STATUSCODE_BADINTERNALERROR;
        }
      }

      // we add the references first even if itType fails, since some nodes might have been created,
      // and/or some might have been already there, so deleting them will be taken care of by
      // the referencedNodesDecrement function later
      referencedNodesIncrement(referencedNodes, paActionInfo);

      if (UA_STATUSCODE_GOOD != retVal) {
        referencedNodesDecrement(paActionInfo);
      }

      for (auto referenceNode : referencedNodes) {
        UA_NodeId_delete(referenceNode);
      }

    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: The type of the object to create could not be found at FB %s\n",
                   paActionInfo.getLayer().getCommFB()->getInstanceName());
    }

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::createObjectNode(const CCreateObjectInfo &paCreateObjectInfo) const {

    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

    UA_NodeId requestedNodeId;
    if (paCreateObjectInfo.mRequestedNodeId) {
      UA_NodeId_copy(paCreateObjectInfo.mRequestedNodeId, &requestedNodeId);
    } else {
      requestedNodeId = UA_NODEID_NUMERIC(
          paCreateObjectInfo.mBrowseName->namespaceIndex,
          0); // if not provided, let the server generate the NodeId using the namespace from the browsename
    }

    UA_NodeId parentNodeId;
    if (paCreateObjectInfo.mParentNodeId) {
      UA_NodeId_copy(paCreateObjectInfo.mParentNodeId, &parentNodeId);
    } else {
      parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
      DEVLOG_WARNING(
          "[OPC UA LOCAL]: You are creating an Object %.*s in the /Root folder. This is not a good practice. "
          "Try to create them in the /Root/Objects folder\n",
          paCreateObjectInfo.mBrowseName->name.length,
          reinterpret_cast<const char *>(paCreateObjectInfo.mBrowseName->name.data));
    }

    std::string nodeName(reinterpret_cast<const char *>(paCreateObjectInfo.mBrowseName->name.data),
                         paCreateObjectInfo.mBrowseName->name.length);

    UA_ObjectAttributes oAttr;
    UA_ObjectAttributes_init(&oAttr);
    oAttr.description = UA_LOCALIZEDTEXT_ALLOC("", nodeName.c_str());
    oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", nodeName.c_str());
    retVal =
        UA_Server_addObjectNode(mUaServer, requestedNodeId, parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                *paCreateObjectInfo.mBrowseName, *paCreateObjectInfo.mTypeNodeId, oAttr, nullptr,
                                paCreateObjectInfo.mReturnedNodeId);
    if (UA_STATUSCODE_GOOD != retVal) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Could not addObjectNode. Error: %s\n", UA_StatusCode_name(retVal));
    }
    UA_NodeId_clear(&requestedNodeId);
    UA_NodeId_clear(&parentNodeId);
    UA_ObjectAttributes_clear(&oAttr);

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::executeCreateVariable(CActionInfo &paActionInfo) {
    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

    auto itVariableTypeNodePairInfo = paActionInfo.getNodePairInfo().begin();

    // look for variable type first
    if (isNodePresent(*itVariableTypeNodePairInfo)) {
      auto itDataValueTypeNodePairInfo = paActionInfo.getNodePairInfo().begin();
      ++itDataValueTypeNodePairInfo;

      // look for data value type
      if (isNodePresent(*itDataValueTypeNodePairInfo)) {

        auto &nodePair = paActionInfo.getNodePairInfo().back();

        std::vector<UA_NodeId *> referencedNodes;
        bool nodeExists = false;
        // check if a nodePair is already present
        retVal = getNode(nodePair, referencedNodes, &nodeExists);

        if (UA_STATUSCODE_GOOD == retVal) {
          if (!nodeExists) {

            std::string nodeName;
            retVal = splitAndCreateFolders(nodePair.getBrowsePath(), nodeName, referencedNodes);
            if (UA_STATUSCODE_GOOD == retVal) {
              CCreateVariableInfo createInformation;

              initializeCreateInfo(nodeName, nodePair, referencedNodes.empty() ? nullptr : referencedNodes.back(),
                                   createInformation);
              createInformation.mVariableTypeNodeId = itVariableTypeNodePairInfo->getNodeId();
              const UA_NodeId *dataValueTypeNodeId = itDataValueTypeNodePairInfo->getNodeId();

              createInformation.mTypeConvert = UA_findDataType(dataValueTypeNodeId);
              createInformation.mAllowWrite = true;

              retVal = createVariableNode(createInformation);

              if (UA_STATUSCODE_GOOD == retVal) {
                UA_NodeId *tmp = UA_NodeId_new();
                UA_NodeId_copy(createInformation.mReturnedNodeId, tmp);
                referencedNodes.push_back(tmp);
              }
            }
          } else {
            DEVLOG_ERROR("[OPC UA LOCAL]: You are trying to create a variable which already exists at FB %s\n",
                         paActionInfo.getLayer().getCommFB()->getInstanceName());
            retVal = UA_STATUSCODE_BADINTERNALERROR;
          }
        }

        // we add the references first even if itType fails, since some nodes might have been created,
        // and/or some might have been already there, so deleting them will be taken care of by
        // the referencedNodesDecrement function later
        referencedNodesIncrement(referencedNodes, paActionInfo);

        if (UA_STATUSCODE_GOOD != retVal) {
          referencedNodesDecrement(paActionInfo);
        }

        for (auto referenceNode : referencedNodes) {
          UA_NodeId_delete(referenceNode);
        }
      } else {
        DEVLOG_ERROR("[OPC UA LOCAL]: The data value type of the variable to create could not be found at FB %s\n",
                     paActionInfo.getLayer().getCommFB()->getInstanceName());
      }
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: The type of the variable to create could not be found at FB %s\n",
                   paActionInfo.getLayer().getCommFB()->getInstanceName());
    }

    return retVal;
  }

  bool COPC_UA_Local_Handler::isNodePresent(CActionInfo::CNodePairInfo &paNodePairInfo) {
    bool nodeExists = false;
    std::vector<UA_NodeId *> referencedNodes;

    UA_StatusCode retVal = getNode(paNodePairInfo, referencedNodes, &nodeExists);

    if (UA_STATUSCODE_GOOD == retVal) {
      // we don't need the nodes from the type
      for (auto presentNode : referencedNodes) {
        UA_NodeId_delete(presentNode);
      }
      referencedNodes.clear();
    }
    return nodeExists;
  }

  UA_StatusCode COPC_UA_Local_Handler::executeDeleteObject(CActionInfo &paActionInfo) {

    bool nodeExists = false;
    auto itInstance = paActionInfo.getNodePairInfo().begin();
    std::vector<UA_NodeId *> referencedNodes;

    // look for instance
    UA_StatusCode retVal = getNode(*itInstance, referencedNodes, &nodeExists);

    if (UA_STATUSCODE_GOOD == retVal) {
      if (nodeExists) {
        retVal = UA_Server_deleteNode(mUaServer, *itInstance->getNodeId(), true);
      } else {
        DEVLOG_ERROR("[OPC UA LOCAL]: The instance of the object to delete could not be found for FB %s\n",
                     paActionInfo.getLayer().getCommFB()->getInstanceName());
        retVal = UA_STATUSCODE_BADINTERNALERROR;
      }

      for (auto referenceNode : referencedNodes) {
        UA_NodeId_delete(referenceNode);
      }
      referencedNodes.clear();
    }

    return retVal;
  }

  void COPC_UA_Local_Handler::initializeCreateInfo(std::string &paNodeName,
                                                   const CActionInfo::CNodePairInfo &paNodePairInfo,
                                                   const UA_NodeId *paParentNodeId,
                                                   CCreateInfo &paResult) const {
    COPC_UA_Helper::getBrowsenameFromNodeName(
        paNodeName.c_str(), scmDefaultBrowsenameNameSpace,
        *paResult.mBrowseName); // this cannot fail here anymore, since it was checked already with getNode
    paResult.mRequestedNodeId = paNodePairInfo.getNodeId();
    paResult.mParentNodeId = paParentNodeId;
  }

  UA_BrowseResult COPC_UA_Local_Handler::browseServer(UA_BrowseDescription &paBrowseDescription) {
    return UA_Server_browse(mUaServer, 500, &paBrowseDescription);
  }

  UA_StatusCode COPC_UA_Local_Handler::getNode(CActionInfo::CNodePairInfo &paNodePairInfo,
                                               std::vector<UA_NodeId *> &paFoundNodeIds,
                                               bool *paIsPresent) {
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;
    *paIsPresent = false;

    if (!paNodePairInfo.getBrowsePath().empty()) {
      UA_BrowsePath *browsePaths = nullptr;
      size_t pathCount = 0;
      size_t firstNonExistingNode = 0;

      std::vector<UA_NodeId *> existingNodeIds;
      retVal = translateBrowseNameAndStore(paNodePairInfo.getBrowsePath().c_str(), &browsePaths, &pathCount,
                                           &firstNonExistingNode, existingNodeIds);
      if (UA_STATUSCODE_GOOD == retVal) {
        if (firstNonExistingNode == pathCount) { // all nodes exist
          *paIsPresent = true;
          if (paNodePairInfo.getNodeId() != nullptr) { // nodeID was provided
            if (!UA_NodeId_equal(paNodePairInfo.getNodeId(), existingNodeIds.back())) {
              *paIsPresent = false; // the found Node has not the same NodeId.
            }
          } else { // if no nodeID was provided, the found Node is stored in the nodePairInfo
            paNodePairInfo.setNodeId(UA_NodeId_new());
            UA_NodeId_copy(existingNodeIds.back(), paNodePairInfo.getNodeId());
          }
        }

        for (auto existingNode : existingNodeIds) {
          if (!*paIsPresent) {
            UA_NodeId_delete(existingNode);
          } else {
            paFoundNodeIds.push_back(existingNode);
          }
        }
        COPC_UA_Helper::releaseBrowseArgument(*browsePaths, pathCount);
      }
    } else {
      void *handle = nullptr;
      // we use UA_Server_getNodeContext just to check if the node exist in the addressspace
      if (UA_STATUSCODE_BADNODEIDUNKNOWN != UA_Server_getNodeContext(mUaServer, *paNodePairInfo.getNodeId(), &handle)) {
        *paIsPresent = true;
      }
    }

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::translateBrowseNameAndStore(const char *paBrowsePath,
                                                                   UA_BrowsePath **paBrowsePaths,
                                                                   size_t *paFoldercount,
                                                                   size_t *paFirstNonExistingNode,
                                                                   std::vector<UA_NodeId *> &paFoundNodeIds) const {

    UA_StatusCode retVal = COPC_UA_Helper::prepareBrowseArgument(paBrowsePath, paBrowsePaths, paFoldercount);

    if (UA_STATUSCODE_GOOD == retVal) {
      UA_BrowsePathResult *browsePathsResults = nullptr;
      std::vector<UA_NodeId *> storedNodeIds;

      browsePathsResults =
          static_cast<UA_BrowsePathResult *>(UA_Array_new(*paFoldercount * 2, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT]));
      for (unsigned int i = 0; i < *paFoldercount * 2; i++) {
        browsePathsResults[i] = UA_Server_translateBrowsePathToNodeIds(mUaServer, &(*paBrowsePaths)[i]);
      }
      retVal = storeAlreadyExistingNodes(browsePathsResults, *paFoldercount, paFirstNonExistingNode, storedNodeIds);

      for (auto storedNodeId : storedNodeIds) {
        if (UA_STATUSCODE_GOOD != retVal) {
          UA_NodeId_delete(storedNodeId);
        } else {
          paFoundNodeIds.push_back(storedNodeId);
        }
      }

      UA_Array_delete(browsePathsResults, *paFoldercount * 2, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT]);
    }

    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::storeAlreadyExistingNodes(const UA_BrowsePathResult *paBrowsePathsResults,
                                                                 size_t paFolderCnt,
                                                                 size_t *paFirstNonExistingNode,
                                                                 std::vector<UA_NodeId *> &paCreatedNodeIds) const {
    size_t foundFolderOffset = 0;
    int retVal; // no unsigned, because we need to check for -1
    bool offsetFound = false;
    for (retVal = static_cast<int>(paFolderCnt) - 1; retVal >= 0; retVal--) {
      if (UA_STATUSCODE_GOOD ==
          paBrowsePathsResults[retVal].statusCode) { // find first existing node. Check for isInverse = TRUE
        foundFolderOffset = 0;
        offsetFound = true;
      } else if (UA_STATUSCODE_GOOD ==
                 paBrowsePathsResults[paFolderCnt + retVal].statusCode) { // and for isInverse = FALSE
        foundFolderOffset = paFolderCnt;
        offsetFound = true;
      }

      if (offsetFound) {
        break;
      }
    }

    if (-1 != retVal) {
      if (paBrowsePathsResults[foundFolderOffset + retVal].targetsSize == 0) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not translate browse paths to node IDs. Target size is 0.\n");
        return UA_STATUSCODE_BADINTERNALERROR;
      } else {
        if (paBrowsePathsResults[foundFolderOffset + retVal].targetsSize > 1) {
          DEVLOG_WARNING("[OPC UA LOCAL]: The given browse path has multiple results for the same path. Taking the "
                         "first result.\n");
        }
      }
    }
    retVal++;

    for (int j = 0; j < retVal; j++) {
      UA_NodeId *tmp = UA_NodeId_new();
      UA_NodeId_copy(&paBrowsePathsResults[foundFolderOffset + j].targets[0].targetId.nodeId, tmp);
      paCreatedNodeIds.push_back(tmp);
    }

    *paFirstNonExistingNode = static_cast<size_t>(retVal);
    return UA_STATUSCODE_GOOD;
  }

  UA_StatusCode COPC_UA_Local_Handler::createFolders(const char *paFolders,
                                                     std::vector<UA_NodeId *> &paCreatedNodeIds) const {

    UA_BrowsePath *browsePaths = nullptr;
    size_t folderCnt = 0;
    size_t firstNonExistingNode;
    std::vector<UA_NodeId *> existingNodeIds;
    UA_StatusCode retVal =
        translateBrowseNameAndStore(paFolders, &browsePaths, &folderCnt, &firstNonExistingNode, existingNodeIds);

    if (UA_STATUSCODE_GOOD == retVal) {
      UA_NodeId parentNodeId;
      if (existingNodeIds.empty()) {
        UA_NodeId objectsNode = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
        UA_NodeId_copy(&objectsNode, &parentNodeId);
      } else {
        UA_NodeId_copy(existingNodeIds.back(), &parentNodeId);
      }

      std::vector<UA_NodeId *> createdNodeIds;

      // create all the nodes on the way
      for (size_t j = firstNonExistingNode; j < folderCnt; j++) {

        UA_NodeId type = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);
        CCreateObjectInfo createInformation;

        createInformation.mRequestedNodeId = nullptr;
        createInformation.mParentNodeId = &parentNodeId;
        UA_QualifiedName_copy(&browsePaths[folderCnt - 1].relativePath.elements[j].targetName,
                              createInformation.mBrowseName);
        createInformation.mTypeNodeId = &type;

        retVal = createObjectNode(createInformation);

        if (UA_STATUSCODE_GOOD != retVal) {
          DEVLOG_ERROR("[OPC UA LOCAL]: Could not create folder %.*s in path %s. Error: %s\n",
                       createInformation.mBrowseName->name.length,
                       reinterpret_cast<const char *>(createInformation.mBrowseName->name.data), paFolders,
                       UA_StatusCode_name(retVal));
          break;
        }

        UA_NodeId *tmp = UA_NodeId_new();
        UA_NodeId_copy(createInformation.mReturnedNodeId, tmp);
        createdNodeIds.push_back(tmp);

        UA_NodeId_clear(&parentNodeId);
        UA_NodeId_copy(createInformation.mReturnedNodeId, &parentNodeId);
      }

      UA_NodeId_clear(&parentNodeId);
      for (auto existingNode : existingNodeIds) {
        if (UA_STATUSCODE_GOOD != retVal) {
          UA_NodeId_delete(existingNode);
        } else {
          paCreatedNodeIds.push_back(existingNode);
        }
      }

      for (auto createdNode : createdNodeIds) {
        paCreatedNodeIds.push_back(createdNode); // store the NodeId because the object was created, and if the next
        // fails, the node should be referenced
      }

      COPC_UA_Helper::releaseBrowseArgument(*browsePaths, folderCnt);
    }
    return retVal;
  }

  UA_StatusCode COPC_UA_Local_Handler::splitAndCreateFolders(const std::string &paBrowsePath,
                                                             std::string &paNodeName,
                                                             std::vector<UA_NodeId *> &paRreferencedNodes) const {
    std::string folders;
    UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
    if (splitFoldersFromNode(paBrowsePath, folders, paNodeName)) {
      retVal = UA_STATUSCODE_GOOD;
      if (!folders.empty()) {
        retVal = createFolders(folders.c_str(), paRreferencedNodes);
      }
    }
    return retVal;
  }

  bool COPC_UA_Local_Handler::splitFoldersFromNode(const std::string &paOriginal,
                                                   std::string &paFolder,
                                                   std::string &paNodeName) const {
    bool retVal = false;

    if (COPC_UA_Helper::isBrowsePathValid(paOriginal)) {
      const auto originalLength = paOriginal.length();
      auto originalEndIterIndex = 0;
      if (paOriginal.back() == '/') {
        ++originalEndIterIndex;
      }
      auto findIndex = paOriginal.rfind('/', originalLength - (originalEndIterIndex + 1));
      if (findIndex != std::string::npos) {
        retVal = true;
        paNodeName = std::string(paOriginal.begin() + findIndex + 1, paOriginal.end() - originalEndIterIndex);

        if (findIndex != 0) { // if '/' was not only found at the beginning, e.g. the Node is not at the root
          paFolder = std::string(paOriginal.begin(), paOriginal.begin() + findIndex);
        }
      }
    }

    return retVal;
  }

  void COPC_UA_Local_Handler::handlePresentNodes(const std::vector<UA_NodeId *> &paPresentNodes,
                                                 std::vector<UA_NodeId *> &paReferencedNodes,
                                                 bool paFailed) const {
    for (auto presentNodes : paPresentNodes) {
      if (paFailed) {
        UA_NodeId_delete(presentNodes);
      } else {
        paReferencedNodes.push_back(presentNodes);
      }
    }
  }

  COPC_UA_Local_Handler::CLocalMethodCall &
  COPC_UA_Local_Handler::addMethodCall(CLocalMethodInfo &paActionInfo,
                                       COPC_UA_Helper::UA_SendVariable_handle &paHandleRecv) {
    util::CCriticalRegion criticalRegion(mMethodCallsMutex);
    mMethodCalls.emplace_back(paActionInfo, paHandleRecv);
    return mMethodCalls.back();
  }

  void COPC_UA_Local_Handler::removeMethodCall(const CLocalMethodCall &toRemove) {
    util::CCriticalRegion criticalRegion(mMethodCallsMutex);
    std::erase(mMethodCalls, toRemove);
  }

  COPC_UA_Local_Handler::CLocalMethodCall *
  COPC_UA_Local_Handler::getLocalMethodCall(const CLocalMethodInfo &paActionInfo) {
    util::CCriticalRegion criticalRegion(mMethodCallsMutex);
    CLocalMethodCall *retVal = nullptr;
    for (auto &methodCall : mMethodCalls) {
      if (methodCall.mActionInfo == &paActionInfo) {
        retVal = &methodCall;
        break;
      }
    }
    return retVal;
  }

  // ******************** CALLBACKS *************************

  UA_StatusCode
  COPC_UA_Local_Handler::CUA_LocalCallbackFunctions::onServerMethodCall( // We omit SONAR only for the parameters
      UA_Server *,
      const UA_NodeId *,
      void *,
      const UA_NodeId *paMethodNodeId,
      void *paMethodContext,
      const UA_NodeId *paParentNodeId,
      void *, // NOSONAR
      size_t paInputSize,
      const UA_Variant *paInput,
      size_t paOutputSize,
      UA_Variant *paOutput // NOSONAR
  ) {

    UA_StatusCode retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
    CLocalMethodInfo *localMethodHandle = nullptr;
    COPC_UA_Local_Handler *thisHandler = static_cast<COPC_UA_Local_Handler *>(paMethodContext);
    for (auto &methodContext : thisHandler->mMethodsContexts) {
      if (UA_NodeId_equal(paParentNodeId, methodContext.mParentNodeId) &&
          UA_NodeId_equal(paMethodNodeId, methodContext.mMethodNodeId)) {
        localMethodHandle = &methodContext.mActionInfo;
        break;
      }
    }

    if (!localMethodHandle) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Method doesn't have any FB referencing it\n");
    } else {
      if (paInputSize != localMethodHandle->getReceiveSize() || paOutputSize != localMethodHandle->getSendSize()) {
        DEVLOG_ERROR("[OPC UA LOCAL]: method call got invalid number of arguments. In: %d==%d, Out: %d==%d\n",
                     localMethodHandle->getReceiveSize(), paInputSize, localMethodHandle->getSendSize(), paOutput);
      } else {

        COPC_UA_Helper::UA_SendVariable_handle sendHandle;
        COPC_UA_Helper::UA_RecvVariable_handle recvHandle;

        for (size_t i = 0; i < paInputSize; i++) {
          recvHandle.mData.push_back(&paInput[i]);
        }

        for (size_t i = 0; i < paOutputSize; i++) {
          sendHandle.mData.push_back(&paOutput[i]);
        }

        // Handle return of receive mData
        if (e_ProcessDataOk ==
            localMethodHandle->getLayer().recvData(static_cast<const void *>(&recvHandle),
                                                   0)) { // TODO: add multidimensional mData handling with 'range'.

          localMethodHandle->getLayer().getCommFB()->interruptCommFB(&localMethodHandle->getLayer());

          // when the method finishes, and RSP is triggered, sendHandle will be filled with the right information
          const CLocalMethodCall &localMethodCall =
              util::getExtEvHandler<COPC_UA_Local_Handler>(*localMethodHandle->getLayer().getCommFB())
                  .addMethodCall(*localMethodHandle, sendHandle);

          util::getExtEvHandler<COPC_UA_Local_Handler>(*localMethodHandle->getLayer().getCommFB())
              .startNewEventChain(localMethodHandle->getLayer().getCommFB());

          // This function is called from the opcua server thread, so we release the lock so the method can be finished
          // by forte in executeAction there might be a problem here if another resource, which is not executing the
          // method response, uses the server and changes it somehow that it breaks something, like deleting the method
          // maybe
          thisHandler->mServerAccessMutex.unlock();
          // wait For semaphore, which will be released by execute Local Method in this handler
          if (!localMethodHandle->getResultReady().timedWait(scmMethodCallTimeoutInNanoSeconds)) {
            DEVLOG_ERROR("[OPC UA LOCAL]: method call did not get result values within timeout of %u nanoseconds.\n",
                         scmMethodCallTimeoutInNanoSeconds);
            retVal = UA_STATUSCODE_BADTIMEOUT;
          } else {
            retVal = sendHandle.mFailed ? UA_STATUSCODE_BADUNEXPECTEDERROR : UA_STATUSCODE_GOOD;
          }
          thisHandler->mServerAccessMutex.lock();

          util::getExtEvHandler<COPC_UA_Local_Handler>(*localMethodHandle->getLayer().getCommFB())
              .removeMethodCall(localMethodCall);
        }
      }
    }
    return retVal;
  }

  void COPC_UA_Local_Handler::CUA_LocalCallbackFunctions::onWrite(UA_Server *,
                                                                  const UA_NodeId *,
                                                                  void *,
                                                                  const UA_NodeId *nodeId,
                                                                  void *nodeContext, // NOSONAR
                                                                  const UA_NumericRange *,
                                                                  const UA_DataValue *data) {

    UA_VariableContext_Handle *variableCallbackHandle = static_cast<UA_VariableContext_Handle *>(nodeContext);

    COPC_UA_Helper::UA_RecvVariable_handle handleRecv;

    handleRecv.mData.push_back(data->hasValue ? &data->value : nullptr); // TODO: check this empty data
    handleRecv.mOffset = variableCallbackHandle->mPortIndex;
    handleRecv.mNodeId = nodeId;

    EComResponse retVal = variableCallbackHandle->mActionInfo.getLayer().recvData(
        static_cast<const void *>(&handleRecv), 0); // TODO: add multidimensional mData handling with 'range'.

    if (e_Nothing != retVal) {
      variableCallbackHandle->mActionInfo.getLayer().getCommFB()->interruptCommFB(
          &variableCallbackHandle->mActionInfo.getLayer());
      util::getExtEvHandler<COPC_UA_Local_Handler>(*variableCallbackHandle->mActionInfo.getLayer().getCommFB())
          .startNewEventChain(variableCallbackHandle->mActionInfo.getLayer().getCommFB());
    }
  }

  bool COPC_UA_Local_Handler::initializeNodesets(UA_Server &paUaServer) {
    return OPC_UA_Nodesets::invoke(&paUaServer) == UA_STATUSCODE_GOOD;
  }

  void COPC_UA_Local_Handler::onAlarmStateChanged(const void *paData, unsigned int paSize, COPC_UA_Layer *paLayer) {
    if (paLayer->recvData(paData, paSize) != e_ProcessDataOk) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Receive Alarm Data failed for FB %s!\n", paLayer->getCommFB()->getInstanceName());
      return;
    }
    paLayer->getCommFB()->interruptCommFB(paLayer);
    startNewEventChain(paLayer->getCommFB());
  }
} // namespace forte::com_infra::opc_ua
