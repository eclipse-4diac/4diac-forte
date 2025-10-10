/*******************************************************************************
 * Copyright (c) 2005, 2025 ACIN, Profactor GmbH, nxtControl GmbH, fortiss GmbH,
 *                          Johannes Kepler University, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gunnar Grabmaier, Smodic Rene,
 *     Gerhard Ebenhofer, Michael Hofmann, Martin Melik Merkumians, Ingo Hegny,
 *     Stanislav Meduna, Patrick Smejkal,
 *                - initial implementation and rework communication infrastructure
 *   Alois Zoitl  - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst - account for data type size in FB initialization
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include <cstddef>
#include "forte/fbcontainer.h"
#include "forte/interfacespec.h"
#include "forte/mgmcmd.h"
#include "forte/dataconn.h"
#include "forte/inoutdataconn.h"
#include "forte/eventconn.h"
#include "forte/util/devlog.h"
#include "forte/stringid.h"

#include <bitset>

namespace forte {
  class CEventChainExecutionThread;
  class CTimerHandler;
  class CDevice;
  class IPlugPin;
  class ISocketPin;
  class CMonitoringHandler;

#ifdef FORTE_REPLAY_DEVICE
  namespace iec61499::system {
    class CResourceReplayer;
  }
#endif // FORTE_REPLAY_DEVICE

  //! Datatype for indicating an absolut port num
  using TAbsDataPortNum = size_t;

  constexpr TAbsDataPortNum INVALID_ABS_DATA_PORT_ID = static_cast<TAbsDataPortNum>(-1);

  typedef StringId TEventTypeID;

  /*!\ingroup CORE\brief Base class for all function blocks.
   */
  class CFunctionBlock : public CFBContainer {
    public:
      /*!\brief Possible states of a runable object.
       *
       */
      enum class E_FBStates {
        Running = 0, // in the most critical execution path of FORTE we are checking for this enum value it is faster if
                     // this is the zero entry
        Idle,
        Stopped,
        Killed
      };

      /*! \brief Indicator that the given EventID is an included adapter's eventID.
       *
       * EventIDs > scmMaxInterfaceEvents: highByte indicates (AdapterID+1)
       */
      static const TEventID scmMaxInterfaceEvents = cgInvalidPortId;
      static_assert((scmMaxInterfaceEvents & (scmMaxInterfaceEvents + 1)) == 0,
                    "scmMaxInterfaceEvents must be a valid bitmask");

      bool initialize() override;
      void deinitialize() override;

      ~CFunctionBlock() override;

      /*!\brief Get the timer of the device where the FB is contained.
       */
      CTimerHandler &getTimer();

      /*!\brief Returns the stringId for type name of this FB instance
       */
      virtual StringId getFBTypeId() const = 0;

      /*!\brief Returns the type name of this FB instance
       */
      const char *getFBTypeName() const {
        if (const auto type = getFBTypeId()) {
          return type.data();
        }
        return "";
      }

      CEventConnection *getEOConnection(StringId paEONameId);

      const CEventConnection *getEOConnection(StringId paEONameId) const {
        return const_cast<CFunctionBlock *>(this)->getEOConnection(paEONameId);
      }

      /*!\brief Connects specific data input of a FB with a specific data connection.
       *
       * If the connection pointer is 0 then it is equal to a disconnect. Furthermore, if their is already a connection
       * to the given data port the connection attempt will be rejected. However if the further connection attempt
       * is done with the same connection object then it is a reconfiguration attempt for any data inputs. This is
       * necessary when during the first connection the data type of the connection was not clear.
       *
       * \param paDINameId StringID of the data input name.
       * \param paDataCon Pointer to the data connection the FB should be connected to.
       * \return TRUE on success, FALSE if data output not exists or output is already connected.
       */
      virtual bool connectDI(TPortId paDIPortId, CDataConnection *paDataCon);

      virtual bool connectDIO(TPortId paDIPortId, CInOutDataConnection *paDataCon);

      /*!\brief Get the pointer to a data input of the FB.
       *
       * \param paDINameId ID of the data input name.
       * \return Pointer to the data input or 0.
       */
      CIEC_ANY *getDataInput(StringId paDINameId);

      /*!\brief Get the pointer to a data in-out of the FB.
       *
       * \param paDIONameId ID of the data in-out name.
       * \return Pointer to the data input or 0.
       */
      CIEC_ANY *getDataInOut(StringId paDIONameId);

      /*!\brief Get the pointer to a data output of the FB.
       *
       * \param paDONameId StringID of the data output name.
       * \return Pointer to the data output or 0. If 0 is returned DataOutput is ANY
       */
      CIEC_ANY *getDataOutput(StringId paDONameId);

      /*!\brief Get the pointer to a variable of the FB.
       *
       * @param paNameList the name hierarchy the requested variable
       * \return Pointer to the variable or 0.
       *  The variable may be an input, in-out, output or in the case of a BFB an internal var.
       */
      CIEC_ANY *getVar(std::span<const StringId> paNameList) override;

      /*!\brief get the pointer to a data input using the portId as identifier
       */
      CIEC_ANY *getDIFromPortId(TPortId paDIPortId);

      /*!\brief get the pointer to a data output using the portId as identifier
       */
      CIEC_ANY *getDOFromPortId(TPortId paDOPortId);

      CIEC_ANY *getDIOFromPortId(TPortId paDIPortId);

      CDataConnection *getDIConnection(StringId paDINameId);

      const CDataConnection *getDIConnection(StringId paDINameId) const {
        return const_cast<CFunctionBlock *>(this)->getDIConnection(paDINameId);
      }

      CDataConnection *getDOConnection(StringId paDONameId);

      const CDataConnection *getDOConnection(StringId paDONameId) const {
        return const_cast<CFunctionBlock *>(this)->getDOConnection(paDONameId);
      }

      CInOutDataConnection *getDIOInConnection(StringId paDIONameId);

      const CInOutDataConnection *getDIOInConnection(StringId paDIONameId) const {
        return const_cast<CFunctionBlock *>(this)->getDIOInConnection(paDIONameId);
      }

      CInOutDataConnection *getDIOOutConnection(StringId paDIONameId);

      const CInOutDataConnection *getDIOOutConnection(StringId paDIONameId) const {
        return const_cast<CFunctionBlock *>(this)->getDIOOutConnection(paDIONameId);
      }

      /*! \brief if the data input is of generic type (i.e, ANY) this function allows a data connection to configure
       * the DI with the specific type coming from the other end of the connection
       */
      virtual bool configureGenericDI(TPortId paDIPortId, const CIEC_ANY &paRefValue);

      /*! \brief if the data in/out is of generic type (i.e, ANY) this function allows a data connection to configure
       * the DIO with the specific type coming from the other end of the connection
       */
      virtual bool configureGenericDIO(TPortId paDIOPortId, const CIEC_ANY &paRefValue);

      /*! \brief if the data output is of generic type (i.e, ANY) this function allows a data connection to configure
       * the DO with the specific type coming from the other end of the connection
       */
      virtual bool configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue);

      /*!\brief Get the pointer to the plub pin of the FB.
       *
       * \param pasocketNameId  StringId of the socket name.
       * \return Pointer to the socket or nullptr.
       */
      IPlugPin *getPlugPin(StringId paPlugNameId);

      /*!\brief Get the pointer to the socket pin of the FB.
       *
       * \param paSocketNameId  StringId of the socket name.
       * \return Pointer to the socket or nullptr.
       */
      ISocketPin *getSocketPin(StringId paSocketNameId);

      const ISocketPin *getSocketPin(StringId paSocketNameId) const {
        return const_cast<CFunctionBlock *>(this)->getSocketPin(paSocketNameId);
      }

      /*!\brief Function that handles incoming events.
       *
       * \param paEIID ID of the input event that has occurred.
       * \param paExecEnv Event chain execution environment the FB will be executed in (used for adding output events).
       */
      void receiveInputEvent(TEventID paEIID, CEventChainExecutionThread *paExecEnv) {
        FORTE_TRACE("InputEvent: Function Block (%s) got event: %d (maxid: %d)\n", getInstanceNameId().data(), paEIID,
                    getFBInterfaceSpec().getNumEIs() - 1);

#ifdef FORTE_TRACE_CTF
        traceInputEvent(paEIID);
#endif // FORTE_TRACE_CTF
        if (E_FBStates::Running == getState()) {
          if (paEIID < getFBInterfaceSpec().getNumEIs()) {
            readInputData(paEIID);
            // Count Event for monitoring
            mEventMonitorCount[paEIID]++;
          }
          executeEvent(paEIID, paExecEnv);
          if (mForces.any()) [[unlikely]] {
            resetForcedOutputs();
          }
        }
      }

      /*!\brief Configuration interface used by the typelib to parameterize generic function blocks.
       *
       * \param paConfigString  A string containing the needed configuration data.
       * \return TRUE if configuration went ok, else FALSE.
       *
       */
      virtual bool configureFB(const char *paConfigString);

      const SFBInterfaceSpec &getFBInterfaceSpec() const {
        return mInterfaceSpec;
      }

      EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

      /*!\brief Get information if the runable object is deletable by a management command.
       *
       */
      bool getDeletable() const {
        return mDeletable;
      }

      /*!\brief Set attribute to enable/disable the runable object deletion by a management command.
       *
       */
      void setDeletable(const bool &paDelAble) {
        mDeletable = paDelAble;
      }

      /*!\brief Return if the runable object is allowed to be deleted now.
       *
       * This is more complex then the simple deleteable flag as the current state has to be incorporated.
       * According to IEC 61499-1 Figure 24 an FB is deleteable if it is not in the Running state.
       * Furthermore all connection references to this FB have to be removed.
       *
       * \return true if currently all conditions are met to be deleteable
       */
      bool isCurrentlyDeleteable() const {
        return ((mDeletable) && (mFBState != E_FBStates::Running) && mConnRefCount == 0);
      }

      /*!\brief return the current execution state of the managed object
       */
      E_FBStates getState() const {
        return mFBState;
      }

      /*! \brief Get the data input with given number
       *
       * Attention this function will not perform any range checks on the paDINum parameter!
       * @param paDINum number of the data input starting with 0
       * @return pointer to the data input
       */
      virtual CIEC_ANY *getDI(TPortId paDINum) = 0;

      const CIEC_ANY *getDI(TPortId paDINum) const {
        return const_cast<CFunctionBlock *>(this)->getDI(paDINum);
      }

      /*! \brief Get the data output with given number
       *
       * Attention this function will not perform any range checks on the paDONum parameter!
       * @param paDONum number of the data output starting with 0
       * @return pointer to the data output
       */
      virtual CIEC_ANY *getDO(TPortId paDONum) = 0;

      const CIEC_ANY *getDO(TPortId paDONum) const {
        return const_cast<CFunctionBlock *>(this)->getDO(paDONum);
      }

      /*! \brief Get the data output with given number
       *
       * Attention this function will not perform any range checks on the paDONum parameter!
       * @param paDONum number of the data output starting with 0
       * @return pointer to the data output
       */
      virtual CIEC_ANY *getDIO(TPortId paDIONum) {
        (void) paDIONum; // indicate to the compiler that the param is unused
        return nullptr;
      }

      const CIEC_ANY *getDIO(TPortId paDIONum) const {
        return const_cast<CFunctionBlock *>(this)->getDIO(paDIONum);
      }

      /*!\brief get the connection object for the given destination identifier
       *
       * @param paDstNameList array of the name hierarchy the requested connection destination
       * @return pointer to the requested connection, returns nullptr if there is no such destination
       */
      CConnection *getInputConnection(std::span<const StringId> paDstNameList) override;

      /*!\brief get the connection object for the given source identifier
       *
       * @param paSrcNameList array of the name hierarchy the requested connection source
       * @return pointer to the requested connection, returns nullptr if there is no such source
       */
      CConnection::Wrapper getOutputConnection(std::span<const StringId> paSrcNameList) override;

      TForteUInt32 &getEIMonitorData(TEventID paEIID);

      TForteUInt32 &getEOMonitorData(TEventID paEOID);

      TAbsDataPortNum getAbsDataPortNum(StringId paPortNameId) const;

      bool setForced(std::span<const StringId> paNameList, bool paForce) override;

      [[nodiscard]] constexpr bool isForced(const TAbsDataPortNum paAbsDataPortNum) const {
        return paAbsDataPortNum < mForces.size() && mForces[paAbsDataPortNum];
      }

      virtual void toString(std::string &paTargetBuf) const;

