/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Gunnar Grabmair, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _BASICFB_H_
#define _BASICFB_H_

#include "funcbloc.h"

#ifndef FORTE_BASIC_FB_DATA_ARRAY  //with this check we can overwrite this define in a platform specific file (e.g., config.h)

/*! Define that adds the data array to a Basic FB
 * May be overwritten by a platform specific version that adapts for example some alignment requirements
 */
#define FORTE_BASIC_FB_DATA_ARRAY(a_nNumEOs, a_nNumDIs, a_nNumDOs, a_nNumIntVars, a_nNumAdapters) \
  union{ \
    TForteByte m_anFBConnData[genFBConnDataSizeTemplate<a_nNumEOs, a_nNumDIs, a_nNumDOs>::value]; \
  };\
  union{ \
    TForteByte m_anFBVarsData[genBasicFBVarsDataSizeTemplate<a_nNumDIs, a_nNumDOs, a_nNumIntVars, a_nNumAdapters>::value]; \
  };

#endif

/*!\ingroup CORE
 * \brief structure to hold the data needed for creating the internal vars
 *
 */
struct SInternalVarsInformation {
    TPortId m_nNumIntVars; //!< Number of internal vars
    const CStringDictionary::TStringId *m_aunIntVarsNames; //!< List of the internalvarsnames
    const CStringDictionary::TStringId *m_aunIntVarsDataTypeNames; //!< List of the data type names for the internal vars
};

/*!\ingroup CORE
 *
 * \brief Class for handling firmware basic function blocks.
 */
class CBasicFB : public CFunctionBlock {
  public:
    /*!\brief The main constructur for a basic function block.
     */

    CBasicFB(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId,
        const SInternalVarsInformation *pa_pstVarInternals, TForteByte *pa_acFBConnData, TForteByte *pa_acBasicFBVarsData);

    CBasicFB(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId,
        const SInternalVarsInformation *pa_pstVarInternals, TForteByte *pa_acFBConnData, TForteByte *pa_acBasicFBVarsData,
        const SCFB_FBInstanceData *const pa_astInternalFBs, const size_t pa_numberOfInternalFbs);

    virtual ~CBasicFB();

    virtual CIEC_ANY* getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize);

    virtual EMGMResponse changeFBExecutionState(EMGMCommandType pa_unCommand);

    template<unsigned int ta_nNumDIs, unsigned int ta_nNumDOs, unsigned int ta_nNumIntVars, unsigned int ta_nNumAdapters = 0>
    struct genBasicFBVarsDataSizeTemplate {
        enum {
          value = ((sizeof(TDataConnectionPtr) + sizeof(CIEC_ANY)) * ta_nNumIntVars + genFBVarsDataSizeTemplate<ta_nNumDIs, ta_nNumDOs, ta_nNumAdapters>::value)
        };
    };

    static size_t genBasicFBVarsDataSize(unsigned int pa_nNumDIs, unsigned int pa_nNumDOs, unsigned int pa_nNumIntVars, unsigned int pa_nNumAdapters = 0) {
      return ((sizeof(TDataConnectionPtr) + sizeof(CIEC_ANY)) * pa_nNumIntVars + genFBVarsDataSize(pa_nNumDIs, pa_nNumDOs, pa_nNumAdapters));
    }
    ;

  protected:
    /*! \brief Get the internal variable with given number
     *
     * Attention this function will not perform any range checks on the pa_nVarIntNum parameter!
     * @param pa_nVarIntNum number of the internal variable starting with 0
     * @return pointer to the internal variable
     */
    CIEC_ANY* getVarInternal(unsigned int pa_nVarIntNum) {
      return m_aoInternals + pa_nVarIntNum;
    }

    CIEC_UINT m_nECCState; //! the current state of the ecc. start value is 0 = initial state id
    const SInternalVarsInformation *const cm_pstVarInternals; //!< struct holding the information on the internal vars.
    size_t cm_amountOfInternalFBs;

  private:
    /*!\brief Get the pointer to a internal variable of the basic FB.
     *
     * \param pa_nInternalName StringId of the internal variable name.
     * \return Pointer to the internal variable or 0.
     */
    CIEC_ANY* getInternalVar(CStringDictionary::TStringId pa_nInternalName);

    CIEC_ANY *m_aoInternals; //!< A list of pointers to the internal variables.

    void createInternalFBs(const SCFB_FBInstanceData *const pa_InternalFBData);

#ifdef FORTE_FMU
    friend class fmuInstance;
#endif //FORTE_FMU
};

#endif /*_BASICFB_H_*/
