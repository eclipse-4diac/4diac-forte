/*******************************************************************************
 * Copyright (c) 2012 - 2015 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef MGMCMDSTRUCT_H_
#define MGMCMDSTRUCT_H_

#include "mgmcmd.h"
#include "core/util/inplace_vector.h"
#include "stringdict.h"
#include <string>

namespace forte {
  namespace core {

    /**  \ingroup CORE \defgroup MGMCommands Management Command Exchange Representation
     * \brief In this section the FORTE-internal data-exchange-structure for management command passing is described.
     */
    /*@{*/

    typedef util::inplace_vector<CStringDictionary::TStringId,
        FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY> TNameIdentifier;

    /*!\brief All the data necessary for processing a management command.
     *
     * The data of this structure is the internal FORTE representation of an IEC 61499 management command.
     * e.g. create \<FB name="START" type="E_RESTART"\> will result in the following data values:
     *    - mCMD = 0x30
     *    - mDestination = ""
     *    - mFistParam = "START"
     *    - mSecondParam = "E_RESTART"
     *    - mAdditionalParams = ""
     */
    struct SManagementCMD{
        /*!\brief An identification number for the requested command based on the definitons in the standard.
         *
         * See the constants in devadmin.h for the available commands
         */
        EMGMCommandType mCMD;

        /*!\brief The destination of the command.
         *
         * if the destination is scmInvalidStringID device is the target.
         * Otherwise a the TStringId of the resource's name the command
         * should be exectued in is the target.
         * FIXME if more than the resource should be supported (e.g. composite FBs) the type needs to be changed .
         */
        CStringDictionary::TStringId mDestination;

        /*!\brief The first parameter of the command e.g. the FB name to create or the src of an connection.
         *
         * The data is stored as an array TStringIds for the hierarchical FB name identifier (e.g., SubApp1.FB, CFB.FB, FB.Port)
         * Each level is stored as one entry in the vector. Top hierarchy is the first entry.
         */
        TNameIdentifier mFirstParam;

        /*!\brief The second parameter of the command e.g. the FB name to create or the src of an connection.
         *
         * The data is stored as an array TStringIds for the hierarchical FB name identifier (e.g., SubApp1.FB, CFB.FB, FB.Port)
         * Each level is stored as one entry in the vector. Top hierarchy is the first entry.
         */
        TNameIdentifier mSecondParam;

        /*!\brief Additional params needed by a MGM command (e.g., to return results of query commands)
         */
        std::string mAdditionalParams;

        /*\brief pointer to the ID to generate the correct response */
        char *mID;

#ifdef FORTE_SUPPORT_MONITORING
        /*\brief pointer to the response to generate the correct response */
        std::string mMonitorResponse;
#endif

    };

  } // namespace core
} // namespace forte

#endif /* MGMCMDSTRUCT_H_ */
