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

#include <forte_config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*!\brief Lists the help for FORTE
 *
 */
void listHelp(void){
  printf("Usage: forte [options]\n");
  printf("Options:\n");
  printf("%-20s Display this information\n", "  -h");
  printf("%-20s Set the listening IP and port for the incoming connections\n", "  -c <IP>:<port>");
#ifdef FORTE_SUPPORT_BOOT_FILE
  printf("%-20s Set the boot-file where to read from to load the applications\n", "  -f <file>");
#endif
#ifdef FORTE_COM_OPC_UA
  printf("%-20s Set the listening port for the OPC UA connection\n", "  -o <Port>");
#endif //FORTE_COM_OPC_UA
}

/*!\brief Parses the command line arguments passed to the main function
 *
 */
const char *parseCommandLineArguments(int argc, char *arg[]){

  const char *pIpPort = "localhost:61499"; //! Default Value (localhost:61499)

  if(argc > 1) {
    for(size_t i = 1; i < static_cast<size_t>(argc); i += 2) {
      if('-' == arg[i][0]) {
        switch(arg[i][1]){
          case 'c': //! sets the destination for the connection
            pIpPort = arg[i + 1];
            break;
#ifdef FORTE_SUPPORT_BOOT_FILE
          case 'f': //! sets the boot-file to be used
            gCommandLineBootFile = arg[i + 1];
            break;
#endif //FORTE_SUPPORT_BOOT_FILE
#ifdef FORTE_COM_OPC_UA
            case 'o': //! Retrieves OPCUA server port number entered from the command line
            gOpcuaServerPort = static_cast<TForteUInt16>(atoi(arg[i + 1]));
            break;
#endif //FORTE_COM_OPC_UA
          default: //! Unknown parameter or -h -> Lists the help for FORTE
            return "";
        }
      } else { //! Unknown parameter -> Lists the help for FORTE
        return "";
      }
    }
  }

  return pIpPort;
}
