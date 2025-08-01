/*******************************************************************************
 * Copyright (c) 2005, 2025 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gunnar Grabmair, Rene Smodic, Gerhard Ebenhofer,
 *     Martin Melik Merkumians, Ingo Hegny, Micheal Hofmann
 *      - initial implementation and rework communication infrastructure
 *   Martin Jobst - add support for data types with different size
 *   Alois Zoitl  - reworked and modernized, added support for type hashes
 *                - split into internal and public api
 *******************************************************************************/

#pragma once

#include "core/typelib.h"
#include "core/stringid.h"
#include <vector>

class CFunctionBlock;
class CResource;
class CIEC_ANY;

namespace forte {
  class CAdapter;
  namespace core {
    class CFBContainer;

    /*!\brief Create a new FB instance of given type and given instance name.
     *
     * \param paInstanceNameId  StringId of instance name as this information can be stored within the resource
     * \param paFBTypeId Type of the FB to create.
     * \param paContainer   FB container the FB is contained in.
     * \return On success a pointer to the new FB is returned, else the return value is 0 use getLastError for getting
     * more information on the problem. possible error codes are:
     *    - UnsupportedType   The requested FB type is not known to the typelib
     *    - InvalidOperation The requested FB can not be created (e.g. out of memory)
     */
    CFunctionBlock *createFB(forte::core::StringId paInstanceNameId,
                             forte::core::StringId paFBTypeId,
                             std::string_view paTypeHash,
                             forte::core::CFBContainer &paContainer,
                             EMGMResponse &paErrorMSG);

    CFunctionBlock *createFB(forte::core::StringId paInstanceNameId,
                             forte::core::StringId paFBTypeId,
                             forte::core::CFBContainer &paContainer);

    /*\brief Delete the given FB
     */
    bool deleteFB(CFunctionBlock *paFBToDelete);

    forte::CAdapter *createAdapter(forte::core::StringId paInstanceNameId,
                                   forte::core::StringId paAdapterTypeId,
                                   forte::core::CFBContainer &paContainer,
                                   bool paIsPlug,
                                   TForteUInt8 paParentAdapterlistID,
                                   EMGMResponse &paErrorMSG);

    CIEC_ANY *createDataTypeInstance(forte::core::StringId paDTNameId, TForteByte *paDataBuf, EMGMResponse &paErrorMSG);

    CFBTypeEntry *getFBTypeEntry(forte::core::StringId paTypeNameId);
    CAdapterTypeEntry *getAdapterTypeEntry(forte::core::StringId paTypeNameId);
    CGlobalConstEntry *getGlobalConstTypeEntry(forte::core::StringId paTypeNameId);

    const std::vector<CFBTypeEntry *> &getFBTypeEntries();
    const std::vector<CAdapterTypeEntry *> &getAdapterTypeEntries();
    const std::vector<CDataTypeEntry *> &getDataTypeEntries();
    const std::vector<CGlobalConstEntry *> &getGlobalConstEntries();
  } // namespace core
} // namespace forte
