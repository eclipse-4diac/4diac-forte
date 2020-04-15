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

forte_add_systemtest_soft(OPCUA_WrongParams wrongParameters.fboot 5)

SET_TESTS_PROPERTIES(OPCUA_WrongParams PROPERTIES FAIL_REGULAR_EXPRESSION "TEST_CONDITION_FAILED|==ERROR|OPC UA LOCAL") #Local server shouldn't start
 
forte_add_systemtest_hard(OPCUA_LocalFirstRead localFirstRead.fboot 5)

SET_TESTS_PROPERTIES(OPCUA_LocalFirstRead PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR|OPC UA REMOTE") #remote handler shouldn't start

forte_add_systemtest_hard(OPCUA_LocalFirstWrite localFirstWrite.fboot 5)

SET_TESTS_PROPERTIES(OPCUA_LocalFirstRead PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR|OPC UA REMOTE") #remote handler shouldn't start

forte_add_systemtest_hard(OPCUA_LocalMethdodsCreate localMethodsCreate.fboot 5)

SET_TESTS_PROPERTIES(OPCUA_LocalFirstRead PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR|OPC UA REMOTE") #remote handler shouldn't start

forte_add_2dev_systemtests(OPCUA_WorkingClientServer clientWorking.fboot serverWorking.fboot "" "" 40 1)

forte_add_2dev_systemtests(OPCUA_WorkingClientNoSubsServer clientWorkingNoSubs.fboot serverWorking.fboot "" "" 40 1)

forte_add_2dev_systemtests(OPCUA_FailingClientServer failingClient.fboot failingServer.fboot "" "" 45 0)

forte_add_2dev_systemtests(OPCUA_DelayedWorking delayedClient.fboot delayedServer.fboot "" "" 85 0)

forte_add_2dev_systemtests(OPCUA_TypesWorking typesClient.fboot typesServer.fboot "" "" 20 1)

forte_add_2dev_systemtests(OPCUA_FailActions clientFailAction.fboot localFailAction.fboot "" "" 50 0)

forte_opcua_add_nodeset_without_generated(fordiacNamespace ${CMAKE_CURRENT_SOURCE_DIR}/nodespaceTest)

#Config files for clients

forte_add_2dev_systemtests(OPCUA_ClientConfigFailNonExistingFile clientAuthFail.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/NonExistingFile.forte" "" 10 0)

forte_add_2dev_systemtests(OPCUA_ClientConfigFailNoEqualSignal1 clientAuthFail.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoEqualSignal1.forte" "" 10 0)

forte_add_2dev_systemtests(OPCUA_ClientConfigFailNoEqualSignal2 clientAuthFail.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoEqualSignal2.forte" "" 10 0)

forte_add_2dev_systemtests(OPCUA_ClientConfigFailTwoEqualSignal1 clientAuthFail.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal1.forte" "" 10 0)

forte_add_2dev_systemtests(OPCUA_ClientConfigFailTwoEqualSignal2 clientAuthFail.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal2.forte" "" 10 0)

forte_add_2dev_systemtests(OPCUA_ClientConfigFailTwoEqualSignal3 clientAuthFail.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal3.forte" "" 10 0)

forte_add_2dev_systemtests(OPCUA_ClientConfigSuccessNoEndpoint clientAuthSuccess.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/successNoEndpoint.forte" "" 10 1)

forte_add_2dev_systemtests(OPCUA_ClientConfigSuccessNonMatchEndpointFirst clientAuthSuccess.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/successNonMatchEndpointFirst.forte" "" 10 1)

if(FORTE_COM_OPC_UA_ENCRYPTION AND FORTE_COM_OPC_UA_MASTER_BRANCH)
  forte_add_2dev_systemtests(OPCUA_ClientConfigFailFailNoCertificateFile clientAuthFail.fboot serverForAuth.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoCertificateFile.forte" "" 10 0)
endif(FORTE_COM_OPC_UA_ENCRYPTION AND FORTE_COM_OPC_UA_MASTER_BRANCH)


forte_add_2dev_systemtests(OPCUA_DoubleResourceDeadlock doubleResourceDeadlockClient.fboot doubleResourceDeadlockServer.fboot "" "" 20 0) # There's no test in these systems. It only tests that if two resources try to start the local/remote thread at the same time, there won't be a deadlock, thus finishing the test without a timeout
