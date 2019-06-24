# OPC UA Improvements

## Bugs ready to be closed

* Reconnection of Client when connection is lost: https://bugs.eclipse.org/bugs/show_bug.cgi?id=532342
* SUBSCRIBE blocks can't subscribe to variables on other OPC-UA servers: https://bugs.eclipse.org/bugs/show_bug.cgi?id=532344
* Look up node in remote server by nodeid and not only browsename: https://bugs.eclipse.org/bugs/show_bug.cgi?id=537651
* The server should not be started if only a Client FB is deployed: https://bugs.eclipse.org/bugs/show_bug.cgi?id=520459
* Set server capabilities in OPCUA: https://bugs.eclipse.org/bugs/show_bug.cgi?id=542609
* Add check for string termination and fix one-line if in opcuaHandler: https://bugs.eclipse.org/bugs/show_bug.cgi?id=542618

## New features

* Each layer is not a thread anymore. 
* Ability to set own NodeIds to nodes (numeric, strings, and bytstring. GUID not implemented)
* Ability to set the desired browse name to nodes. No more automatic generation from the connected FB
* Nodesets can easily be integrated to forte

## Yet to be done

* Ability to create objects
* Create forte namespace and limit local server to it
* Ability to reference a namespace locally/remotely using the uri and not only the number
* Add most common opc ua types in forte (LocalizedText, QualifiedName)
* Add support for arrays
* Replace CSingledLinkedList with vectors
* Replace CIEC_STRING and char* with std::string
* Return references instead of poitners where possible
* Split initializeReadWrite and initializeCreateMethod in smaller functions
* Use std::algorithms where possible (getNode has a iteration at the end where it could fit)
* Store parent and nodeId for all methods. Methods from the same objectType don't work otherwise

## For users

This section explain how to use the OPC UA layer in this branch:

### Parameters

The parameters for the communication FBs (ID data input) is divided in two (three for remote actions) parts, separated by a semicolon ';'

opc_ua[ACTION;ENDPOINT;PAIR1;PAIR2;...]

ACTION: Mandatory field and can have the following values (all in upper case):
* READ
* WRITE
* CREATE_METHOD
* CALL_METHOD
* SUBSCRIBE
* CREATE_OBJECT
* DELETE_OBJECT 

EDNPOINT: Mandatory and restricted part when using for remote actions. It must be end with a '#' chraracter.
* Example: opc.tcp://192.168.0.100:4840#

PAIR: In the format BROWSENAME,NODE\_ID (separated by a comma ','). When possible, NODE\_ID can be omitted, in which case the comma ',' must also be omitted. If the BROWSENAME is to be omited (when possible) the ',' must be present.

* BROWSENAME: Browsepath to the noded and must always start with "/Objects". A colon before the name allows to define the namespace of the browsename of the current element. Default namespace of browsename is 1
    
    * Example:  /Objects/myFolder/myNode
    * Example:  /Objects/2:myFolder/myNode -> In objects, it will look for a folder with name myFolder and namespace 2 and inside, the node with name myNode and namespace 1


* NODE\_ID: In the format  NAMESPACE\_INDEX:IDENTIFIER\_TYPE=IDENTIFIER

    * NAMESPACE\_INDEX: Number of the namepsace. If ignored, it defaults to 0. When ignored, the ':' must also be omitted
   
    * IDENTIFIER\_TYPE: Can have the following values:
        * i: The nodeId is numeric
        * s: the nodeId is a string
        * b: the nodeId is a bytestring
        * g: the nodeId is GUID (NOT IMPLEMENTED)
       
    * IDENTIFIER: identifier according to the IDENTIFIER\_TYPE
       
       * 1:i=12345 -> namespace 1, numeric identifier 12345
       * 3:s=hello -> namespace 3, string identifier "hello"
       * 2:b=hello2 -> namespace 2, bytestring identifier hello2
       * s=hello5 -> namespace 0,  string identifier "hello5"

* PAIR Examples:
    * /Objects/myFolder/myNode,1:i=12345 -> Both browsepath and nodeId are provided
    * /Objects/myFolder/myNode1 -> browsepath is provided, the nodeId is omitted
    * ,1:i=12345 -> browsepath is omitted, but nodeId is provided
    
 
