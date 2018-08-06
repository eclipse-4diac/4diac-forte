Installation Instructions
The Modbus com interface need the libmodbus (http://libmodbus.org/) library to work.
Follow the steps below to set up your environment before compiling forte
1. libmodbus files should be placed in the following folder structure:
  <libmodbushome>/include/modbus: include files
  <libmodbushome>/lib: libmodbus.lib (windows) or libmodbus.so (posix)
2. When configuring forte project with CMake choose <libmodbushome> as FORTE_COM_MODBUS_LIB_DIR
3. forte.exe must be able to find the dynamic libmodbus library after build
  - copy libmodbus.dll/so to folder with forte.exe or
  - add libmodbus.dll/so to library include path

Parameter Documentation
Modbus Client (TCP)
At the moment the Modbus client can only be used for reading values from a Modbus server.
modbus[(protocol:)ip:port:pollFreqency:functionCode:(slaveId):readAddresses:sendAddresses(:responseTimeout:byteTimeout)]
  - protocol: tcp (tcp is default)
  - ip: 127.0.0.1 etc
  - port: default is 502
  - pollFrequency: polling frequency in milliseconds
  - functionCode: 1 - Read Coils
          2 - Read Discrete Inputs
          3 - Read Holding Registers
          4 - Read Input Register
  - slaveId (optional): the slave id used by the modbus server (0xFF is standard)
  - readAddresses: addresses can be specified between 0-65535
           more than one address (max 100) can be specified using
            + comma for separate addresses 0,2,65500
            + dots for interval 5..10
            + combination 0,5..10,2,65500
  - sendAddresses: addresses can be specified between 0-65535
           if data is only read sendAddresses should be left empty
           more than one address (max 100) can be specified using
            + comma for separate addresses 0,2,65500
            + dots for interval 5..10
            + combination 0,5..10,2,65500
  - responseTimeout (optional): timeout in milliseconds to wait for a response (500ms is default)
  - byteTimeout (optional): timeout in milliseconds between two consecutive bytes (500ms is default)

example: modbus[127.0.0.1:502:1000:3:1:0..3:]
