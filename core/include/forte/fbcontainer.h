/*******************************************************************************
 * Copyright (c) 2015, 2025 fortiss GmbH, Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial implementation and rework communication infrastructure
 *               - merged fbs and containers in one list
 *   Martin Jobst - add smart pointer for internal FBs
 *                - use span for create and delete FB
 *******************************************************************************/

#pragma once

#include "forte/conn.h"
#include "forte/stringid.h"
#include <vector>
#include <memory>

namespace forte {
  class CIEC_ANY;

  class CAdapter;
  class CAnyAdapterPin;
  class CDevice;
  class CFunctionBlock;
  class CResource;

  class CFBContainer {
      template<typename U>
      friend class CInternalFB;

      friend class CAnyAdapterPin;

    protected:
      using NameIterator = TNameIdentifier::const_iterator;

    public:
      CFBContainer(StringId paContInstanceName, CFBContainer &paParent);

      virtual bool initialize();
      virtual void deinitialize();

      virtual ~CFBContainer();

      StringId getInstanceNameId() const {
        return mContInstanceName;
      }

      const char *getInstanceName() const {
        return mContInstanceName.data();
      }

      /*!\brief Create a new FB instance of given type and name
       *
       * @param paInstanceNameId    instance name for the FB to be created
       * @param paTypeName      the type name of the FB to be created
       * @return response of the command execution as defined in IEC 61499
       */
      EMGMResponse createFB(StringId paInstanceNameId, StringId paTypeName, std::string_view paTypeHash);

      /*!\brief Gets a function block from the container or recursively from its subcontainers
       *
       * @param paNameList iterator to the name hierarchy the requested function block, if retval is not 0 it will
       * point to the the item which found the FB
       * @return pointer to the requested function block, returns 0 if function block is not in the list
       */
      virtual CFunctionBlock *getFB(NameIterator &paNameListIt, NameIterator paNameListEnd);

      typedef std::vector<CFBContainer *> TFBContainerList;

      TFBContainerList &getChildren() {
        return mChildren;
      }

      const TFBContainerList &getChildren() const {
        return mChildren;
      }

      CFBContainer &getParent() const {
        return mParent;
      }

      virtual CResource *getResource() {
        return mParent.getResource();
      }

      virtual const CResource *getResource() const {
        return const_cast<CFBContainer *>(this)->getResource();
      }

      virtual CDevice *getDevice() {
        return mParent.getDevice();
      }

      virtual const CDevice *getDevice() const {
        return const_cast<CFBContainer *>(this)->getDevice();
      }

      /*! \brief Get the full hierarchical name of this FB in its application
       *
       * Generates a dot separated name list of this FB excluding device and resource
       *
       * \return full hierarchical name
       */
      virtual std::string getFullQualifiedApplicationInstanceName(const char sepChar) const;

      /*! \brief Get the full hierarchical name of this FB in its application
       * \param paResult The result to append the name into
       */
      virtual void getFullQualifiedApplicationInstanceName(TNameIdentifier &paResult) const;

      //! Change the execution state of all contained FBs and also recursively in all contained containers
      virtual EMGMResponse changeExecutionState(EMGMCommandType paCommand);

      virtual bool isFB() {
        return false;
      }

      virtual bool isDynamicContainer() {
        return true;
      }

      /*!\brief Get the pointer to a variable of the FB container.
       *
       * @param paNameList the name hierarchy the requested variable
       * \return Pointer to the variable or 0.
       */
      virtual CIEC_ANY *getVar(std::span<const StringId> paNameList);

      /*!\brief Set the force flag for a variable of the FB container.
       *
       * @param paNameList the name hierarchy the requested variable
       * \return true on success, false otherwise.
       */
      virtual bool setForced(std::span<const StringId> paNameList, bool paForce);

      /*!\brief get the connection object for the given destination identifier
       *
       * @param paDstNameList array of the name hierarchy the requested connection destination
       * @return pointer to the requested connection, returns nullptr if there is no such destination
       */
      virtual CConnection *getInputConnection(std::span<const StringId> paDstNameList);

