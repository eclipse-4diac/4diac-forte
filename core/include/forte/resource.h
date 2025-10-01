/*******************************************************************************
 * Copyright (c) 2005 - 2018 ACIN, Profactor GmbH, fortiss GmbH, Johannes Kepler University
 *               2022 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Rene Smodic, Thomas Strasser, Gerhard Ebenhofer, Ingo Hegny,
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst - add CTF tracing integration
 *******************************************************************************/

#pragma once

#include <utility>

#include "forte/ecet.h"
#include "forte/fbcontainer.h"
#include "forte/funcbloc.h"

#include <unordered_map>

namespace forte {
  class ResourceInternal;
  struct SManagementCMD;

  /*! \ingroup CORE\brief Base class for all resources handling the reconfiguration management within this
   * resource and the background execution of event chains.
   *
   */
  class CResource : public CFunctionBlock {

    public:
      /*! \brief The main constructor for a resource.
       *
       * The resource can only be generated with a given device the resource is contained. A resource can not exist
       * outside of an device.
       *  \param paDevice           the device the resource is contained in
       *  \param paInterfaceSpec   interface-specification of resource
       *  \param paInstanceNameId    StringId of instance-name
       *  \param paObjectHandler    reference to object handler
       *  \param paFBData           Byte-array for resource-specific data
       */
      CResource(CFBContainer &paDevice, const SFBInterfaceSpec &paInterfaceSpec, StringId paInstanceNameId);

      ~CResource() override;

      /*!\brief Execute the given management command
       *
       * Evaluates the mDestination parameter of the command if empty the resource tries
       * to execute the management command if not it tries to apply the mgm command to a
       * contained FB if possible.
       * \param paCommand internal representation of the management command
       * \return response of the MGMCommand execution as defined in IEC 61499
       */
      virtual EMGMResponse executeMGMCommand(SManagementCMD &paCommand);

      CResource *getResource() override {
        return this;
      }

      const CResource *getResource() const override {
        return this;
      }

      CDevice *getDevice() override {
        return CFBContainer::getDevice();
      }

      const CDevice *getDevice() const override {
        return CFBContainer::getDevice();
      }

      std::string getFullQualifiedApplicationInstanceName(const char) const override {
        // we don't want to add anything here as the resource name should be excluded
        return std::string();
      }

      void getFullQualifiedApplicationInstanceName(TNameIdentifier &) const override {
        // we don't want to add anything here as the resource name should be excluded
      }

      CEventChainExecutionThread *getResourceEventExecution() const {
        return mResourceEventExecution.get();
      };

      EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

      /*!\brief Write a parameter value to a given FB-input
       *
       * @param paNameList the identifier name list of the parameter to be written
       * @param paValue the value to be writen
       * @return response of the command execution as defined in IEC 61499
       */
      virtual EMGMResponse writeValue(TNameIdentifier &paNameList, const std::string &paValue, bool paForce = false);

      bool isDynamicContainer() override {
        return true;
      }

      CConnection::Wrapper getOutputConnection(std::span<const StringId> paSrcNameList) override;

      [[nodiscard]] ResourceInternal &getInternal() const {
        return *mInternal;
      }

    protected:
      CResource(const SFBInterfaceSpec &paInterfaceSpec, StringId paInstanceNameId);

      void executeEvent(TEventID, CEventChainExecutionThread *const) override {
        // nothing to do here for a resource
      }

      void readInputData(TEventID) override {
        // nothing to read from for a resource
      }

      void writeOutputData(TEventID) override {
        // nothing to write to for a resource
      }

      CIEC_ANY *getDO(size_t) override {
        return nullptr;
      }

      CEventConnection *getEOConUnchecked(TPortId) override {
        return nullptr;
      }

      CDataConnection *getDOConUnchecked(TPortId) override {
        return nullptr;
      }

      std::unordered_map<TNameIdentifier, std::string> mInitialValues;

      void setInitialValues() override;

      /*! Wrapper for simplifying connection creation in resources
       *
       */
      EMGMResponse createConnection(SManagementCMD &paCommand);

    private:
      /*!\brief Create a new connection between source and destination
       *
       * This function is protected so that devices and resource can utilize to setup their connections.
       * @param  paSrcNameList identifier name list for the source of the connection
       * @param  padstNameList identifier name list for the destination of the connection
       * @return response of the command execution as defined in IEC 61499
       */
      EMGMResponse createConnection(TNameIdentifier &paSrcNameList, TNameIdentifier &paDstNameList);

      /*! Handle and perform the actions required for an execution state change
       * command (i.e., START, STOP, KILL, RESET)
       *
       * @param paCMD    the ecuctoin state change command to be performed
       * @param paTarget identifier which is the target for this request if empty the target is the resource
       * @return response of the command execution as defined in IEC 61499
       */
      EMGMResponse handleExecutionStateCmd(EMGMCommandType paCMD, TNameIdentifier &paTarget);

      /*!\brief Create a new connection between source and destination
       *
       * @param  paSrcNameList identifier name list for the source of the connection
       * @param  padstNameList identifier name list for the destination of the connection
       * @return response of the command execution as defined in IEC 61499
       */
      EMGMResponse deleteConnection(TNameIdentifier &paSrcNameList, TNameIdentifier &paDstNameList);

      /*!\brief Read a parameter value from a given FB
       *
       * @param paNameList the identifier name list of the parameter to be read
       * @param paValue the destination for the value to be read
       * @return response of the command execution as defined in IEC 61499
       */
      EMGMResponse readValue(TNameIdentifier &paNameList, std::string &paValue);

      CConnection *getResIf2InConnection(StringId paResInput);

      virtual CConnection *getResIf2InConnectionUnchecked(TPortId) {
        return nullptr;
      }

      /*!\brief The event chain execution of background (low priority) event chains started within this resource
       */
      std::unique_ptr<CEventChainExecutionThread> mResourceEventExecution;

      std::unique_ptr<ResourceInternal> mInternal;
  };
} // namespace forte
