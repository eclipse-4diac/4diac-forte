/*************************************************************************
 * Copyright (c) 2026 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Mayr - initial implementation
 *******************************************************************************/

#include "jsonlayer.h"

#include "forte/cominfra/comlayersmanager.h"
#include "forte/cominfra/basecommfb.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"

#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_lint.h"

#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_ulint.h"

#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_struct.h"

#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lreal.h"

#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

#include "forte/datatypes/forte_char.h"
#include "forte/datatypes/forte_wchar.h"

#include "forte/util/parameterParser.h"

#include <nlohmann/json.hpp>

#include <algorithm>

using json = nlohmann::ordered_json;
using namespace forte::literals;

namespace forte {
  namespace com_infra {

    namespace {
      [[maybe_unused]]
      const ComLayerManager::EntryImpl<CJSONComLayer> entry("json"_STRID);
    }

    CJSONComLayer::CJSONComLayer(CComLayer *paUpperLayer, CBaseCommFB *paFB) : CComLayer(paUpperLayer, paFB) {
    }

    CJSONComLayer::~CJSONComLayer() = default;

    EComResponse CJSONComLayer::openConnection(char *paLayerParameter) {
      mFieldNames.clear();

      if (paLayerParameter != nullptr && paLayerParameter[0] != '\0') {
        constexpr unsigned int maxParameters = 10;
        forte::util::CParameterParser parser(paLayerParameter, ',', maxParameters);

        const unsigned int count = parser.parseParameters();

        for (unsigned int i = 0; i < count; ++i) {
          const char *parameter = parser[i];

          if (parameter != nullptr && parameter[0] != '\0') {
            mFieldNames.emplace_back(parameter);
          }
        }
      }

      return e_InitOk;
    }

    void CJSONComLayer::closeConnection() {
    }

    std::string CJSONComLayer::getFieldName(unsigned int paIndex) const {

      if (paIndex < mFieldNames.size()) {
        return mFieldNames[paIndex];
      }

      return "value" + std::to_string(paIndex);
    }

    EComResponse CJSONComLayer::sendData(void *paData, unsigned int paSize) {

      std::string result;

      if (!serialize(static_cast<const CIEC_ANY **>(paData), paSize, result)) {

        return e_ProcessDataDataTypeError;
      }

      return mBottomLayer->sendData((void *) result.data(), static_cast<unsigned int>(result.size()));
    }

    EComResponse CJSONComLayer::recvData(const void *paData, unsigned int paSize) {

      const std::string input(static_cast<const char *>(paData), paSize);

      return deserialize(input) ? e_ProcessDataOk : e_ProcessDataDataTypeError;
    }

    bool CJSONComLayer::serialize(const CIEC_ANY **paData, unsigned int paSize, std::string &paResult) {

      json output = json::object();
      for (unsigned int i = 0; i < paSize; i++) {
        if (!serializeValue(output, getFieldName(i), paData[i]->unwrap())) {
          return false;
        }
      }

      paResult = output.dump();

      return true;
    }

