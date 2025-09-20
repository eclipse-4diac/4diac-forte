/*******************************************************************************
 * Copyright (c) 2005, 2025 ACIN, Profactor GmbH, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Rene Smodic, Alois Zoitl, Thomas Strasser, Martin Melik Merkumians,
 *   Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *   Martin Erich Jobst
 *    - modernize C++
 *******************************************************************************/

#pragma once

#include <cstddef>
#include <cstdint>

/***************************************************************************
 type definitions
 ***************************************************************************/
namespace forte {
  using TForteByte = std::uint8_t; //!< 8 bit unsigned
  using TForteWord = std::uint16_t; //!< 16 bit unsigned
  using TForteDWord = std::uint32_t; //!< 32 bit unsigned
  using TForteLWord = std::uint64_t; //!< 64 bit unsigned

  using TForteInt8 = std::int8_t; //!< 8 bit signed
  using TForteInt16 = std::int16_t; //!< 16 bit signed
  using TForteInt32 = std::int32_t; //!< 32 bit signed
  using TForteInt64 = std::int64_t; //!< 64 bit signed

  using TForteUInt8 = std::uint8_t; //!< 8 bit unsigned
  using TForteUInt16 = std::uint16_t; //!< 16 bit unsigned
  using TForteUInt32 = std::uint32_t; //!< 32 bit unsigned
  using TForteUInt64 = std::uint64_t; //!< 64 bit unsigned

  using TForteChar = unsigned char; //!< 8 bit character
  using TForteWChar = char16_t; //!< 16 bit character

  using TForteFloat = float; //!< single precision IEEE float (32 Bit)
  using TForteDFloat = double; //!< double precision IEEE float (64 bit)

  //! This value holds the length an identifier may have on order that it is
  //! unique. Identifiers may be FB-Type Names, FB-Names, var names,...
  constexpr unsigned int cgIdentifierLength = 32;

  //! Type to hold an identifier during runtime. The +1 is for holding a trailing \\0
  using TIdentifier = char[cgIdentifierLength + 1];

  //! Type for holding an optimized FORTE internal port number.
  using TPortId = std::size_t;

  //! constant defining the max possible number of an FB Port (event or data, in or output)
  constexpr TPortId cgInvalidPortId = 65535;
} // namespace forte
