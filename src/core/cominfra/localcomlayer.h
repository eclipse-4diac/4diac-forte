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

class CIEC_ANY;

namespace forte {

  namespace com_infra {

    class CLocalComLayer : public CComLayer{

      public:
        CLocalComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poFB);
        ~CLocalComLayer() override;

        EComResponse sendData(void *pa_pvData, unsigned int pa_unSize) override;
        EComResponse recvData(const void *, unsigned int ) override {
          return e_ProcessDataOk;
        }

        EComResponse processInterrupt() override {
          return e_ProcessDataOk;
        }

      private:
        EComResponse openConnection(char *pa_acLayerParameter) override;
        void closeConnection() override;
        void setRDs(CLocalComLayer *pa_poSublLayer, CIEC_ANY **pa_aSDs, unsigned int pa_unNumSDs);

        class CLocalCommGroup {
          public:
            explicit CLocalCommGroup(CStringDictionary::TStringId pa_nGroupName) :
                m_nGroupName(pa_nGroupName), m_lPublList(), m_lSublList(){
            }

            CLocalCommGroup(const CLocalCommGroup& pa_roLocalCommGroup) :
                m_nGroupName(pa_roLocalCommGroup.m_nGroupName), m_lPublList(), m_lSublList(){
            }

            ~CLocalCommGroup() = default;

            CStringDictionary::TStringId m_nGroupName;
            CSinglyLinkedList<CLocalComLayer*> m_lPublList;
            CSinglyLinkedList<CLocalComLayer*> m_lSublList;
        };

        class CLocalCommGroupsManager{

          public:

            CLocalCommGroup* registerPubl(const CStringDictionary::TStringId pa_nID, CLocalComLayer *pa_poLayer);
            void unregisterPubl(CLocalCommGroup *pa_poGroup, CLocalComLayer *pa_poLayer);

            CLocalCommGroup* registerSubl(const CStringDictionary::TStringId pa_nID, CLocalComLayer *pa_poLayer);
            void unregisterSubl(CLocalCommGroup *pa_poGroup, CLocalComLayer *pa_poLayer);

          private:
            CLocalCommGroupsManager() = default;

            CLocalCommGroup* findLocalCommGroup(CStringDictionary::TStringId pa_nID);
            CLocalCommGroup* createLocalCommGroup(CStringDictionary::TStringId pa_nID);
            void removeCommGroup(CLocalCommGroup *pa_poGroup);

            static void removeListEntry(CSinglyLinkedList<CLocalComLayer*>  &pa_rlstList, CLocalComLayer *pa_poLayer);

            /*!\brief The Sync object used locking the access to the internal used datastructures
             */
            CSyncObject m_oSync;

            CSinglyLinkedList<CLocalCommGroup> m_lstLocalCommGroups;

            friend class CLocalComLayer;

          public:
            CLocalCommGroupsManager(const CLocalCommGroupsManager &) = delete;
            CLocalCommGroupsManager &operator =(const CLocalCommGroupsManager&) = delete;
        };

        static CLocalCommGroupsManager sm_oLocalCommGroupsManager;


        CLocalCommGroup *m_poLocalCommGroup;
    };
  }

}

#endif /* LOCALCOMLAYER_H_ */