#ifdef FORTE_TRACE_CTF
      virtual void traceInstanceData() {
      }
#endif // FORTE_TRACE_CTF

      void addInputEventConnection(TEventID paEIID) {
        if (!getFBInterfaceSpec().mEITypeNames.empty()) {
          mInputEventConnectionCount[paEIID]++;
        }
        incConnRefCount();
      }

      void removeInputEventConnection(TEventID paEIID) {
        if (mInputEventConnectionCount != nullptr && mInputEventConnectionCount[paEIID] > 0) {
          mInputEventConnectionCount[paEIID]--;
        }
        decConnRefCount();
      }

      /* !\brief checks if an input event pin is connected
       *
       */
      [[nodiscard]] bool isInputEventConnected(TEventID paEIID) const {
        return mInputEventConnectionCount != nullptr && mInputEventConnectionCount[paEIID] > 0;
      }

      void incConnRefCount() {
        mConnRefCount++;
      }

      void decConnRefCount() {
        mConnRefCount--;
      }

    protected:
      /*!\brief The main constructor for a function block.
       *
       * \param paContainer      FB container this function block is contained in (mainly necessary for management
       * functions and service interfaces)
       * \param paInterfaceSpec  const pointer to the interface spec
       * \param paInstanceNameId string id
       */
      CFunctionBlock(CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, StringId paInstanceNameId);

      /*!\brief Function to send an output event of the FB.
       *
       * \param paECET the event chain execution thread to be used for sending output events
       * \param paEO Event output ID where event should be fired.
       * \param paExecEnv Event chain execution environment where the event will be sent to.
       */
      void sendOutputEvent(TEventID paEO, CEventChainExecutionThread *const paECET) {
        FORTE_TRACE("OutputEvent: Function Block sending event: %d (maxid: %d)\n", paEO,
                    getFBInterfaceSpec().getNumEOs() - 1);

#ifdef FORTE_TRACE_CTF
        traceOutputEvent(paEO, paECET);
#endif // FORTE_TRACE_CTF
        size_t numEOs = getFBInterfaceSpec().getNumEOs();
        if (paEO < numEOs) {
          writeOutputData(paEO);
          getEOConUnchecked(static_cast<TPortId>(paEO))->triggerEvent(paECET);

          // Count Event for monitoring, use size and number of EOs for performance reason so that only one value has to
          // be gathered from the interface spec
          mEventMonitorCount[mEventMonitorCount.size() - numEOs + paEO]++;
        }
      }

      /*!\brief Function to read data from an input connection into a variable of the FB.
       *
       * \param paAbsDataPortNum absolute data port index to be read
       * \param paValue Variable to read into.
       * \param paConn Connection to read from.
       */
      void readData(const TPortId paAbsDataPortNum, CIEC_ANY &paValue, const CDataConnection *const paConn) {
        if (!paConn) {
          return;
        }
        if (!isForced(paAbsDataPortNum)) [[likely]] {
          paConn->readData(paValue);
        }
#ifdef FORTE_TRACE_CTF
        traceReadData(paAbsDataPortNum, paValue);
#endif // FORTE_TRACE_CTF
      }

      /*!\brief Function to write data to an output connection from a variable of the FB.
       *
       * \param paAbsDataPortNum absolute data port index to be written
       * \param paValue Variable to write from.
       * \param paConn Connection to write into.
       */
      template<typename T, typename U>
      void writeData(const TAbsDataPortNum paAbsDataPortNum, T &paValue, U &paConn) {
        if (!isForced(paAbsDataPortNum)) [[likely]] {
          paConn.writeData(paValue);
        }
#ifdef FORTE_TRACE_CTF
        traceWriteData(paAbsDataPortNum - mInterfaceSpec.getNumDIs(), paValue);
#endif // FORTE_TRACE_CTF
      }

      /*!\brief Set the initial values of data inputs, outputs, and internal vars.
       *
       * The default implementation initializes all input, output, and internal variables
       * to the implicit default value corresponding to the type of the variable.
       *
       * Any FB with explicit default values still has to override setInitialValues for proper Reset behavior,
       * other FBs may choose to override for improved performance.
       */
      virtual void setInitialValues() = 0;

      /*!\brief Function to send an output event via the adapter.
       *
       * \param paAdapter adpater for which the event should be sent
       * \param paEID Event ID where event should be fired.
       * \param paExecEnv Event chain execution environment where the event will be sent to.
       */
      void sendAdapterEvent(CAdapter &paAdapter, TEventID paEID, CEventChainExecutionThread *const paECET);

      virtual CEventConnection *getEOConUnchecked(TPortId paEONum) = 0;

      /*! \brief Get the data output connection with given number
       *
       * Attention this function will not perform any range checks on the paDONum parameter!
       * @param paDONum number of the data output starting with 0
       * @return pointer to the data output connection
       */
      virtual CDataConnection *getDOConUnchecked(TPortId paDONum) = 0;

      /*! \brief Get the data input connection with given number
       *
       * Attention this function will not perform any range checks on the paDINum parameter!
       * @param paDINum number of the data input starting with 0
       * @return pointer to the data input connection
       */
      virtual CDataConnection **getDIConUnchecked(TPortId paDINum) = 0;

      /*! \brief Get the data inout connection with given number
       *
       * Attention this function will not perform any range checks on the paDIONum parameter!
       * @param paDIONum number of the data inout starting with 0
       * @return pointer to the data inout connection
       */
      virtual CInOutDataConnection **getDIOInConUnchecked(TPortId paDIONum) {
        (void) paDIONum; // indicate to the compiler that the param is unused
        return nullptr;
      }

      virtual CInOutDataConnection *getDIOOutConUnchecked(TPortId) {
        return nullptr;
      }

      virtual IPlugPin *getPlugPinUnchecked(TPortId) {
        return nullptr;
      }

      virtual ISocketPin *getSocketPinUnchecked(TPortId) {
        return nullptr;
      }

      const SFBInterfaceSpec &mInterfaceSpec; //!< Pointer to the interface specification

      /*!\brief initialize the data structure which holds connection counts per pin
       */
      void setupInputConnectionTrackingData() {
        if (!getFBInterfaceSpec().mEITypeNames.empty()) {
          mInputEventConnectionCount = std::make_unique<size_t[]>(getFBInterfaceSpec().getNumEIs());
        }
      }

      void setupEventMonitoringData();
      void freeEventMonitoringData();

    private:
      bool isFB() override {
        return true;
      }

      bool isDynamicContainer() override {
        return false;
      }

      /*!\brief Function providing the functionality of the FB (e.g. execute ECC for basic FBs).
       *
       * \param paECET the event chain execution thread this FB was invoked from
       * \param paEIID Event input ID where event occurred.
       * \param paExecEnv Event chain execution environment the FB will be executed in (used for adding output
       * events).
       *
       *
       */
      virtual void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) = 0;

      /*!\brief Function reading the values from input connections of the FB.
       *
       * \param paEIID Event input ID where event occurred.
       */
      virtual void readInputData(TEventID paEIID) = 0;

      /*!\brief Function writing the values to output connections of the FB.
       *
       * \param paEIID Event output ID where event occurred.
       */
      virtual void writeOutputData(TEventID paEO) = 0;

      bool setForced(TAbsDataPortNum paAbsDataPortNum, bool paForceValue);

      /*!\brief Function resetting the values of forced outputs to the value from the output connection.
       */
      void resetForcedOutputs();

    public:
      CFunctionBlock(const CFunctionBlock &) = delete;

    protected:
      constexpr static char csmToStringSeparator[] = ", ";

    private:
      //! vector for storing the event counts for input and output events together, first inputs then outputs
      std::vector<TForteUInt32> mEventMonitorCount;
      //! vector of bools for determining force state of data inputs, data outputs, and var in outs in that order
      std::bitset<cgMaxSupportedForceIndex> mForces;

