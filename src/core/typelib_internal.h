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

#include "typelib.h"
#include "core/stringdict.h"
#include <vector>

class CFunctionBlock;
class CResource;
class CIEC_ANY;
class CAdapter;

namespace forte::core {
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
  CFunctionBlock *createFB(CStringDictionary::TStringId paInstanceNameId,
                           CStringDictionary::TStringId paFBTypeId,
                           std::string_view paTypeHash,
                           forte::core::CFBContainer &paContainer,
                           EMGMResponse &paErrorMSG);

  CFunctionBlock *createFB(CStringDictionary::TStringId paInstanceNameId,
                           CStringDictionary::TStringId paFBTypeId,
                           forte::core::CFBContainer &paContainer);

  /*\brief Delete the given FB
   */
  bool deleteFB(CFunctionBlock *paFBToDelete);

  CAdapter *createAdapter(CStringDictionary::TStringId paInstanceNameId,
                          CStringDictionary::TStringId paFBTypeId,
                          forte::core::CFBContainer &paContainer,
                          bool paIsPlug,
                          EMGMResponse &paErrorMSG);

  CIEC_ANY *
  createDataTypeInstance(CStringDictionary::TStringId paDTNameId, TForteByte *paDataBuf, EMGMResponse &paErrorMSG);

  CFBTypeEntry *getFBTypeEntry(CStringDictionary::TStringId paTypeNameId);
  CAdapterTypeEntry *getAdapterTypeEntry(CStringDictionary::TStringId paTypeNameId);
  CGlobalConstEntry *getGlobalConstTypeEntry(CStringDictionary::TStringId paTypeNameId);

  const std::vector<CFBTypeEntry *> &getFBTypeEntries();
  const std::vector<CAdapterTypeEntry *> &getAdapterTypeEntries();
  const std::vector<CDataTypeEntry *> &getDataTypeEntries();
  const std::vector<CGlobalConstEntry *> &getGlobalConstEntries();

  /*!\brief Function to create an data type instance of given type
   *
   * @param paDataTypeIds pointer to the data type ids. If the datatype
   *        is an Array to more values are taken from the array. If the given
   *        type is Any 0 is returned as necessary for maintaining the FB's interface.
   *        The functions puts the pointer in the datatype array to the next data point's id.
   * @param paDataBuf pointer to the data buffer which should be used by the data type to create
   * @return on success... pointer to the datatype instance
   *         on error... 0
   */
  CIEC_ANY *createDataPoint(const CStringDictionary::TStringId *&paDataTypeIds, TForteByte *&paDataBuf);
} // namespace forte::core
