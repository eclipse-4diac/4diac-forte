# FORTE HTTP Com Layer
Simple HTTP Com Layer for 4diac-RTE (FORTE).

# Configuration
* Add the folder containing the source files to the FORTE modules directory (src/modules/)
* In CMake GUI, enable FORTE_COM_HTTP

# Documentation

All RDs and SDs data output/inputs should be connected to STRING types for it to work

## Client 

### Type of FB: 
- Needs a client FB. 
- For a GET request, it should have at exactly 1 RD (where the output is stored). SDs are ignored
- For a PUT/POST request you can set the data in two ways: 1- On the PARAMETERS in the PARAM data input (see below), or in a RD (exactly 1) of the client FB. If both cases are present, the RD takes precedence and the PARAMETER is ignored.

### PARAM:

http[IP:PORT/PATH?PARAMETERS;POST|PUT|GET;content-type;response-code]

### Mandatory:

IP:PORT ->  ip address and port of the remote endpoint
PATH -> path on the server to reach
POST | PUT | GET : They should be written exactly like this, and they define the type of packet to be sent.

### Optional:

PARAMETERS: parameters to be sent
content-type: HTTP content-type of the packet to be sent. If parameters are used with PUT or POST (SD or PARAMETERS), content-type will be ignored and application/x-www-form-urlencoded will be used instead. If  not defined, " text/html" will be used
response-code: the response that's expected. If not defined, " HTTP/1.1 200 OK " will be used. If the response-code of a packet is not as defined, the packet is ignored. If "*" is set, the packet is never ignored, no matter what the response code is


http[34.231.219.193:80/get;GET;*] --> Accepts all responses
http[34.231.219.193:80/get?par1=value1;GET;*] --> Request with parameters
http[34.231.219.193:80/put;PUT;application/json] --> Use application/json content type 
http[34.231.219.193:80/put;PUT;application/json;HTTP/1.1 201 Created] --> Use application/json content type and discard packets wich have not "HTTP/1.1 201 Created" as response code 

## Server

### Type of FB:
- Needs a server FB
- It must have exactly one SD (for the response text)
- Can have 0 or many RDs for the incoming parameters

### PARAM:

http[PATH]

The server FB will trigger an event when the PATH is accessed with GET request. If parameters are provided, they should match the number of RDs in the FB and should be of type (name=value&....). The "&" should be used to define more than one parameter. The name is actually not used. The values are stored in the RDs in the order they arrived and treated as STRINGs.

* Currently, only HTTP GET, PUT and POST requests are supported for clients. For servers, only GET.
* For a GET request, the FB should have exactly 1 RD (where the output is stored). SDs are ignored
* For a PUT/POST request you can set the data in two ways: 1- On the PARAMETERS in the PARAM data input (see below), or in a RD (exactly 1) of the client FB. If both cases are present, the RD takes precedence and the PARAMETER is ignored.
* Publish and Subscribe blocks are currently not supported and may result in unexpected behaviour.

# Notes
* The following IEC 61499 data types are currently NOT supported for PUT requests: DATE, DATE_AND_TIME, TIME_OF_DAY, arrays, structs
* By default, a GET request is sent as: <br />
  GET /path HTTP/1.1 <br />
  Host: ip:port" <br />
  <br />
* By default, a PUT{POST} request is sent as: <br />
  PUT{POST} /path HTTP/1.1 <br />
  Host: ip:port <br />
  Content-type: text/html <br />
  Content-length: length_of_data_string <br />
  <br />
  <br />
  data_string
 * See https://www.eclipse.org/forums/index.php/t/1088858/ for a discussion of how the layer was implemented