    bool CJSONComLayer::serializeValue(json &output, const std::string &name, const CIEC_ANY &value) {

      auto setValue = [&](auto val) {
        if (name.empty()) {
          output = val;
        } else {
          output[name] = val;
        }
      };

      switch (value.getDataTypeID()) {

        case CIEC_ANY::e_BOOL: setValue(static_cast<const CIEC_BOOL &>(value).operator bool()); break;
        case CIEC_ANY::e_BYTE: setValue(static_cast<TForteByte>(static_cast<const CIEC_BYTE &>(value))); break;
        case CIEC_ANY::e_WORD: setValue(static_cast<TForteWord>(static_cast<const CIEC_WORD &>(value))); break;
        case CIEC_ANY::e_DWORD: setValue(static_cast<TForteDWord>(static_cast<const CIEC_DWORD &>(value))); break;
        case CIEC_ANY::e_LWORD: setValue(static_cast<TForteLWord>(static_cast<const CIEC_LWORD &>(value))); break;

        case CIEC_ANY::e_SINT: setValue(static_cast<TForteInt8>(static_cast<const CIEC_SINT &>(value))); break;
        case CIEC_ANY::e_INT: setValue(static_cast<TForteInt16>(static_cast<const CIEC_INT &>(value))); break;
        case CIEC_ANY::e_DINT: setValue(static_cast<TForteInt32>(static_cast<const CIEC_DINT &>(value))); break;
        case CIEC_ANY::e_LINT: setValue(static_cast<TForteInt64>(static_cast<const CIEC_LINT &>(value))); break;

        case CIEC_ANY::e_USINT: setValue(static_cast<TForteUInt8>(static_cast<const CIEC_USINT &>(value))); break;
        case CIEC_ANY::e_UINT: setValue(static_cast<TForteUInt16>(static_cast<const CIEC_UINT &>(value))); break;
        case CIEC_ANY::e_UDINT: setValue(static_cast<TForteUInt32>(static_cast<const CIEC_UDINT &>(value))); break;
        case CIEC_ANY::e_ULINT: setValue(static_cast<TForteUInt64>(static_cast<const CIEC_ULINT &>(value))); break;

        case CIEC_ANY::e_REAL: setValue(static_cast<TForteFloat>(static_cast<const CIEC_REAL &>(value))); break;
        case CIEC_ANY::e_LREAL: setValue(static_cast<TForteDFloat>(static_cast<const CIEC_LREAL &>(value))); break;

        case CIEC_ANY::e_STRING: {
          const CIEC_STRING &str = static_cast<const CIEC_STRING &>(value);
          setValue(str.getStorage());
          break;
        }

        case CIEC_ANY::e_WSTRING: {
          std::string utf8;
          static_cast<const CIEC_WSTRING &>(value).toUTF8(utf8, false);
          setValue(utf8);
          break;
        }

        case CIEC_ANY::e_CHAR: {
          const CIEC_CHAR &c = static_cast<const CIEC_CHAR &>(value);
          std::string utf8;
          c.toString(utf8);
          setValue(utf8);
          break;
        }

        case CIEC_ANY::e_WCHAR: {
          TForteWChar wc = static_cast<TForteWChar>(static_cast<const CIEC_WCHAR &>(value));

          auto &converter = std::use_facet<std::codecvt<char16_t, char, std::mbstate_t>>(std::locale());

          std::mbstate_t mb{};
          char buffer[8];
          const char16_t *from_next;
          char *to_next;

          converter.out(mb, &wc, &wc + 1, from_next, buffer, buffer + sizeof(buffer), to_next);

          setValue(std::string(buffer, static_cast<size_t>(to_next - buffer)));
          break;
        }

        case CIEC_ANY::e_STRUCT: {
          const CIEC_STRUCT &structure = static_cast<const CIEC_STRUCT &>(value);
          json object;
          const StringId *names = structure.elementNames();
          for (size_t i = 0; i < structure.getStructSize(); i++) {
            std::string memberName;
            if (names) {
              memberName = std::string(names[i].get());
            } else {
              memberName = "member" + std::to_string(i);
            }

            if (!serializeValue(object, memberName, *structure.getMember(i))) {
              return false;
            }
          }

          setValue(object);

          break;
        }

        case CIEC_ANY::e_ARRAY: {
          const CIEC_ARRAY &array = static_cast<const CIEC_ARRAY &>(value);
          json outputArray = json::array();
          for (size_t i = 0; i < array.size(); i++) {
            json element;
            if (!serializeValue(element, "", array[array.getLowerBound() + static_cast<intmax_t>(i)])) {
              return false;
            }

            outputArray.push_back(element);
          }

          if (name.empty()) {
            output = outputArray;
          } else {
            output[name] = outputArray;
          }

          break;
        }

        default: return false;
      }

      return true;
    }

    bool CJSONComLayer::deserialize(const std::string &paInput) {

      json input = json::parse(paInput, nullptr, false);

      if (input.is_discarded() || !input.is_object()) {
        return false;
      }

      for (unsigned int i = 0; i < mFb->getNumRD(); i++) {
        const std::string field = getFieldName(i);
        if (!input.contains(field)) {
          continue;
        }

        CIEC_ANY *destination = mFb->getRDs()[i];

        if (nullptr == destination) {
          return false;
        }

        CIEC_ANY &value = destination->unwrap();

        if (!deserializeValue(input[field], value)) {
          return false;
        }
      }

      return true;
    }

    template<typename T>
    bool getNumber(const json &j, T &out) {
      if constexpr (std::is_integral_v<T>) {
        if (!j.is_number_integer())
          return false;

        auto v = j.get<int64_t>();

        if (v < std::numeric_limits<T>::min() || v > std::numeric_limits<T>::max())
          return false;
        out = static_cast<T>(v);
      } else if constexpr (std::is_floating_point_v<T>) {
        if (!j.is_number())
          return false;

        auto v = j.get<double>();

        if (!std::isfinite(v))
          return false;

        if (v < std::numeric_limits<T>::lowest() || v > std::numeric_limits<T>::max())
          return false;
        out = static_cast<T>(v);
      }

      return true;
    }

