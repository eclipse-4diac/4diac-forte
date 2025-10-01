/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Martin Erich Jobst, Johannes Kepler University Linz,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Gunnar Grabmair, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *                - initial implementation and rework communication infrastructure
 *   Martin Jobst - account for data type size in FB initialization
 *   Alois Zoitl  - exracted internal variable handling to new CBaseFB
 *                - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"

namespace forte {
  class CBaseFB : public CFunctionBlock {
    public:
      ~CBaseFB() override = default;

      CIEC_ANY *getVar(std::span<const StringId> paNameList) override;

      void toString(std::string &paTargetBuf) const override;

#ifdef FORTE_TRACE_CTF
      void traceInstanceData() override;
#endif

    protected:
      CBaseFB(CFBContainer &paContainer,
              const SFBInterfaceSpec &paInterfaceSpec,
              StringId paInstanceNameId,
              std::span<const StringId> paVarInternalNames);

      /*! \brief Get the internal variable with given number
       *
       * Attention this function will not perform any range checks on the paVarIntNum parameter!
       * @param paVarIntNum number of the internal variable starting with 0
       * @return pointer to the internal variable
       */
      virtual CIEC_ANY *getVarInternal(size_t paVarIntNum) = 0;

      const CIEC_ANY *getVarInternal(size_t paVarIntNum) const {
        return const_cast<CBaseFB *>(this)->getVarInternal(paVarIntNum);
      }

      void setInitialValues() override = 0;

    private:
      /*!\brief Get the pointer to a internal variable of the basic FB.
       *
       * \param paInternalName StringId of the internal variable name.
       * \return Pointer to the internal variable or 0.
       */
      CIEC_ANY *getInternalVar(StringId paInternalName);

      const std::span<const StringId> cmVarInternalNames;
  };
} // namespace forte
