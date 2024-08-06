/*******************************************************************************
 * Copyright (c) 2015, 2024 fortiss GmbH, Primetals Technologies Austria GmbH
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
 *******************************************************************************/
#ifndef _FBCONTAINER_H_
#define _FBCONTAINER_H_

#include "fortelist.h"
#include "stringdict.h"
#include "mgmcmdstruct.h"
#include <vector>

class CFunctionBlock;
class CDevice;
class CResource;

namespace forte {
  namespace core {

    class CFBContainer{
      public:
        CFBContainer(CStringDictionary::TStringId paContInstanceName, CFBContainer &paParent);
        virtual ~CFBContainer();

        CStringDictionary::TStringId getInstanceNameId() const{
          return mContInstanceName;
        }

        const char* getInstanceName() const {
          return CStringDictionary::getInstance().get(mContInstanceName);
        }

        /*!\brief Adds a function block created via the typelib to the FB-List
         *
         * @param paFuncBlock new function block to add to the FB-List
         * @return response of the command execution as defined in IEC 61499
         */
        EMGMResponse addFB(CFunctionBlock *paFuncBlock);

        /*!\brief Gets a function block from the container or recursively from its subcontainers
         *
         * @param paNameList iterator to the name hierarchy the requested function block, if retval is not 0 it will point to the the item which found the FB
         * @return pointer to the requested function block, returns 0 if function block is not in the list
         */
        virtual CFunctionBlock *getFB(forte::core::TNameIdentifier::CIterator &paNameListIt);

        typedef std::vector<CFBContainer *> TFBContainerList;

        TFBContainerList &getChildren(){
          return mChildren;
        }

        const TFBContainerList &getChildren() const {
          return mChildren;
        }

        CFBContainer& getParent() const { return mParent;}

        virtual CResource* getResource(){
          return mParent.getResource();
        }

        virtual const CResource* getResource() const {
          return const_cast<CFBContainer*>(this)->getResource();
        }

        virtual CDevice* getDevice(){
          return mParent.getDevice();
        }

        virtual const CDevice* getDevice() const {
          return const_cast<CFBContainer*>(this)->getDevice();
        }

        /*! \brief Get the full hierarchical name of this FB in its application
         *
         * Generates a dot separated name list of this FB excluding device and resource
         *
         * \return full hierarchical name
         */
        virtual std::string getFullQualifiedApplicationInstanceName(const char sepChar) const;

        //! Change the execution state of all contained FBs and also recursively in all contained containers
        virtual EMGMResponse changeExecutionState(EMGMCommandType paCommand);

        virtual bool isFB() {
          return false;
        }

        virtual bool isDynamicContainer() {
          return true;
        }

      protected:
        /*!\brief Create a new FB instance of given type and name
         *
         * @param paNameListIt    iterator to the current position in the name list for the FB to be created (e.g., SubApp1.SubApp2.FBName, FBName2)
         * @param paTypeName      the type name of the FB to be created
         * @return response of the command execution as defined in IEC 61499
         */
        EMGMResponse createFB(forte::core::TNameIdentifier::CIterator &paNameListIt, CStringDictionary::TStringId paTypeName);

        /*!\brief Create a new FB instance of given type and name
         *
         * @param paInstanceNameId    instance name for the FB to be created
         * @param paTypeName      the type name of the FB to be created
         * @return response of the command execution as defined in IEC 61499
         */
        EMGMResponse createFB(CStringDictionary::TStringId paInstanceNameId, CStringDictionary::TStringId paTypeName);

        /*!\brief Delete a FB instance with given name
         *
         * @param paNameListIt    iterator to the current position in the name list for the FB to be deleted (e.g., SubApp1.SubApp2.FBName, FBName2)
         * @return response of the command execution as defined in IEC 61499
         */
        EMGMResponse deleteFB(forte::core::TNameIdentifier::CIterator &paNameListIt);

        /*! get fb contained in this fbcontainer
         *
         */
        CFunctionBlock *getFB(CStringDictionary::TStringId paFBName);

        CFBContainer *getChild(CStringDictionary::TStringId paName);

        TFBContainerList::iterator getChildrenIterator(CStringDictionary::TStringId paName);

      private:

        /*!\brief Check if the given iterator points to a valid child with the provide name
         *
         */
        bool isChild(TFBContainerList::iterator childIt, CStringDictionary::TStringId paName) {
          return (childIt != mChildren.end() && (*childIt)->getInstanceNameId() == paName);
        }

        /*!\brief Retrieve a FBContainer with given name. If it does not exist create it.
         *
         * @param paContainerName name of the container
         * @return pointer to the container or 0 if an FB with the same name exists in this container
         */
        CFBContainer *findOrCreateContainer(CStringDictionary::TStringId paContainerName);

        const CStringDictionary::TStringId mContInstanceName; //!< Instance name of the container
        CFBContainer &mParent; //!< the parent FBContainer this FBContainer is contained in. The parent of a device is the device itself!

        TFBContainerList mChildren; //!< List of children (i.e, fbs or subapplications in this container)
    };

  } /* namespace core */
} /* namespace forte */

#endif /* FBCONTAINER_H_ */
