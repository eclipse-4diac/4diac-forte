#*******************************************************************************
# Copyright (c) 2017 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Jose Cabral - initial API and implementation and/or initial documentation
# *******************************************************************************/ 

Activate this module to access the I/O of the uMIC 200 of MicroControl
See: http://www.microcontrol.net/en/products/control-systems/umic200/


===================
ATTENTION: To use the relay and led, in some cases you might have to change some header files in your uMIC200. 
You need to find the files /usr/include/umic_led.h and /usr/include/umic_relay.h and add two things:
1- At the beginning, right after the first #define add:

//----------------------------------------------------------------------------//
// take precautions if compiled with C++ compiler                             //
#ifdef __cplusplus                                                            //
extern "C" {                                                                  //
#endif                                                                        //
//----------------------------------------------------------------------------//

2- And at the end, right before the last #endif, add

//----------------------------------------------------------------------------//
#ifdef __cplusplus                                                            //
}                                                                             //
#endif                                                                        //
// end of C++ compiler wrapper                                                //
//----------------------------------------------------------------------------//
 
 
If you don't want to use the relay and led, or you don't want to change the header files, go to ~/org.eclipse.4diac.org/src/modules/umic/processinterface.h and 
set the definitions of UMIC_LED_ENABLED and UMIC_RELAY_ENABLED to zero.
=================

Use the IX and QX Function Blocks. The parameters should be as follow

TYPE.NUMBER

NUMBER depends on TYPE, where TYPE can be:
  1 - led
  2 - dio
  3 - relay
  
NOTE: all are in lowercase.
"led" controls the LED1-4. The possible numbers for NUMBER are from 0 to 7 according to the table below:

   0 - LED1_GREEN
   1 - LED2_GREEN
   2 - LED3_GREEN
   3 - LED4_GREEN
   4 - LED1_RED
   5 - LED2_RED
   6 - LED3_RED
   7 - LED4_RED
   
NOTE: Only QX can be used with "led"

Example: 
  led.4       will control the LED1 color red
   
"dio" controls the 8 digital I/O at pins 5-12. Number can be from 1 to 8. 

NOTE: Use IX to make it input, and QX to make it output

Example:
  dio.3       will access Digital I/O 3 at pin 7 of the uMIC200
  
"relay" controls the relays. The possible numbers are 1 and 2. The 1 controls the NO relay and 2 the NC. 

NOTE: Only QX can be used with "relay"

Example: 
  relay.1       will control the NC relay at pins 14- 15

