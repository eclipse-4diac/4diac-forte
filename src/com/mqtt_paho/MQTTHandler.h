/*******************************************************************************
 * Copyright (c) 2013, 2023 ACIN, Primetals Technologies Austria GmbH,
 *                          Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *                         - change CIEC_STRING to std::string
 * Markus Meingast         - refactoring and adaption to new Client class,
 *                           enabling connection to multiple servers
 *******************************************************************************/

#ifndef MQTTHANDLER_H_
#define MQTTHANDLER_H_

#include <extevhan.h>
#include <fortelist.h>
#include <MQTTComLayer.h>
#include <forte_sync.h>
#include <forte_thread.h>
#include <forte_sem.h>
#include <string>
#include <vector>
#include <memory>

class CMQTTClient;

class MQTTHandler : public CExternalEventHandler, public CThread {
  DECLARE_HANDLER(MQTTHandler)
public:
  enum RegisterLayerReturnCodes {
    eRegisterLayerSucceeded,
    eWrongClientID,
    eConnectionFailed
  };

  int registerLayer(const std::string& paAddress, const std::string& paClientId, MQTTComLayer* paLayer);

  void unregisterLayer(MQTTComLayer* paLayer);

  void enableHandler() override;
  /*!\brief Disable this event source
   */
  void disableHandler() override;
  /*!\brief Sets the priority of the event source
   *
   * \param paPriority new priority of the event source
   */
  void setPriority(int paPriority) override;
  /*!\brief Get the current priority of the event source
   *
   * \return current priority
   */
  int getPriority() const override;

  void resumeSelfSuspend();

  void selfSuspend();

  void startNewEventChain(MQTTComLayer* layer);

protected:
  void run() override;

private:
  std::shared_ptr<CMQTTClient> getClient(const std::string& paAddress, const std::string& paClientId);

  static CSyncObject smMQTTMutex;
  static const int smSleepTime = 5000;

  static forte::arch::CSemaphore mStateSemaphore;
  static bool mIsSemaphoreEmpty;


  std::vector<std::shared_ptr<CMQTTClient>> mClients;
};

#endif /* MQTTHANDLER_H_ */
