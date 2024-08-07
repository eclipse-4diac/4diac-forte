/*******************************************************************************
 * Copyright (c) 2005, 2024 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
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
  public:
    ~CDevice() override = default;

    /*! \brief Starts to execute FBNs.
     *
     *  This function will send a IEC 61499 start command to all existing resources in the device (if any).
     *  The function will return after the start. The Forte HAL has to ensure that it is waited till the device
     *  finishes its execution.
     *  \return 0 on success -1 on error
     */
    virtual int startDevice() {
      changeExecutionState(EMGMCommandType::Start);
      return 1;
    }

    /*!\brief wait till the execution of this device ends.
     *
     * The execution of devices can be stopped by sending it the kill command.
     */
    virtual void awaitShutdown() = 0;

    CStringDictionary::TStringId getFBTypeId() const override {
      return CStringDictionary::scmInvalidStringId;
    }

    /*!\brief Execute the given management command
     *
     * Evaluates the mDestination parameter of the command if empty this class tries to execute the management command if not
     * the referenced resource is called for handling the command.
     * \param paCommand FORTE internal representation of the management command
     * \return response of the MGMCommand execution as defined in IEC 61499
     */
    EMGMResponse executeMGMCommand(forte::core::SManagementCMD &paCommand) override;

    EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

    //! Retrieve the device execution of this device
    CDeviceExecution &getDeviceExecution() {
      return mDeviceExecution;
    }

    CTimerHandler& getTimer() const {
      return mDeviceExecution.getTimer();
    }

    CDevice* getDevice() override {
      return this;
    }

    const CDevice* getDevice() const override {
      return this;
    }

  protected:
    /*! \brief Sets up all the necessary data and classes necessary for execution.
     *
     */
    CDevice(const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
        CResource(paInterfaceSpec, paInstanceNameId), mDeviceExecution(*this) {
    }

  private:
     /*! \brief
     *
     */
    CDeviceExecution mDeviceExecution;

};

#endif
