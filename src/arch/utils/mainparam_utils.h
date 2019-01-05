/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Tarik Terzimehic
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_UTILS_MAINPARAM_UTILS_H_
#define SRC_ARCH_UTILS_MAINPARAM_UTILS_H_

/*!\brief Lists the help for FORTE
 *
 */
void listHelp(void);

/*!\brief Parses the command line arguments passed to the main function
 *
 */
const char *parseCommandLineArguments(int argc, char *arg[]);

#ifdef FORTE_COM_OPC_UA
// Returns OPCUA server port number entered from the command line
TForteUInt16 getOpcuaServerPort(void);
#endif //FORTE_COM_OPC_UA

#endif /* SRC_ARCH_UTILS_MAINPARAM_UTILS_H_ */
