openPOWERLINK interface for FORTE

Instructions

Prerequisits
The following is needed in order to use openPOWERLINK with FORTE:

 - openPOWERLINK V1.8.0
 - TinyXML

To use openPOWERLINK download openPOWERLINK V1.8.0 from Sourceforge (http://sourceforge.net/projects/openpowerlink/). The stack should be compiled using at least the following options in CMake:

 - CFG_POWERLINK_CHM
 - CFG_POWERLINK_PROCESS_IMAGE
 - CFG_X86_WINDOWS_DLL (only available on Windows)

If compiled on Windows the resulting DLL and LIB must be copied to the main openPOWERLINK-V1.8.0 folder.

TinyXML can be downloaded from http://sourceforge.net/projects/tinyxml/.

Configure FORTE
Check FORTE_MODULE_POWERLINK and set FORTE_MODULE_POWERLINK_LIB_DIR to [OPENPOWERLINK-V1.08.0_HOME] and FORTE_MODULE_POWERLINK_TINYXML_DIR to the directory where you unpacked TinyXML.
