/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FBCONTAINER_H_
#define _FBCONTAINER_H_

#include "fortelist.h"
#include "stringdict.h"
#include "mgmcmdstruct.h"

class CFunctionBlock;

namespace forte {
  namespace core {

    class CFBContainer{
      public:
        CFBContainer(CStringDictionary::TStringId paContainerName, CFBContainer *paParent);
        virtual ~CFBContainer();

        CStringDictionary::TStringId getName() const{
          return mContainerName;
        }

        /*!\brief Gets a function block from the container or recursively from its subcontainers
         *
         * @param paNameList itartor to the name hierarchy the requested function block, if retval is not 0 it will point to the the item which found the FB
         * @return pointer to the requested function block, returns 0 if function block is not in the list
         */
        CFunctionBlock* getContainedFB(forte::core::TNameIdentifier::CIterator &paNameListIt);

        /*!\brief Adds a created function block to the FB-List
         *
         * @param pa_poFuncBlock new function block to add to the FB-List
         * @return response of the command execution as defined in IEC 61499
         */
        EMGMResponse addFB(CFunctionBlock *pa_poFuncBlock);

        typedef CSinglyLinkedList<CFunctionBlock *> TFunctionBlockList;

        TFunctionBlockList &getFBList(){
          return mFunctionBlocks;
        }

      protected:

        /*!\brief Create a new FB instance of given type and name
         *
         * @param paNameListIt    iterator to the current position in the name list for the FB to be created (e.g., SubApp1.SubApp2.FBName, FBName2)
         * @param paTypeName      the type name of the FB to be created
         * @param paRes           the resource this container is contained in and in which the fb should reside
         * @return response of the command execution as defined in IEC 61499
         */
        EMGMResponse createFB(forte::core::TNameIdentifier::CIterator &paNameListIt,
            CStringDictionary::TStringId paTypeName, CResource *paRes);

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
        CFBContainer *getFBContainer(CStringDictionary::TStringId paContainerName);

        //! Change the execution state of all contained FBs and also recursively in all contained containers
        EMGMResponse changeContainedFBsExecutionState(EMGMCommandType paCommand);


        typedef CSinglyLinkedList<CFBContainer *> TFBContainerList;

      private:
        /*!\brief Retrieve a FBContainer with given name. If it does not exist create it.
         *
         * @param paContainerName name of the container
         * @return pointer to the container or 0 if an FB with the same name exists in this container
         */
        CFBContainer *findOrCreateContainer(CStringDictionary::TStringId paContainerName);

        CStringDictionary::TStringId mContainerName; //!< name of the container
        CFBContainer *mParent; //!< the parent FBContainer this FBContainer is contained in. The parent of a resource is 0

        TFunctionBlockList mFunctionBlocks; //!< The functionblocks hold in this container
        TFBContainerList mSubContainers; //!< List of subcontainers (i.e, subapplications in this container)
    };

  } /* namespace core */
} /* namespace forte */

#endif /* FBCONTAINER_H_ */
