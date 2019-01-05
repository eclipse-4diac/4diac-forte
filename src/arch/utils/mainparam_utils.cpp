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

#ifdef FORTE_COM_OPC_UA
// Static variable and function used to pass OPCUA server port number to the OPCUA module
static TForteUInt16 s_nOpcuaServerPort = FORTE_COM_OPC_UA_PORT;
TForteUInt16 getOpcuaServerPort(void){
  return s_nOpcuaServerPort;
}
#endif //FORTE_COM_OPC_UA

/*!\brief Lists the help for FORTE
 *
 */
void listHelp(void){
  printf("\nUsage of FORTE:\n");
  printf("   -h\t lists this help.\n");
  printf("\n");
  printf("   -c\t sets the destination for the connection.\n");
  printf("     \t Usage: forte -c <IP>:<Port>");
  printf("\n");
#ifdef FORTE_COM_OPC_UA
  printf("   -o\t sets the port for the OPC UA connection.\n");
  printf("     \t Usage: forte -o <Port>");
  printf("\n");
#endif //FORTE_COM_OPC_UA
}

/*!\brief Parses the command line arguments passed to the main function
 *
 */
const char *parseCommandLineArguments(int argc, char *arg[]){

  const char *pIpPort = "localhost:61499"; //! Default Value (localhost:61499)

  if(argc > 1){
    for(size_t i = 1; i < argc; i += 2){

      if(arg[i][0] == '-'){
        switch(arg[i][1]){
          case 'c': //! sets the destination for the connection
            pIpPort = arg[i + 1];
            break;
#ifdef FORTE_COM_OPC_UA
          case 'o': //! Retrieves OPCUA server port number entered from the command line
              s_nOpcuaServerPort = (TForteUInt16)atoi(arg[i+1]);
            break;
#endif //FORTE_COM_OPC_UA
          case 'h': //! Lists the help for FORTE
            pIpPort = "";
            return pIpPort;
         default: //! Unknown parameter -> Lists the help for FORTE
           pIpPort = "";
           return pIpPort;
        }
      }
      else{ //! Unknown parameter -> Lists the help for FORTE
        pIpPort = "";
      }
    }
  }

  return pIpPort;
}