ID Examples:
* opc\_ua[READ;/Objects/test1] -> read the local node /Objects/test1. Create the node if it doesn't exits with a random nodeId
* opc\_ua[READ;/Objects/test1,i=1] -> read the local node /Objects/test1. Create the node if it doesn't exits with a numeric node id 1 and namespace 0. If the node already exist and it has other nodeId, it will fail.
* opc\_ua[WRITE;/Objects/test1;/Objects/test1] -> Write nodes /Objects/test1 and /Objects/test1 from values in SD\_1 and SD\_2. Will create the nodes if they don't exist with random nodeIds. If they exist, it will write to them when triggered.
 * opc\_ua[WRITE;opc.tcp://192.168.0.100:4840#/Objects/test1;/Objects/test1] -> Write nodes /Objects/test1 and /Objects/test1 on remote from values in SD\_1 and SD\_2. If nodes aren't found, 4diac FORTE will look for them after some seconds until it finds them. Remote creation is not supported


* NOTE: depending on the action being performed, browsepath and/or nodeId are mandatory/optional. See the table below
 
 ### Function Blocks 
 
 All SDs and RDs must be connected to some known data type   
    
| Remote/Local  | Desired Action  | Function Block to use | Restrictions |
| ------------- |---------------| ---------------------|-------------|
| Local         | READ            | SUBSCRIBE             | Number of Pairs should match the number of RDs. NodeId is optional. If browsename is omitted, it will look for the node using the nodeId and won't create any (because it doesn't know where to create it)|
| Local         | WRITE           | PUBLISH               | Number of Pairs should match the number of SDs .NodeId is optional. If browsename is omitted, it will look for the node using the nodeId and won't create any (because it doesn't know where to create it)|
| Local         | CREATE_METHOD   | SERVER                | Number of Pairs should be 1. Browsepath MUST be provided. NodeId is optional |
| Local         | CALL_METHOD     | X                     | Not allowed to call local methods |
| Local         | SUBSCRIBE       | X                     | Not allowed to subscribe to local variables |
| Local         | CREATE_OBJECT   | X                     | NOT IMPLEMENTED YET |
| Local         | DELETE_OBJECT   | X                     | NOT IMPLEMENTED YET |
| Remote        | READ            | CLIENT                | Number of Pairs should match the number of RDs and no SDs must be present. Browsename and/or NodeId must be provided. If both are provided they should match |
| Remote        | WRITE           | CLIENT                | Number of Pairs should match the number of SDs and no RDs must be present. Browsename and/or NodeId must be provided. If both are provided they should match |
| Remote        | CREATE_METHOD   | CLIENT                | Not allowed to create methods remotely |
| Remote        | CALL_METHOD     | CLIENT                | Number of Pairs should be 1. Browsepath MUST be provided. NodeId is optional. If both are provided they should match |
| Remote        | SUBSCRIBE       | SUBSCRIBE             | Number of Pairs should match the number of RDs. Browsename and/or NodeId must be provided. If both are provided they should match  |
| Remote        | CREATE_OBJECT   | X                     | NOT IMPLEMENTED YET (probably won't be) |
| Remote        | DELETE_OBJECT   | X                     | NOT IMPLEMENTED YET (probably won't be) |


## For developers

The code was changed a lot. There are now two handlers, one for the local server and one for clients, and an abstract parent class for both which have common code. Most work is shifted to these handlers which offer three main functions: initialize, execute and uninitialize.  

The layer pass its parameter to the abstract parent, which parses and checks that the restrictions from the table before are met. It returns an ACTION class and from the action, it stores the local or remote (clients) handler to use it later. When sending, the execute function is performed (on the respective handler). 

The local handler is mostly the same as before.

The client handler is more complicated. All actions are executed asynchronously in its own thread. The client handler has another thread (ConnectionThread) which takes care of connecting to the remote and initializing the ACTIONS (layers). Why? because when the server is not yet present, ConnectionThread keeps trying to connect to it. So when the server starts, it tries to initialize all ACTIONS related to the client. When some ACTION is initialized it passes this information to the mainThead of the client handler which iterates over the asynchronous calls. 

A CI is set for the branch feature/MO44GA at https://hudson.eclipse.org/4diac/job/FORTE_MO44GA/. Tests are provided to handle most cases. The coverage can be seen there. The tests will be pushed to the repository org.eclipse.4diac.examples