    bool CJSONComLayer::deserializeValue(const json &input, CIEC_ANY &value) {

      switch (value.getDataTypeID()) {

        case CIEC_ANY::e_BOOL: static_cast<CIEC_BOOL &>(value) = CIEC_BOOL(input.get<bool>()); break;

        case CIEC_ANY::e_BYTE: {
          TForteByte v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_BYTE &>(value) = CIEC_BYTE(v);
          break;
        }

        case CIEC_ANY::e_WORD: {
          TForteWord v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_WORD &>(value) = CIEC_WORD(v);
          break;
        }

        case CIEC_ANY::e_DWORD: {
          TForteDWord v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_DWORD &>(value) = CIEC_DWORD(v);
          break;
        }

        case CIEC_ANY::e_LWORD: {
          TForteLWord v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_LWORD &>(value) = CIEC_LWORD(v);
          break;
        }

        case CIEC_ANY::e_SINT: {
          TForteInt8 v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_SINT &>(value) = CIEC_SINT(v);
          break;
        }

        case CIEC_ANY::e_INT: {
          TForteInt16 v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_INT &>(value) = CIEC_INT(v);
          break;
        }

        case CIEC_ANY::e_DINT: {
          TForteInt32 v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_DINT &>(value) = CIEC_DINT(v);
          break;
        }

        case CIEC_ANY::e_LINT: {
          TForteInt64 v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_LINT &>(value) = CIEC_LINT(v);
          break;
        }

        case CIEC_ANY::e_USINT: {
          TForteUInt8 v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_USINT &>(value) = CIEC_USINT(v);
          break;
        }

        case CIEC_ANY::e_UINT: {
          TForteUInt16 v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_UINT &>(value) = CIEC_UINT(v);
          break;
        }

        case CIEC_ANY::e_UDINT: {
          TForteUInt32 v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_UDINT &>(value) = CIEC_UDINT(v);
          break;
        }

        case CIEC_ANY::e_ULINT: {
          TForteUInt64 v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_ULINT &>(value) = CIEC_ULINT(v);
          break;
        }

        case CIEC_ANY::e_REAL: {
          TForteFloat v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_REAL &>(value) = CIEC_REAL(v);
          break;
        }

        case CIEC_ANY::e_LREAL: {
          TForteDFloat v;
          if (!getNumber(input, v))
            return false;
          static_cast<CIEC_LREAL &>(value) = CIEC_LREAL(v);
          break;
        }

        case CIEC_ANY::e_STRING: {
          if (!input.is_string())
            return false;
          const std::string str = input.get<std::string>();
          CIEC_STRING &dst = static_cast<CIEC_STRING &>(value);
          dst.assign(str.c_str(), static_cast<TForteUInt16>(str.size()));
          break;
        }

        case CIEC_ANY::e_WSTRING: {
          if (!input.is_string())
            return false;
          const std::string str = input.get<std::string>();
          const int result =
              static_cast<CIEC_WSTRING &>(value).fromUTF8(str.data(), static_cast<int>(str.size()), false);
          if (result < 0)
            return false;
          break;
        }

        case CIEC_ANY::e_CHAR: {
          const std::string str = input.get<std::string>();
          if (str.empty())
            return false;
          CIEC_CHAR &dst = static_cast<CIEC_CHAR &>(value);
          dst.fromString(str.c_str());
          break;
        }

        case CIEC_ANY::e_WCHAR: {
          const std::string str = input.get<std::string>();
          CIEC_WSTRING tmp;
          tmp.assign(str.c_str(), static_cast<TForteUInt16>(str.size()));
          TForteWChar wc = 0;
          if (tmp.toUTF16(reinterpret_cast<TForteByte *>(&wc), sizeof(TForteWChar)) < 0)
            return false;
          static_cast<CIEC_WCHAR &>(value) = CIEC_WCHAR(wc);
          break;
        }

        case CIEC_ANY::e_STRUCT: {
          CIEC_STRUCT &structure = static_cast<CIEC_STRUCT &>(value);
          if (!input.is_object())
            return false;
          const StringId *names = structure.elementNames();

          for (size_t i = 0; i < structure.getStructSize(); i++) {
            std::string memberName;
            if (names)
              memberName = names[i].get();
            else
              memberName = "member" + std::to_string(i);

            if (input.contains(memberName)) {
              CIEC_ANY *member = structure.getMember(i);
              if (nullptr == member)
                return false;
              if (!deserializeValue(input[memberName], *member))
                return false;
            }
          }
          break;
        }
        case CIEC_ANY::e_ARRAY: {
          CIEC_ARRAY &array = static_cast<CIEC_ARRAY &>(value);
          if (!input.is_array())
            return false;

          const size_t arraySize = array.size();
          const size_t inputSize = input.size();
          const size_t count = std::min(arraySize, inputSize);

          for (size_t i = 0; i < count; i++) {
            CIEC_ANY &element = array[array.getLowerBound() + static_cast<intmax_t>(i)];
            if (!deserializeValue(input[i], element))
              return false;
          }
          break;
        }

        default: {
          return false;
        }
      }

      return true;
    }

  } // namespace com_infra
} // namespace forte
