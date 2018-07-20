Installation Instructions
The OPC com layer requires the following packages
  - OPC Client library release 0.4 (http://sourceforge.net/projects/opcclient/)
  - Boost Lexical Cast (http://www.boost.org)

Before OPC Client is compiled the function init() in OPCClient.cpp must be changed from:
void COPCClient::init()
{  
  HRESULT  result = CoInitialize(NULL);
  if (FAILED(result))
  {
    throw OPCException("CoInitialize failed");
  }

  CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

  result = CoGetMalloc(MEMCTX_TASK, &iMalloc);
  if (FAILED(result))
  {
    throw OPCException("CoGetMalloc failed");
  }
}

to:
void COPCClient::init()
{  
  CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

  HRESULT  result = CoGetMalloc(MEMCTX_TASK, &iMalloc);
  if (FAILED(result))
  {
    throw OPCException("CoGetMalloc failed");
  }
}
  
  
Once the OPC Client library is compiled performe the steps below:
1. Place OPC Client library in the following folder structure:
  <opcclientroot>/include - all headers should be placed here
  <opcclientroot>/lib - OPCClientToolkit.lib
2. Choose <opcclientroot> for FORTE_COM_OPC_LIB_ROOT in CMake
3. Choose Boost root folder <boostroot> for FORTE_COM_OPC_BOOST_ROOT
   (the lexical_cast.hpp header must be available in <boostroot>/boost)

Parameter Documentation (all values are required)
OPC Client
opc[host:serverName:updateRate:deadBand:fbInputItems:fbOutputItems]
  - host: 127.0.0.1 etc
  - serverName: e.g. Matrikon.OPC.Simulation
  - updateRate: update frequency in milliseconds
  - deadBand: dead band used for update (same unit as value)
  - fbInputItems/fbOutputItems: items to be added, items should be separated with a comma i.e.
                  Random.Int2,Bucket Brigade.Int4

example: opc[127.0.0.1:Matrikon.OPC.Simulation:2000:0.01:Bucket Brigade.Int4:Random.Int2,Bucket Brigade.Int4]
