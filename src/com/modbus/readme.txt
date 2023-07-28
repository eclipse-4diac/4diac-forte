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
modbus[(protocol:)ip:port:(slaveId):pollFreqency:readAddresses:sendAddresses(:responseTimeout:byteTimeout)]
  - protocol: tcp (tcp is default)
  - ip: 127.0.0.1 etc
  - port: default is 502
  - to reuse a previous connection define only ip and port and leave everything up to slaveId empty
  - slaveId (optional): the slave id used by the modbus server (0xFF is standard)
  - pollFrequency: polling frequency in milliseconds
  - readAddresses: addresses can be specified between 0-65535
           more than one address (max 100) can be specified using
            + comma for separate addresses 0,2,65500
            + dots for interval 5..10
            + combination 0,5..10,2,65500
            function is by default Holding register, and can be changed with a prefixed letter
            + 'c' for coil
            + 'd' for discrete input
            + 'h' for holding register
            + 'i' for input register
  - sendAddresses: addresses can be specified between 0-65535
           if data is only read sendAddresses should be left empty
           more than one address (max 100) can be specified using
            + comma for separate addresses 0,2,65500
            + dots for interval 5..10
            + combination 0,5..10,2,65500
            function can be selected like for readAddresses
  - responseTimeout (optional): timeout in milliseconds to wait for a response (500ms is default)
  - byteTimeout (optional): timeout in milliseconds between two consecutive bytes (500ms is default)

example: modbus[127.0.0.1:502:1000:3:1:0..3:]

Modbus Client (RTU)
modbus[rtu:port:baudrate:parity:databits:stopbits:flow:(slaveId):pollFreqency:readAddresses:sendAddresses(:responseTimeout:byteTimeout)]
  - port: serial port (i.e. /dev/ttyUSB0 or COM1)
  - baudrate: serial port baudrate (i.e. 9600)
  - parity: N (none), E (even), or O (odd)
  - databits: 8
  - stopbits: 1
  - flow:
           + leave empty for none
           + arduino - disable DTR and wait for Arduino to boot just in case
           + delay - wait 2 seconds after connecting
           + longdelay - wait 3 seconds after connecting
  - to reuse a previous connection define only port and leave everything up to slaveId empty
  - all other paramters are as for TCP