      /*!\brief get the connection object for the given source identifier
       *
       * @param paSrcNameList array of the name hierarchy the requested connection source
       * @return pointer to the requested connection, returns nullptr if there is no such source
       */
      virtual CConnection::Wrapper getOutputConnection(std::span<const StringId> paSrcNameList);

    protected:
      /*!\brief Create a new FB instance of given type and name
       *
       * @param paNameList the name list for the FB to be created (e.g., SubApp1.SubApp2.FBName, FBName2)
       * @param paTypeName      the type name of the FB to be created
       * @return response of the command execution as defined in IEC 61499
       */
      EMGMResponse createFB(std::span<const StringId> paNameList, StringId paTypeName, std::string_view paTypeHash);

      /*!\brief Delete a FB instance with given name
       *
       * @param paNameList the name list for the FB to be deleted (e.g., SubApp1.SubApp2.FBName, FBName2)
       * @return response of the command execution as defined in IEC 61499
       */
      EMGMResponse deleteFB(std::span<const StringId> paNameList);

      /*! get fb contained in this fbcontainer
       *
       */
      CFunctionBlock *getFB(StringId paFBName);

      CFBContainer *getChild(StringId paName);

      TFBContainerList::iterator getChildrenIterator(StringId paName);

    private:
      /*!\brief Adds a function block created via the typelib to the FB-List
       *
       * @param paFuncBlock new function block to add to the FB-List
       */
      void addFB(CFunctionBlock &paFuncBlock);

      /*!\brief Removes a function block from the FB-List
       *
       * @param paFuncBlock function block to remove from the FB-List
       */
      void removeFB(CFunctionBlock &paFuncBlock);

      /*!\brief Check if the given iterator points to a valid child with the provide name
       *
       */
      bool isChild(TFBContainerList::iterator childIt, StringId paName) {
        return (childIt != mChildren.end() && (*childIt)->getInstanceNameId() == paName);
      }

      /*!\brief Retrieve a FBContainer with given name. If it does not exist create it.
       *
       * @param paContainerName name of the container
       * @return pointer to the container or 0 if an FB with the same name exists in this container
       */
      CFBContainer *findOrCreateContainer(StringId paContainerName);

      const StringId mContInstanceName; //!< Instance name of the container
      CFBContainer &mParent; //!< the parent FBContainer this FBContainer is contained in. The parent of a device is
                             //!< the device itself!

      TFBContainerList mChildren; //!< List of children (i.e, fbs or subapplications in this container)
  };

  template<typename T>
  class CInternalFB {
      static_assert(std::is_base_of_v<CFunctionBlock, T>, "T must be a CFunctionBlock");

    public:
      CInternalFB(StringId paInstanceNameId, CFBContainer &paContainer) :
          mFB(std::make_unique<T>(paInstanceNameId, paContainer)) {
        paContainer.addFB(*mFB);
      }

      CInternalFB(StringId paInstanceNameId, const char *paConfigString, CFBContainer &paContainer) :
          mFB(std::make_unique<T>(paInstanceNameId, paContainer)) {
        mFB->configureFB(paConfigString);
        paContainer.addFB(*mFB);
      }

      template<typename U = T, typename = typename std::enable_if<std::is_base_of<CAdapter, U>::value>::type>
      CInternalFB(StringId paInstanceNameId, CFBContainer &paContainer, TForteUInt8 paParentAdapterlistID) :
          mFB(std::make_unique<T>(paInstanceNameId, paContainer, paParentAdapterlistID)) {
        paContainer.addFB(*mFB);
      }

      ~CInternalFB() {
        mFB->getParent().removeFB(*mFB);
      }

      T *get() noexcept {
        return mFB.get();
      }

      T &operator*() noexcept {
        return mFB.operator*();
      }

      T *operator->() noexcept {
        return mFB.operator->();
      }

      template<typename... Args>
      auto operator()(Args &&...args) {
        return (*mFB)(std::forward<Args>(args)...);
      }

    private:
      std::unique_ptr<T> mFB;
  };
} /* namespace forte */
