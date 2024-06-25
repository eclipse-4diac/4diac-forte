/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Mario Kastner, Alois Zoitl - initial implementation
 *******************************************************************************/

#pragma once

#include "localcomlayer.h"
#include <vector>


namespace forte {

  namespace com_infra {


    /*! A local communication layer that allows to write to a single struct member of a local group.
     *
     * The boundary conditions are that the local group publishing and subscribing is with a single
     * RD/SD of type struct.
     *
     * This layer can only be used with PUBLISH_1 FBs.
     *
     * To use it a ID param with the following structure needs to be provided:
     *    structmemb[localgroupname;structtype;structmembername]
     *
     * It is also possible to wirte to children of the structs using:
     *      structmemb[localgroupname;structtype;structmembername.structchildmembername]
     *
     * or writing to array elements using:
     *      structmemb[localgroupname;structtype;structmembername(10).structchildmembername]
     *      Note: Please use 'array(0)' to access instead of 'array[0]'!!
     *
     *
     *  - localgroupname:  is the local group this local com layer should attach to. If this is the
     *                     first block for this group an according group is created.
     *  - structtype: the type of the struct this layer expects in the local group. If the local
     *                group is existing it will be checked if the local group is of size one and
     *                that the data value exchanged is of this size. If no local group is existing
     *                a group with this struct type is created.
     *  - structmembername: the name of a struct member of the above struct type. This is the struct
     *                      member this layer will write to.
     */
    class CStructMemberLocalComLayer : public CLocalComLayer{

      public:
        CStructMemberLocalComLayer(CComLayer *paUpperLayer, CBaseCommFB *paFB);

      protected:
        void setRDs(forte::com_infra::CBaseCommFB &paSubl, CIEC_ANY **paSDs, TPortId paNumSDs);

      private:
        using TTargetStructIndexList = std::vector<TForteInt16>;
        static constexpr size_t scmNumLayerParameters = 3;
        TTargetStructIndexList mIndexList;

        EComResponse openConnection(char *paLayerParameter) override;
        CIEC_ANY* getTargetByIndex(CIEC_STRUCT* paRoot, TTargetStructIndexList &paIndexList);

        TTargetStructIndexList buildIndexList(CIEC_ANY* paRoot, const char *paNestedStructString);
        bool parseArrayIndexFromString(const char *paNestedStructString, CIEC_INT& targetIndex);

        enum EComStringIndex {
          e_LOCALGROUPNAME,
          e_STRUCTTYPE,
          e_STRUCTMEMBERNAME
        };
    };
  }
}
