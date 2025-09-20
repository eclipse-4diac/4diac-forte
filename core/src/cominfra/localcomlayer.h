/*******************************************************************************
 * Copyright (c) 2011-2014 fortiss and TU Wien ACIN.
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure
 *    Martin Jobst - account for new FB layout and varying data type size
 *******************************************************************************/
#ifndef LOCALCOMLAYER_H_
#define LOCALCOMLAYER_H_

#include "forte/cominfra/comlayer.h"
#include "forte/stringid.h"
#include "forte/arch/forte_sync.h"
#include <vector>

namespace forte {
  class CIEC_ANY;

  namespace com_infra {

    class CLocalComLayer : public CComLayer {

      public:
        CLocalComLayer(CComLayer *paUpperLayer, CBaseCommFB *paFB);
        ~CLocalComLayer() override;

        EComResponse sendData(void *paData, unsigned int paSize) override;
        EComResponse recvData(const void *, unsigned int) override {
          return e_ProcessDataOk;
        }

        EComResponse processInterrupt() override {
          return e_ProcessDataOk;
        }

      protected:
        virtual void setRDs(CBaseCommFB &paSubl, CIEC_ANY **paSDs, TPortId paNumSDs);

        class CLocalCommGroup {
          public:
            using TLocalComLayerList = std::vector<CLocalComLayer *>;
            using TLocalComDataTypeList = std::vector<StringId>;

            explicit CLocalCommGroup(StringId paGroupName, TLocalComDataTypeList paDataTypes) :
                mGroupName(paGroupName),
                mPublList(),
                mSublList(),
                mDataTypes(paDataTypes) {
            }

            CLocalCommGroup(const CLocalCommGroup &paLocalCommGroup) :
                mGroupName(paLocalCommGroup.mGroupName),
                mPublList(paLocalCommGroup.mPublList),
                mSublList(paLocalCommGroup.mSublList),
                mDataTypes(paLocalCommGroup.mDataTypes) {
            }

            ~CLocalCommGroup() = default;

            StringId mGroupName;
            TLocalComLayerList mPublList;
            TLocalComLayerList mSublList;
            TLocalComDataTypeList mDataTypes;
        };

        class CLocalCommGroupsManager {

          public:
            bool registerPubl(const StringId paGroupID, CLocalComLayer *paLayer);
            bool registerPubl(const StringId paGroupID,
                              CLocalComLayer *paLayer,
                              CIEC_ANY **paDataPins,
                              TPortId paNumDataPins);
            void unregisterPubl(const StringId paGroupID, CLocalComLayer *paLayer);

            bool registerSubl(const StringId paGroupID, CLocalComLayer *paLayer);
            void unregisterSubl(const StringId paGroupID, CLocalComLayer *paLayer);

            CLocalCommGroup *getComGroup(const StringId paGroupID);

          private:
            using TLocalCommGroupList = std::vector<CLocalCommGroup>;

            CLocalCommGroupsManager() = default;

            TLocalCommGroupList::iterator getLocalCommGroupIterator(StringId paID);

            CLocalCommGroup *findOrCreateLocalCommGroup(StringId paID, CIEC_ANY **paDataPins, TPortId paNumDataPins);
            void checkForGroupRemoval(TLocalCommGroupList::iterator comGroupIt);

            bool isGroupIteratorForGroup(TLocalCommGroupList::iterator iter, StringId paID) {
              return (iter != mLocalCommGroups.end() && iter->mGroupName == paID);
            }

            static void removeListEntry(CLocalCommGroup::TLocalComLayerList &paComLayerList, CLocalComLayer *paLayer);
            static CLocalCommGroup::TLocalComDataTypeList buildDataTypeList(CIEC_ANY **paDataPins,
                                                                            TPortId paNumDataPins);
            static bool checkDataTypes(const CLocalCommGroup &group, CIEC_ANY **paDataPins, TPortId paNumDataPins);

            /*!\brief The Sync object used locking the access to the internal used datastructures
             */
            arch::CSyncObject mSync;

            TLocalCommGroupList mLocalCommGroups;

            friend class CLocalComLayer;

          public:
            CLocalCommGroupsManager(const CLocalCommGroupsManager &) = delete;
            CLocalCommGroupsManager &operator=(const CLocalCommGroupsManager &) = delete;
        };

        static CLocalCommGroupsManager &getLocalCommGroupsManager() {
          return smLocalCommGroupsManager;
        }

        StringId mGroupID;

      private:
        static CLocalCommGroupsManager smLocalCommGroupsManager;

        EComResponse openConnection(char *const paLayerParameter) override;
        void closeConnection() override;
    };
  } // namespace com_infra

} // namespace forte

#endif /* LOCALCOMLAYER_H_ */
