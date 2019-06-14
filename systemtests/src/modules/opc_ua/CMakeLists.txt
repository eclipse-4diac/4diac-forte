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
 
forte_add_systemtest_hard(OPCUA_LocalFirstRead localFirstRead.fboot 5)

forte_add_systemtest_hard(OPCUA_LocalFirstWrite localFirstWrite.fboot 5)

forte_add_systemtest_hard(OPCUA_LocalMethdodsCreate localMethodsCreate.fboot 5)

forte_add_2dev_systemtests(WorkingClientServer clientWorking.fboot serverWorking.fboot "" "" 30 1)

forte_add_2dev_systemtests(WorkingClientNoSubsServer clientWorkingNoSubs.fboot serverWorking.fboot "" "" 30 1)

forte_add_2dev_systemtests(FailingClientServer failingClient.fboot failingServer.fboot "" "" 25 0)

forte_add_2dev_systemtests(DelayedWorking delayedClient.fboot delayedServer.fboot "" "" 85 0)

forte_add_2dev_systemtests(TypesWorking typesClient.fboot typesServer.fboot "" "" 10 1)