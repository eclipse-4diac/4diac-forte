/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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


#endif /* SRC_ARCH_UTILS_MAINPARAM_UTILS_H_ */
