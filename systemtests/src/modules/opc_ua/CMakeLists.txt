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

forte_add_2dev_systemtests(WorkingClientServer clientWorking.fboot serverWorking.fboot "" "" 30 1)

forte_add_2dev_systemtests(WorkingClientNoSubsServer clientWorkingNoSubs.fboot serverWorking.fboot "" "" 30 1)

forte_add_2dev_systemtests(FailingClientServer failingClient.fboot failingServer.fboot "" "" 35 0)

forte_add_2dev_systemtests(DelayedWorking delayedClient.fboot delayedServer.fboot "" "" 85 0)

forte_add_2dev_systemtests(TypesWorking typesClient.fboot typesServer.fboot "" "" 10 1)

forte_add_2dev_systemtests(FailActions clientFailAction.fboot localFailAction.fboot "" "" 40 0)

forte_opcua_add_nodeset_without_generated(fordiacNamespace ${CMAKE_CURRENT_SOURCE_DIR}/nodespaceTest)

