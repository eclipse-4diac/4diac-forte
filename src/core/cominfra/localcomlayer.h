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

#include "comlayer.h"
#include "../stringdict.h"
#include "../fortelist.h"
#include <forte_sync.h>
#include <vector>

class CIEC_ANY;

namespace forte {

  namespace com_infra {

    class CLocalComLayer : public CComLayer{

      public:
        CLocalComLayer(CComLayer* paUpperLayer, CBaseCommFB * paFB);
        ~CLocalComLayer() override;

        EComResponse sendData(void *paData, unsigned int paSize) override;
        EComResponse recvData(const void *, unsigned int ) override {
          return e_ProcessDataOk;
        }

        EComResponse processInterrupt() override {
          return e_ProcessDataOk;
        }

      protected:
        class CLocalCommGroup {
          public:
            explicit CLocalCommGroup(CStringDictionary::TStringId paGroupName) :
                mGroupName(paGroupName), mPublList(), mSublList(){
            }

            CLocalCommGroup(const CLocalCommGroup& paLocalCommGroup) :
                mGroupName(paLocalCommGroup.mGroupName), mPublList(), mSublList(){
            }

            ~CLocalCommGroup() = default;

            CStringDictionary::TStringId mGroupName;
            CSinglyLinkedList<CLocalComLayer*> mPublList;
            CSinglyLinkedList<CLocalComLayer*> mSublList;
        };

        class CLocalCommGroupsManager{

          public:

            CLocalCommGroup* registerPubl(const CStringDictionary::TStringId paID, CLocalComLayer *paLayer);
            void unregisterPubl(CLocalCommGroup *paGroup, CLocalComLayer *paLayer);

            CLocalCommGroup* registerSubl(const CStringDictionary::TStringId paID, CLocalComLayer *paLayer);
            void unregisterSubl(CLocalCommGroup *paGroup, CLocalComLayer *paLayer);

          private:
            using TLocalCommGroupList = std::vector<CLocalCommGroup>;

            CLocalCommGroupsManager() = default;

            TLocalCommGroupList::iterator getLocalCommGroupIterator(CStringDictionary::TStringId paID);

            CLocalCommGroup* findOrCreateLocalCommGroup(CStringDictionary::TStringId paID);
            void removeCommGroup(CLocalCommGroup &paGroup);

            bool isGroupIteratorForGroup(TLocalCommGroupList::iterator iter, CStringDictionary::TStringId paID){
              return (iter != mLocalCommGroups.end() && iter->mGroupName == paID);
            }

            static void removeListEntry(CSinglyLinkedList<CLocalComLayer*>  &pa_rlstList, CLocalComLayer *paLayer);


            /*!\brief The Sync object used locking the access to the internal used datastructures
             */
            CSyncObject mSync;


            TLocalCommGroupList mLocalCommGroups;

            friend class CLocalComLayer;

          public:
            CLocalCommGroupsManager(const CLocalCommGroupsManager &) = delete;
            CLocalCommGroupsManager &operator =(const CLocalCommGroupsManager&) = delete;
        };


        static CLocalCommGroupsManager& getLocalCommGroupsManager(){
          return smLocalCommGroupsManager;
        }

        CLocalCommGroup *mLocalCommGroup;

      private:
        static CLocalCommGroupsManager smLocalCommGroupsManager;

        EComResponse openConnection(char *const paLayerParameter) override;
        void closeConnection() override;
        void setRDs(CLocalComLayer *paSublLayer, CIEC_ANY **paSDs, TPortId paNumSDs);
    };
  }

}

#endif /* LOCALCOMLAYER_H_ */
