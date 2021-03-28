/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/

#ifndef _GEN_XOR_H_
#define _GEN_XOR_H_

#include "genbitbase.h"

class GEN_XOR : public CGenBitBase{
  DECLARE_GENERIC_FIRMWARE_FB(GEN_XOR)

  private:

    virtual void executeEvent(int paEIID);

    GEN_XOR(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);
    virtual ~GEN_XOR();

  public:
    template<typename T>
    void calculateValue(){
      T oIn;

      /*
       * Implementation of XOR for BOOL data type
       * ----------------------------------------
       * Classical interpretation of Exclusive OR:
       * The output value of the gate is 'true'
       * iff (if and only if) one of the boolean
       * inputs is set to 'true'.
       */
      if(CIEC_ANY::e_BOOL == st_OUT().getDataTypeID()){
        unsigned int nInputSet = 0;
        for(unsigned int nInputIndex = 0; nInputIndex < getFBInterfaceSpec()->m_nNumDIs; nInputIndex++){
          oIn.saveAssign(*static_cast<T*>(getDI(nInputIndex)));

          nInputSet = oIn ? nInputSet + 1 : nInputSet;
        }

        st_OUT().saveAssign(static_cast<T>(1 == nInputSet));
      }

      /*
       * Implementation of XOR for BYTE and WORD data types
       * --------------------------------------------------
       * The function is Interpreted as a cascade of
       * 2-Input XOR gates. This leads to a behaviour of an
       * odd-parity checker.
       */
      else{
        T oOut;

        for(unsigned int nInputIndex = 0; nInputIndex < getFBInterfaceSpec()->m_nNumDIs; nInputIndex++){
          oIn.saveAssign(*static_cast<T*>(getDI(nInputIndex)));

          if(0 == nInputIndex){
            st_OUT().saveAssign(oIn);
          }
          else{
            oOut.saveAssign(st_OUT());
            st_OUT().saveAssign(XOR(oOut, oIn));
          }
        }
      }
    }
};

#endif //_GEN_XOR_H_

