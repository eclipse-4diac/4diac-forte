/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
 *    Michael Hofmann
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include <memory>
#include <span>
#include <type_traits>

#include "forte/datatype.h"
#include "forte/mgmcmd.h"
#include "forte/stringid.h"

namespace forte {
  // forward declaration of a few classes to reduce include file dependencies
  class CFunctionBlock;

  class CConnectionPoint {
    public:
      CConnectionPoint(CFunctionBlock &paFB, const TPortId paPortId) : mFB(&paFB), mPortId(paPortId) {
      }

      CConnectionPoint() = default;

      bool operator==(const CConnectionPoint &paRight) const {
        return ((mFB == paRight.mFB) && (mPortId == paRight.mPortId));
      }

      bool operator!=(const CConnectionPoint &paRight) const {
        return !(*this == paRight);
      }

      CFunctionBlock &getFB() const {
        return *mFB;
      }

      TPortId getPortId() const {
        return mPortId;
      }

    private:
      CFunctionBlock *mFB;
      TPortId mPortId;
  };
  static_assert(std::is_trivial_v<CConnectionPoint>);

  /*!\ingroup CORE \brief Base class for handling a connection.
   */

  class CConnection {
    public:
      struct DelegatingDeleter final {
          void operator()(const CConnection *paConnection) const {
            if (paConnection->isDelegating()) {
              delete paConnection;
            }
          }
      };

      using Wrapper = std::unique_ptr<CConnection, DelegatingDeleter>;

      template<typename T, typename... Args>
      Wrapper make_delegating(Args &&...paArgs) {
        return Wrapper(new T(std::forward<Args>(paArgs)...));
      }

      CConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId) : mSourceId(paSrcFB, paSrcPortId) {
      }

      virtual ~CConnection() = default;

      /*! \brief Establishes a connection.
       *
       * With this command the connection is created and the FBs are set to appropriate states.
       * As connections are stored at the function block outputs this mainly affects the destination of
       * the connection.
       *
       * A child needs to implement this function!
       *
       * \param paDstFB     the destination FB of this connection
       * \param paDestPortNameId the input port name to which this connection should be connected to
       * \return can be the following response:
       *     - Ready... creation went well.
       *     - NoSuchObject... The destination is not a valid input.
       *     - InvalidState... The specified connection already exists.
       */
      virtual EMGMResponse connect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) = 0;

      /*!\brief establish an event connection of a CFB to an event output of the CFB.
       *
       */
      virtual EMGMResponse connectToCFBInterface(CFunctionBlock &paDstFB,
                                                 std::span<const StringId> paDstPortNameId) = 0;

      /*! \brief Disconnects the connection.
       *
       * With this command the connection is removed and the FBs are set to appropriate states.
       * This mainly effects the destination of the connection as the source point is the storage
       * for the connection object
       *
       * A child needs to implement this function!
       *
       * \param paDstFB     the destination FB of this connection
       * \param paDestPortNameId the input port name to from which this connection should be removed
       * \return can be the following response:
       *     - Ready... creation went well.
       *     - NoSuchObject... The destination is not a valid input.
       *     - InvalidState... this connection is not connected to the destination
       */
      virtual EMGMResponse disconnect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) = 0;

      /*!
       * \brief Get a delegating connection for the given name
       * @param paSrcNameList The name
       * @return a delegating connection for the name, an empty wrapper if there is no such delegating connection,
       *          or this if the name list was empty
       */
      virtual Wrapper getDelegatingConnection(const std::span<const StringId> paSrcNameList) {
        if (paSrcNameList.empty()) {
          return Wrapper(this);
        }
        return {};
      }

      /*! \brief Get the source string of the connection
       */
      const CConnectionPoint &getSourceId() const {
        return mSourceId;
      }

      /*!
       * \brief Get the full-qualified source port name
       * @param paResult The result to append the source port name into
       */
      virtual void getSourcePortName(TNameIdentifier &paResult) const = 0;

      virtual bool isDelegating() const {
        return false;
      }

      virtual bool isInOut() const {
        return false;
      }

    protected:
      /*!\brief An identifier for the source of this connection
       *
       * The source is identified by a FB pointer and the port ID
       */
      CConnectionPoint mSourceId;
  };
} // namespace forte
