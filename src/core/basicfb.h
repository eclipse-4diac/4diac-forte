/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
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
 *    Martin Jobst - account for data type size in FB initialization
 *******************************************************************************/
#ifndef _BASICFB_H_
#define _BASICFB_H_

#include "funcbloc.h"

/*!\ingroup CORE
 * \brief structure to hold the data needed for creating the internal vars
 *
 */
struct SInternalVarsInformation {
    TPortId mNumIntVars; //!< Number of internal vars
    const CStringDictionary::TStringId *mIntVarsNames; //!< List of the internalvarsnames
    const CStringDictionary::TStringId *mIntVarsDataTypeNames; //!< List of the data type names for the internal vars
};

/*!\ingroup CORE
 *
 * \brief Class for handling firmware basic function blocks.
 */
class CBasicFB : public CFunctionBlock {
  public:
    /*!\brief The main constructur for a basic function block.
     */
    CBasicFB(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec *paInterfaceSpec,
             CStringDictionary::TStringId paInstanceNameId,
             const SInternalVarsInformation *paVarInternals);

    bool initialize() override;

    ~CBasicFB() override;

    CIEC_ANY* getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize) override;

    int toString(char *paValue, size_t paBufferSize) const override;

    size_t getToStringBufferSize() const override;

#ifdef FORTE_TRACE_CTF
    void traceInstanceData() override;
#endif

  protected:
    /*! \brief Get the internal variable with given number
     *
     * Attention this function will not perform any range checks on the paVarIntNum parameter!
     * @param paVarIntNum number of the internal variable starting with 0
     * @return pointer to the internal variable
     */
    virtual CIEC_ANY* getVarInternal(size_t paVarIntNum) {
      return mInternals[paVarIntNum];
    }

    const CIEC_ANY* getVarInternal(size_t paVarIntNum) const {
      return const_cast<CBasicFB *>(this)->getVarInternal(paVarIntNum);
    }

    CIEC_STATE mECCState; //! the current state of the ecc. start value is 0 = initial state id
    const SInternalVarsInformation *const cmVarInternals; //!< struct holding the information on the internal vars.

    static size_t calculateBasicFBVarsDataSize(const SInternalVarsInformation &paVarInternals);

    void *mBasicFBVarsData;
  private:
    /*!\brief Get the pointer to a internal variable of the basic FB.
     *
     * \param paInternalName StringId of the internal variable name.
     * \return Pointer to the internal variable or 0.
     */
    CIEC_ANY* getInternalVar(CStringDictionary::TStringId paInternalName);

    void setInitialValues() override;

    CIEC_ANY **mInternals; //!< A list of pointers to the internal variables.

#ifdef FORTE_FMU
        friend class fmuInstance;
#endif //FORTE_FMU
};

#endif /*_BASICFB_H_*/
