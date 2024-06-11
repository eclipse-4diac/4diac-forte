/*******************************************************************************
 * Copyright (c) 2024 Primetals Technologies Austria GmbH, Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Martin Jobst - initial implementation and rework communication infrastructure
 *******************************************************************************/

#include "device.h"

class C4diacFORTEInstance {
  public:
    C4diacFORTEInstance() = default;

    bool startupNewDevice(const std::string &paMGRID);

    void triggerDeviceShutdown();

    void awaitDeviceShutdown();

  private:
    /*  \brief Create an instance of the device that is configured for this 4diac FORTE instance
     *
     * This method is to be implemented by the code providing the device to be used for this 4diac FORTE instance.
     *
     * \param paMGRID  string for configuring the mgr id of this device (e.g., port address for a TCP port)
     *                 if an empty string is given the device default mgr id will be used.
     * \return pointer to the newly created device, nullptr if it could not be created.
     *
     */
    static std::unique_ptr<CDevice>  createDev(const std::string &paMGRID);

    std::unique_ptr<CDevice> mActiveDevice;

};

