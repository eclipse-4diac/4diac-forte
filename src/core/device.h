/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _DEVICE_H
#define _DEVICE_H

#include "resource.h"
#include "devexec.h"

/*!\ingroup CORE CDevice represents a device according to IEC 61499. CDevice contains
 - one or more IEC 61499 compliant resources (CResource),
 -  a device management (CDeviceAdministrator)
 -  a function block execution environment (CFBNExecution)
 -  a library (CTypeLib) which administrates the types the 1499 device is
 capable of
 - External event interface for handling interrupts and other external notifications.
 -  FORTE-HAL: FORTE Hardware Abstraction Layer

 There are different device type's available (e.g. remote device RMT_DEV)
 for different tasks which have different device configurations
 (resources function blocks) and device parameters.
 */
class CDevice : public CResource {
  private:
    /*! \brief
     *
     */
    CDeviceExecution mDeviceExecution;

  protected:
    virtual void executeEvent(int) {
    }

  public:
    /*! \brief Sets up all the necessary data and classes necessary for execution.
     *
     */
    CDevice(const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId, TForteByte *pa_acFBConnData,
        TForteByte *pa_acFBVarsData) :
        CResource(pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acFBVarsData), mDeviceExecution() {
    }

    virtual ~CDevice() {
    }


    virtual CStringDictionary::TStringId getFBTypeId(void) const {
      return CStringDictionary::scm_nInvalidStringId;
    }


    /*! \brief Starts to execute FBNs.
     *
     *  This function will send a IEC 61499 start command to all existing resources in the device (if any).
     *  The function will return after the start. The Forte HAL has to ensure that it is waited till the device
     *  finishes its execution.
     *  \return 0 on success -1 on error
     */
    virtual int startDevice(void) {
      changeFBExecutionState(cg_nMGM_CMD_Start);
      return 1;
    }

    /*!\brief Execute the given management command
     *
     * Evaluates the m_sDestination parameter of the command if empty this class tries to execute the management command if not
     * the referenced resource is called for handling the command.
     * \param pa_oCommand FORTE internal representation of the management command
     * \return response of the MGMCommand execution as defined in IEC 61499
     */
    virtual EMGMResponse executeMGMCommand(forte::core::SManagementCMD &paCommand);

    virtual EMGMResponse changeFBExecutionState(EMGMCommandType paCommand);

    //! Retrieve the device execution of this device
    CDeviceExecution &getDeviceExecution(void) {
      return mDeviceExecution;
    }

    CTimerHandler& getTimer() const {
      return mDeviceExecution.getTimer();
    }
};

#endif
