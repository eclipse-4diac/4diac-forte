#*******************************************************************************
# * Copyright (c) 2019 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Jose Cabral  - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_add_systemtest_soft(OPCUA_WrongParams wrongParameters.fboot 10)

SET_TESTS_PROPERTIES(OPCUA_WrongParams PROPERTIES FAIL_REGULAR_EXPRESSION "TEST_CONDITION_FAILED|==ERROR|OPC UA LOCAL") #Local server shouldn't start
 
forte_add_systemtest_hard(OPCUA_LocalFirstRead localFirstRead.fboot 10)

SET_TESTS_PROPERTIES(OPCUA_LocalFirstRead PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR|OPC UA REMOTE") #remote handler shouldn't start

forte_add_systemtest_hard(OPCUA_LocalFirstWrite localFirstWrite.fboot 10)

SET_TESTS_PROPERTIES(OPCUA_LocalFirstRead PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR|OPC UA REMOTE") #remote handler shouldn't start

forte_add_systemtest_hard(OPCUA_LocalMethdodsCreate localMethodsCreate.fboot 10)

SET_TESTS_PROPERTIES(OPCUA_LocalFirstRead PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR|OPC UA REMOTE") #remote handler shouldn't start

forte_add_multi_systemtests(OPCUA_WorkingClientServer 60 0 clientWorking.fboot " " serverWorking.fboot " ") #This could be hard, but connections errors sometimes occur which don't affect the TEST_CONDITIONS

forte_add_multi_systemtests(OPCUA_WorkingClientNoSubsServer 60 0 clientWorkingNoSubs.fboot " " serverWorking.fboot " ") #This could be hard, but connections errors sometimes occur which don't affect the TEST_CONDITIONS

forte_add_multi_systemtests(OPCUA_FailingClientServer 60 0 failingClient.fboot " " failingServer.fboot " ")

forte_add_multi_systemtests(OPCUA_DelayedWorking 120 0 delayedClient.fboot " " delayedServer.fboot " ")

forte_add_multi_systemtests(OPCUA_TypesWorking 60 0 typesClient.fboot " " typesServer.fboot " ") #This could be hard, but connections errors sometimes occur which don't affect the TEST_CONDITIONS

forte_add_multi_systemtests(OPCUA_FailActions 60 0 clientFailAction.fboot " " localFailAction.fboot " ")


#Config files for clients

forte_add_multi_systemtests(OPCUA_ClientConfigFailNonExistingFile 30 0 clientAuthFail.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/NonExistingFile.forte" serverForAuth.fboot " " )

forte_add_multi_systemtests(OPCUA_ClientConfigFailNoEqualSignal1 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoEqualSignal1.forte" serverForAuth.fboot " ")

forte_add_multi_systemtests(OPCUA_ClientConfigFailNoEqualSignal2 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoEqualSignal2.forte" serverForAuth.fboot " ")

forte_add_multi_systemtests(OPCUA_ClientConfigFailTwoEqualSignal1 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal1.forte" serverForAuth.fboot " ")

forte_add_multi_systemtests(OPCUA_ClientConfigFailTwoEqualSignal2 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal2.forte" serverForAuth.fboot " ")

forte_add_multi_systemtests(OPCUA_ClientConfigFailTwoEqualSignal3 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal3.forte" serverForAuth.fboot " ")

# deactivated as not correctly working with open62541 v1.3.4
# forte_add_multi_systemtests(OPCUA_ClientConfigSuccessNoEndpoint 30 0 clientAuthSuccess.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/successNoEndpoint.forte" serverForAuth.fboot " ")

# deactivated as not correctly working with open62541 v1.3.4
# forte_add_multi_systemtests(OPCUA_ClientConfigSuccessNonMatchEndpointFirst 30 0 clientAuthSuccess.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/successNonMatchEndpointFirst.forte" serverForAuth.fboot " ")

if(FORTE_COM_OPC_UA_ENCRYPTION AND FORTE_COM_OPC_UA_MASTER_BRANCH)
  forte_add_multi_systemtests(OPCUA_ClientConfigFailFailNoCertificateFile clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoCertificateFile.forte" serverForAuth.fboot " ")
endif(FORTE_COM_OPC_UA_ENCRYPTION AND FORTE_COM_OPC_UA_MASTER_BRANCH)

# deactivated as not currently working with FB layout refactor
# forte_add_multi_systemtests(OPCUA_DoubleResourceDeadlock 30 0 doubleResourceDeadlockClient.fboot " " doubleResourceDeadlockServer.fboot " ") # There's no test in these systems. It only tests that if two resources try to start the local/remote thread at the same time, there won't be a deadlock, thus finishing the test without a timeout

forte_add_multi_systemtests(OPCUA_FastWrite 200 0 fastWriteClient.fboot " " fastWriteServer.fboot " ") # There's no test in these systems. It only tests quick accessing the server with a client subscribed to it, and there won't be any problem, thus finishing the test without a timeout

forte_add_subdirectory(namespace)
