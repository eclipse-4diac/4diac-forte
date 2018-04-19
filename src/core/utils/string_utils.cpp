/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "string_utils.h"
#include <forte_dint.h>
#include <forte_udint.h>
#include <forte_lint.h>
#include <forte_ulint.h>

#include <errno.h>
#include <string.h>

bool forte::core::util::isAtoFChar(char pa_cValue){
  pa_cValue = static_cast<char>(toupper(pa_cValue));
  return ((pa_cValue >= 'A') && (pa_cValue <= 'F'));
}

TForteInt8 forte::core::util::charAtoFToInt(char pa_cValue){
  return static_cast<TForteInt8>((toupper(pa_cValue) - 'A') + 10);
}

long int forte::core::util::strtol(const char *nptr, char **endptr, int base) {
  long int nRetVal = 0;
  bool bNegativeNumber = false;
  errno = 0;

  //in difference to IEC 611313-3 literals base 16 literals may have a leading -
  if((*nptr) == '-'){
    bNegativeNumber = true;
    ++nptr;
  }

  if(16 == base){
	  if(('0' == (*nptr)) && ('x' == nptr[1])){
	    //we have a preceding 0x step over it
	    nptr += 2;
	  }
  }

  long nLimit1 = (bNegativeNumber ? -(CIEC_DINT::scm_nMinVal / base) : (CIEC_DINT::scm_nMaxVal / base));
  long nLimit2 = (bNegativeNumber ? -(CIEC_DINT::scm_nMinVal % base) : (CIEC_DINT::scm_nMaxVal % base));
  if (nLimit2 < 0) // Rounding direction for negative numbers is implementation defined
  {
    nLimit1--;
    nLimit2 = base-nLimit2;
  }

  for(; *nptr; ++nptr){ //Do until '/0'
    if((!isDigit(*nptr)) && (!((16 == base) && isAtoFChar(*nptr)))){
      //we have an invalid character
      break;
    }
    TForteInt8 nCharVal = charHexDigitToInt(*nptr);
    if(nRetVal > nLimit1 || (nRetVal == nLimit1 && nCharVal > nLimit2)) {
      nRetVal = (bNegativeNumber) ? CIEC_DINT::scm_nMinVal : CIEC_DINT::scm_nMaxVal;
      errno = ERANGE;
      break;
    }
    nRetVal = base * nRetVal + nCharVal;
  }

  if((bNegativeNumber) && (ERANGE != errno)){
    nRetVal = -nRetVal;
  }

  if(0 != endptr){
    *endptr = const_cast<char*>(nptr);
  }
  return nRetVal;
}

unsigned long int forte::core::util::strtoul(const char *nptr, char **endptr, int base){
  unsigned long int unRetVal = 0;
  unsigned long int unLimit1 = (CIEC_UDINT::scm_nMaxVal / base);
  unsigned long int unLimit2 = (CIEC_UDINT::scm_nMaxVal % base);
  errno = 0;

  if((16 == base) && ('0' == (*nptr)) && ('x' == nptr[1])){
    //we have a preceding 0x step over it
    nptr += 2;
  }

  for(; *nptr; ++nptr){ //Do until '/0'
    if((!isDigit(*nptr)) && (!((16 == base) && isAtoFChar(*nptr)))){
      //we have an invalid character
      break;
    }
    TForteInt8 nCharVal = charHexDigitToInt(*nptr);

    if(unRetVal > unLimit1 || (unRetVal == unLimit1 && (unsigned long int) nCharVal > unLimit2)){
      //in this round we would exceed the limit of the data type
      unRetVal = CIEC_UDINT::scm_nMaxVal;
      errno = ERANGE;
      break;
    }
    unRetVal = base * unRetVal + nCharVal;
  }
  if(0 != endptr){
    *endptr = const_cast<char*>(nptr);
  }
  return unRetVal;
}

#ifdef FORTE_USE_64BIT_DATATYPES
long long int forte::core::util::strtoll(const char *nptr, char **endptr, int base) {
  long long int nRetVal = 0;
  bool bNegativeNumber = false;
  errno = 0;

  //in difference to IEC 611313-3 literals base 16 literals may have a leading -
  if((*nptr) == '-'){
    bNegativeNumber = true;
    ++nptr;
  }

  if(16 == base){
    if(('0' == (*nptr)) && ('x' == nptr[1])){
      //we have a preceding 0x step over it
      nptr += 2;
    }
  }

// The volatile magic works around an apparent optimizer bug on Visual Studio 2008. Looks like this one:
// http://connect.microsoft.com/VisualStudio/feedback/details/267232/sign-error-in-long-integer-arithmetic
// "The Visual C++ team has evaluated the bug and determined it does not meet the guidelines necessary to warrant a fix."
  volatile long long nLimit1, nLimit2;

  if (bNegativeNumber){
    volatile long long nLimMinDiv = CIEC_LINT::scm_nMinVal / base;
    volatile long long nLimMinMod = CIEC_LINT::scm_nMinVal % base;

    nLimit1 = -nLimMinDiv;
    nLimit2 = -nLimMinMod;

    if (nLimit2 < 0) // Rounding direction for negative numbers is implementation defined
    {
      nLimit1--;
      nLimit2 = base-nLimit2;
    }
  }
  else{
    nLimit1 = CIEC_LINT::scm_nMaxVal / base;
    nLimit2 = CIEC_LINT::scm_nMaxVal % base;
  }

  for(; *nptr; ++nptr){ //Do until '/0'
    if((!isDigit(*nptr)) && (!((16 == base) && isAtoFChar(*nptr)))){
      //we have an invalid character
      break;
    }
    TForteInt8 nCharVal = charHexDigitToInt(*nptr);
    if(nRetVal > nLimit1 || (nRetVal == nLimit1 && nCharVal > nLimit2)){
      nRetVal = (bNegativeNumber) ? CIEC_LINT::scm_nMinVal : CIEC_LINT::scm_nMaxVal;
      errno = ERANGE;
      break;
    }
    nRetVal = base * nRetVal + nCharVal;
  }

  if((bNegativeNumber) && (ERANGE != errno)){
    nRetVal = -nRetVal;
  }
  if(0 != endptr){
    *endptr = const_cast<char*>(nptr);
  }
  return nRetVal;
}

unsigned long long int forte::core::util::strtoull(const char *nptr, char **endptr, int base){
  unsigned long long int unRetVal = 0;
  unsigned long long int unLimit1 = (CIEC_ULINT::scm_nMaxVal / base);
  unsigned long long int unLimit2 = (CIEC_ULINT::scm_nMaxVal % base);
  errno = 0;

  if((16 == base) && ('0' == (*nptr)) && ('x' == nptr[1])){
    //we have a preceding 0x step over it
    nptr += 2;
  }

  for(; *nptr; ++nptr){ //Do until '/0'
    if((!isDigit(*nptr)) && (!((16 == base) && isAtoFChar(*nptr)))){
      //we have an invalid character
      break;
    }
    TForteInt8 nCharVal = charHexDigitToInt(*nptr);
    if(unRetVal > unLimit1 || (unRetVal == unLimit1 && (unsigned long long int) nCharVal > unLimit2)){
      //in this round we would exceed the limit of the data type
      unRetVal = CIEC_ULINT::scm_nMaxVal;
      errno = ERANGE;
      break;
    }
    unRetVal = base * unRetVal + nCharVal;
  }
  if(0 != endptr){
    *endptr = const_cast<char*>(nptr);
  }
  return unRetVal;
}

#endif