#ifdef FORTE_TRACE_CTF
      void traceInputEvent(TEventID paEIID);
      void traceOutputEvent(TEventID paEOID, CEventChainExecutionThread *const paECET);
      void traceReadData(TPortId paDINum, CIEC_ANY &paValue);
      void traceWriteData(TPortId paDONum, CIEC_ANY &paValue);
#endif

      /*!\brief Current state of the runnable object.
       *
       */
      E_FBStates mFBState;

      /*!\brief Stores the number of input connections for each event pin
       */
      std::unique_ptr<size_t[]> mInputEventConnectionCount;

      /*!\brief Stores the total number of connections that reference this FB
       */
      size_t mConnRefCount;

      /*!\brief Attribute defines if runnable object can be deleted by a management command.
       *
       * Default value is set to true.
       * If the runnable object is declared in a device or resource specification it must be set to false.
       */
      bool mDeletable;

      friend class CMonitoringHandler;

#ifdef FORTE_FMU
      friend class fmuInstance;
#endif // FORTE_FMU

#ifdef FORTE_REPLAY_DEVICE
      friend class forte::iec61499::system::CResourceReplayer;
#endif // FORTE_REPLAY_DEVICE
  };

#ifdef OPTIONAL
#undef OPTIONAL
#endif

} // namespace forte
