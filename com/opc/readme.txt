Installation Instructions
The OPC com layer requires the following packages
  - OPC Client library (https://github.com/kumajaya/OPC-Client-X64.git)
  - Boost Lexical Cast (http://www.boost.org)

Once the OPC Client library is installed, performe the steps below:
1. Choose OPC Client <installdir> for FORTE_COM_OPC_LIB_ROOT in CMake
2. Choose Boost root folder <boostroot> for FORTE_COM_OPC_BOOST_ROOT
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
