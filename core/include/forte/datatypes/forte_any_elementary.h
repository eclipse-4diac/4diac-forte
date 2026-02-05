/*******************************************************************************
 * Copyright (c) 2005, 2026 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH,
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Martin Melik Merkumians, Alois Zoitl,
 *      Monika Wenger, Stansilav Meduna
 *                - initial implementation and rework communication infrastructure
 *   Markus Meingast, Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"
#include <cstring>
#include <bit>

namespace forte {
  /*!\ingroup COREDTS IIEC_ANY_ELEMENTARY represents the elementary data types according to
   *  IEC 61131.
   */
  class CIEC_ANY_ELEMENTARY : public CIEC_ANY {
    public:
      constexpr CIEC_ANY_ELEMENTARY(const CIEC_ANY_ELEMENTARY &paVal) : CIEC_ANY() {
        setLargestUInt(paVal.getLargestUInt());
      }

      ~CIEC_ANY_ELEMENTARY() override = default;

      EDataTypeID getDataTypeID() const override {
        return e_ANY;
      }

      void toString(std::string &paTargetBuf) const override;
      int fromString(const char *paValue) override;
      void setValue(const CIEC_ANY &paValue) override;

      constexpr TLargestUIntValueType getLargestUInt() const override {
        const TForteByte *data = getConstDataPtr();
        if (!data) {
          return 0;
        }
        switch (getDataTypeID()) {
          case e_BOOL: return (*reinterpret_cast<const TForteByte *>(data) != 0) ? 1 : 0;
          case e_SINT: return static_cast<TLargestUIntValueType>(*reinterpret_cast<const TForteInt8 *>(data));
          case e_USINT: return *reinterpret_cast<const TForteUInt8 *>(data);
          case e_BYTE: return *reinterpret_cast<const TForteByte *>(data);
          case e_INT: return static_cast<TLargestUIntValueType>(*reinterpret_cast<const TForteInt16 *>(data));
          case e_UINT: return *reinterpret_cast<const TForteUInt16 *>(data);
          case e_WORD: return *reinterpret_cast<const TForteWord *>(data);
          case e_DINT: return static_cast<TLargestUIntValueType>(*reinterpret_cast<const TForteInt32 *>(data));
          case e_UDINT: return *reinterpret_cast<const TForteUInt32 *>(data);
          case e_DWORD: return *reinterpret_cast<const TForteDWord *>(data);
          case e_LINT: return static_cast<TLargestUIntValueType>(*reinterpret_cast<const TForteInt64 *>(data));
          case e_ULINT: return *reinterpret_cast<const TForteUInt64 *>(data);
          case e_LWORD: return *reinterpret_cast<const TForteLWord *>(data);
          case e_REAL: return std::bit_cast<TForteUInt32>(*reinterpret_cast<const TForteFloat *>(data));
          case e_LREAL: return std::bit_cast<TForteUInt64>(*reinterpret_cast<const TForteDFloat *>(data));
          case e_TIME:
          case e_LTIME: return static_cast<TLargestUIntValueType>(*reinterpret_cast<const TForteInt64 *>(data));
          case e_DATE:
          case e_TIME_OF_DAY:
          case e_DATE_AND_TIME:
          case e_LDATE:
          case e_LTIME_OF_DAY:
          case e_LDATE_AND_TIME: return *reinterpret_cast<const TForteUInt64 *>(data);
          case e_CHAR: return static_cast<TLargestUIntValueType>(*reinterpret_cast<const TForteChar *>(data));
          case e_WCHAR: return static_cast<TLargestUIntValueType>(*reinterpret_cast<const TForteWChar *>(data));
          default: return 0;
        }
      }

      constexpr void setLargestUInt(TLargestUIntValueType paVal) override {
        TForteByte *data = getDataPtr();
        if (!data) {
          return;
        }
        switch (getDataTypeID()) {
          case e_BOOL: *reinterpret_cast<TForteByte *>(data) = (paVal != 0) ? 1 : 0; break;
          case e_SINT: *reinterpret_cast<TForteInt8 *>(data) = static_cast<TForteInt8>(paVal); break;
          case e_USINT: *reinterpret_cast<TForteUInt8 *>(data) = static_cast<TForteUInt8>(paVal); break;
          case e_BYTE: *reinterpret_cast<TForteByte *>(data) = static_cast<TForteByte>(paVal); break;
          case e_INT: *reinterpret_cast<TForteInt16 *>(data) = static_cast<TForteInt16>(paVal); break;
          case e_UINT: *reinterpret_cast<TForteUInt16 *>(data) = static_cast<TForteUInt16>(paVal); break;
          case e_WORD: *reinterpret_cast<TForteWord *>(data) = static_cast<TForteWord>(paVal); break;
          case e_DINT: *reinterpret_cast<TForteInt32 *>(data) = static_cast<TForteInt32>(paVal); break;
          case e_UDINT: *reinterpret_cast<TForteUInt32 *>(data) = static_cast<TForteUInt32>(paVal); break;
          case e_DWORD: *reinterpret_cast<TForteDWord *>(data) = static_cast<TForteDWord>(paVal); break;
          case e_LINT: *reinterpret_cast<TForteInt64 *>(data) = static_cast<TForteInt64>(paVal); break;
          case e_ULINT: *reinterpret_cast<TForteUInt64 *>(data) = static_cast<TForteUInt64>(paVal); break;
          case e_LWORD: *reinterpret_cast<TForteLWord *>(data) = static_cast<TForteLWord>(paVal); break;
          case e_REAL:
            *reinterpret_cast<TForteFloat *>(data) = std::bit_cast<TForteFloat>(static_cast<TForteUInt32>(paVal));
            break;
          case e_LREAL:
            *reinterpret_cast<TForteDFloat *>(data) = std::bit_cast<TForteDFloat>(static_cast<TForteUInt64>(paVal));
            break;
          case e_TIME:
          case e_LTIME: *reinterpret_cast<TForteInt64 *>(data) = static_cast<TForteInt64>(paVal); break;
          case e_DATE:
          case e_TIME_OF_DAY:
          case e_DATE_AND_TIME:
          case e_LDATE:
          case e_LTIME_OF_DAY:
          case e_LDATE_AND_TIME: *reinterpret_cast<TForteUInt64 *>(data) = paVal; break;
          case e_CHAR: *reinterpret_cast<TForteChar *>(data) = static_cast<TForteChar>(paVal); break;
          case e_WCHAR: *reinterpret_cast<TForteWChar *>(data) = static_cast<TForteWChar>(paVal); break;
          default: break;
        }
      }

      constexpr TLargestIntValueType getLargestInt() const override {
        const TForteByte *data = getConstDataPtr();
        if (!data) {
          return 0;
        }
        switch (getDataTypeID()) {
          case e_BOOL: return (*reinterpret_cast<const TForteByte *>(data) != 0) ? 1 : 0;
          case e_SINT: return *reinterpret_cast<const TForteInt8 *>(data);
          case e_USINT: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteUInt8 *>(data));
          case e_BYTE: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteByte *>(data));
          case e_INT: return *reinterpret_cast<const TForteInt16 *>(data);
          case e_UINT: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteUInt16 *>(data));
          case e_WORD: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteWord *>(data));
          case e_DINT: return *reinterpret_cast<const TForteInt32 *>(data);
          case e_UDINT: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteUInt32 *>(data));
          case e_DWORD: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteDWord *>(data));
          case e_LINT: return *reinterpret_cast<const TForteInt64 *>(data);
          case e_ULINT: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteUInt64 *>(data));
          case e_LWORD: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteLWord *>(data));
          case e_REAL:
            return static_cast<TLargestIntValueType>(
                std::bit_cast<TForteUInt32>(*reinterpret_cast<const TForteFloat *>(data)));
          case e_LREAL:
            return static_cast<TLargestIntValueType>(
                std::bit_cast<TForteUInt64>(*reinterpret_cast<const TForteDFloat *>(data)));
          case e_TIME:
          case e_LTIME: return *reinterpret_cast<const TForteInt64 *>(data);
          case e_DATE:
          case e_TIME_OF_DAY:
          case e_DATE_AND_TIME:
          case e_LDATE:
          case e_LTIME_OF_DAY:
          case e_LDATE_AND_TIME:
            return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteUInt64 *>(data));
          case e_CHAR: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteChar *>(data));
          case e_WCHAR: return static_cast<TLargestIntValueType>(*reinterpret_cast<const TForteWChar *>(data));
          default: return 0;
        }
      }

      constexpr void setLargestInt(TLargestIntValueType paVal) override {
        TForteByte *data = getDataPtr();
        if (!data) {
          return;
        }
        switch (getDataTypeID()) {
          case e_BOOL: *reinterpret_cast<TForteByte *>(data) = (paVal != 0) ? 1 : 0; break;
          case e_SINT: *reinterpret_cast<TForteInt8 *>(data) = static_cast<TForteInt8>(paVal); break;
          case e_USINT: *reinterpret_cast<TForteUInt8 *>(data) = static_cast<TForteUInt8>(paVal); break;
          case e_BYTE: *reinterpret_cast<TForteByte *>(data) = static_cast<TForteByte>(paVal); break;
          case e_INT: *reinterpret_cast<TForteInt16 *>(data) = static_cast<TForteInt16>(paVal); break;
          case e_UINT: *reinterpret_cast<TForteUInt16 *>(data) = static_cast<TForteUInt16>(paVal); break;
          case e_WORD: *reinterpret_cast<TForteWord *>(data) = static_cast<TForteWord>(paVal); break;
          case e_DINT: *reinterpret_cast<TForteInt32 *>(data) = static_cast<TForteInt32>(paVal); break;
          case e_UDINT: *reinterpret_cast<TForteUInt32 *>(data) = static_cast<TForteUInt32>(paVal); break;
          case e_DWORD: *reinterpret_cast<TForteDWord *>(data) = static_cast<TForteDWord>(paVal); break;
          case e_LINT: *reinterpret_cast<TForteInt64 *>(data) = static_cast<TForteInt64>(paVal); break;
          case e_ULINT: *reinterpret_cast<TForteUInt64 *>(data) = static_cast<TForteUInt64>(paVal); break;
          case e_LWORD: *reinterpret_cast<TForteLWord *>(data) = static_cast<TForteLWord>(paVal); break;
          case e_REAL:
            *reinterpret_cast<TForteFloat *>(data) = std::bit_cast<TForteFloat>(static_cast<TForteUInt32>(paVal));
            break;
          case e_LREAL:
            *reinterpret_cast<TForteDFloat *>(data) = std::bit_cast<TForteDFloat>(static_cast<TForteUInt64>(paVal));
            break;
          case e_TIME:
          case e_LTIME: *reinterpret_cast<TForteInt64 *>(data) = paVal; break;
          case e_DATE:
          case e_TIME_OF_DAY:
          case e_DATE_AND_TIME:
          case e_LDATE:
          case e_LTIME_OF_DAY:
          case e_LDATE_AND_TIME: *reinterpret_cast<TForteUInt64 *>(data) = static_cast<TForteUInt64>(paVal); break;
          case e_CHAR: *reinterpret_cast<TForteChar *>(data) = static_cast<TForteChar>(paVal); break;
          case e_WCHAR: *reinterpret_cast<TForteWChar *>(data) = static_cast<TForteWChar>(paVal); break;
          default: break;
        }
      }

      static EDataTypeID getElementaryDataTypeId(StringId paTypeNameId);

    protected:
      constexpr CIEC_ANY_ELEMENTARY() = default;

      constexpr void setTBOOL8(bool src) {
        setLargestUInt(src ? 1 : 0);
      }
      constexpr void setTUINT32(TForteUInt32 src) {
        setLargestUInt(src);
      }
      constexpr void setTUINT16(TForteUInt16 src) {
        setLargestUInt(src);
      }
      constexpr void setTUINT8(TForteUInt8 src) {
        setLargestUInt(src);
      }
      constexpr void setTINT32(TForteInt32 src) {
        setLargestInt(src);
      }
      constexpr void setTINT16(TForteInt16 src) {
        setLargestInt(src);
      }
      constexpr void setTINT8(TForteInt8 src) {
        setLargestInt(src);
      }
      constexpr void setChar(TForteChar src) {
        setLargestUInt(src);
      }
      constexpr void setChar16(TForteWChar src) {
        setLargestUInt(src);
      }
      constexpr void setTFLOAT(TForteFloat src) {
        setLargestUInt(std::bit_cast<TForteUInt32>(src));
      }
      constexpr void setTDFLOAT(TForteDFloat src) {
        setLargestUInt(std::bit_cast<TForteUInt64>(src));
      }
      constexpr void setTUINT64(TForteUInt64 src) {
        setLargestUInt(src);
      }
      constexpr void setTINT64(TForteInt64 src) {
        setLargestInt(src);
      }

      constexpr bool getTBOOL8() const {
        return getLargestUInt() != 0;
      }
      constexpr TForteUInt32 getTUINT32() const {
        return static_cast<TForteUInt32>(getLargestUInt());
      }
      constexpr TForteUInt16 getTUINT16() const {
        return static_cast<TForteUInt16>(getLargestUInt());
      }
      constexpr TForteUInt8 getTUINT8() const {
        return static_cast<TForteUInt8>(getLargestUInt());
      }
      constexpr TForteInt32 getTINT32() const {
        return static_cast<TForteInt32>(getLargestInt());
      }
      constexpr TForteInt16 getTINT16() const {
        return static_cast<TForteInt16>(getLargestInt());
      }
      constexpr TForteInt8 getTINT8() const {
        return static_cast<TForteInt8>(getLargestInt());
      }
      constexpr TForteChar getChar8() const {
        return static_cast<TForteChar>(getLargestUInt());
      }
      constexpr TForteWChar getChar16() const {
        return static_cast<TForteWChar>(getLargestUInt());
      }
      constexpr TForteUInt64 getTUINT64() const {
        return getLargestUInt();
      }
      constexpr TForteInt64 getTINT64() const {
        return getLargestInt();
      }
      constexpr TForteFloat getTFLOAT() const {
        return std::bit_cast<TForteFloat>(static_cast<TForteUInt32>(getLargestUInt()));
      }
      constexpr TForteDFloat getTDFLOAT() const {
        return std::bit_cast<TForteDFloat>(getLargestUInt());
      }

    private:
      bool isTypeSpecifier(const char *paValue, const char *paHashPosition) const;
      bool isCastable(StringId paTypeNameId) const;
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_ELEMENTARY> : std::true_type {};
} // namespace forte
