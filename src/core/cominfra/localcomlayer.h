/*******************************************************************************
  * Copyright (c) 2011-2014 fortiss and TU Wien ACIN.
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl - initial implementation and rework communication infrastructure
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
        virtual ~CLocalComLayer();

        virtual EComResponse sendData(void *pa_pvData, unsigned int pa_unSize);
        virtual EComResponse recvData(const void *, unsigned int ){
          return e_ProcessDataOk;
        }

        virtual EComResponse processInterrupt(){
          return e_ProcessDataOk;
        }

      private:
        virtual EComResponse openConnection(char *pa_acLayerParameter);
        virtual void closeConnection();
        void setRDs(CLocalComLayer *pa_poSublLayer, CIEC_ANY *pa_aSDs, unsigned int pa_unNumSDs);

        class SLocalCommGroup {
          public:
            explicit SLocalCommGroup(CStringDictionary::TStringId pa_nGroupName) :
                m_nGroupName(pa_nGroupName), m_lPublList(), m_lSublList(){
            }

            SLocalCommGroup(const SLocalCommGroup& pa_roLocalCommGroup) :
                m_nGroupName(pa_roLocalCommGroup.m_nGroupName), m_lPublList(), m_lSublList(){
            }

            ~SLocalCommGroup(){
            }

            CStringDictionary::TStringId m_nGroupName;
            CSinglyLinkedList<CLocalComLayer*> m_lPublList;
            CSinglyLinkedList<CLocalComLayer*> m_lSublList;
        };

        class CLocalCommGroupsManager{

          public:

            SLocalCommGroup* registerPubl(const CStringDictionary::TStringId pa_nID, CLocalComLayer *pa_poLayer);
            void unregisterPubl(SLocalCommGroup *pa_poGroup, CLocalComLayer *pa_poLayer);

            SLocalCommGroup* registerSubl(const CStringDictionary::TStringId pa_nID, CLocalComLayer *pa_poLayer);
            void unregisterSubl(SLocalCommGroup *pa_poGroup, CLocalComLayer *pa_poLayer);

          private:
            CLocalCommGroupsManager(){};

            SLocalCommGroup* findLocalCommGroup(CStringDictionary::TStringId pa_nID);
            SLocalCommGroup* createLocalCommGroup(CStringDictionary::TStringId pa_nID);
            void removeCommGroup(SLocalCommGroup *pa_poGroup);

            static void removeListEntry(CSinglyLinkedList<CLocalComLayer*>  &pa_rlstList, CLocalComLayer *pa_poLayer);

            /*!\brief The Sync object used locking the access to the internal used datastructures
             */
            CSyncObject m_oSync;

            CSinglyLinkedList<SLocalCommGroup> m_lstLocalCommGroups;

            friend class CLocalComLayer;

            //not to be implemented
            CLocalCommGroupsManager(const CLocalCommGroupsManager &);
            CLocalCommGroupsManager &operator =(const CLocalCommGroupsManager&);
        };

        static CLocalCommGroupsManager sm_oLocalCommGroupsManager;


        SLocalCommGroup *m_poLocalCommGroup;
    };
  }

}

#endif /* LOCALCOMLAYER_H_ */
