/*******************************************************************************
 * Copyright (c) 2026 Aimirim STI
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure: This file was largely AI-generated. The AI-generated
 * portions are made available under CC0-1.0 and not subject to the
 * project's licence. The human contributor has reviewed and verified
 * that the code is correct.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: Anthropic Claude Opus 5.5 (claude-opus-5-5)
 *
 * Contributors:
 *   Pedro Ricardo C. Souza - initial implementation
 *******************************************************************************/

#pragma once

#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include "forte/cfb.h"
#include "forte/interfacespec.h"
#include "forte/mgmcmd.h"
#include "forte/stringid.h"

struct lua_State;

namespace forte {
  class CIEC_ANY;
}

namespace forte::lua {

  //! Whether paName is an identifier, optionally qualified with packages (pkg::sub::Name)
  bool isValidTypeName(std::string_view paName);

  //! The data type of one variable of a Lua type, an elementary or compiled type or an array of one
  struct SLuaDataType {
      StringId mTypeName;
      bool mIsArray = false;
      intmax_t mLowerBound = 0;
      intmax_t mUpperBound = 0;
      //! The IEC 61131-3 literal the variable starts with, empty for the default of its type
      std::string mInitialValue;

      //! Creates a value holding the initial value
      [[nodiscard]] std::unique_ptr<CIEC_ANY> createValue() const;
      //! Sets paValue, a value created by createValue, back to the initial value
      void setInitialValue(CIEC_ANY &paValue) const;
  };

  struct SLuaAdapterDefinition {
      StringId mName;
      StringId mTypeName;
      bool mIsPlug;
  };

  /*!\brief The interface of a Lua type, read from the interfaceSpec table of its module
   *
   * Owns all names and WITH lists, and an SFBInterfaceSpec spanning them. Adapters are kept in the order of
   * their Lua adapter ID: plugs first, then sockets, each in declaration order.
   */
  class CLuaInterface {
    public:
      //! Upper bound for events per list, so adapter event IDs ((list ID + 1) << 8 | event) stay unambiguous
      static constexpr std::size_t scmMaxEvents = 255;
      //! Data points per list and internal variables, from the 16 bit index of the access constants
      static constexpr std::size_t scmMaxDataPoints = 65535;
      //! Adapters per type, from the 8 bit adapter ID and the list ID reserved for composite interfaces
      static constexpr std::size_t scmMaxAdapters = 255;
      //! Terminates a run in the EIWith and EOWith lists
      static constexpr TPortId scmWithDelimiter = 65535;

      CLuaInterface() = default;
      CLuaInterface(const CLuaInterface &) = delete;
      CLuaInterface &operator=(const CLuaInterface &) = delete;

      /*!\brief Reads the interfaceSpec table at paIndex
       *
       * The optional lists DIInitialValues and DOInitialValues hold one IEC 61131-3 literal per data point, an
       * empty string for the default of its type. Each literal must parse completely as the declared type.
       * In-out variables are optional: numDIOs, DIONames, DIODataTypeNames and DIOInitialValues. In EIWith an index
       * from numDIs on names in-out variable index - numDIs, in EOWith one from numDOs on in-out index - numDOs.
       *
       * @param paWithAdapters whether numAdapters and adapterInstanceDefinition are read (FB types)
       * @param paError receives a description of the problem
       * @return Ready, BadParams for a malformed table, UnsupportedType for an unknown data or adapter type
       */
      EMGMResponse parse(lua_State *paState, int paIndex, bool paWithAdapters, std::string &paError);

      //! Reads the internalVarsInformation table at paIndex with its optional intVarsInitialValues, as parse
      EMGMResponse parseInternals(lua_State *paState, int paIndex, std::string &paError);

      [[nodiscard]] const SFBInterfaceSpec &getSpec() const {
        return mSpec;
      }

      //! The interface of an adapter's plug side: inputs and outputs of the declared interface swapped
      [[nodiscard]] const SFBInterfaceSpec &getMirroredSpec() const {
        return mMirroredSpec;
      }

      //! Data input indexes sampled with the event input
      [[nodiscard]] std::span<const TPortId> getEIWith(TEventID paEI) const {
        return mEIWith[paEI];
      }

      //! Data output indexes written with the event output
      [[nodiscard]] std::span<const TPortId> getEOWith(TEventID paEO) const {
        return mEOWith[paEO];
      }

      [[nodiscard]] std::span<const SLuaDataType> getDITypes() const {
        return mDITypes;
      }

      [[nodiscard]] std::span<const SLuaDataType> getDOTypes() const {
        return mDOTypes;
      }

      [[nodiscard]] std::span<const SLuaDataType> getDIOTypes() const {
        return mDIOTypes;
      }

      [[nodiscard]] std::span<const StringId> getInternalNames() const {
        return mInternalNames;
      }

      [[nodiscard]] std::span<const SLuaDataType> getInternalTypes() const {
        return mInternalTypes;
      }

      //! All adapters by Lua adapter ID
      [[nodiscard]] std::span<const SLuaAdapterDefinition> getAdapters() const {
        return mAdapters;
      }

      [[nodiscard]] std::size_t getNumPlugs() const {
        return mPlugNames.size();
      }

      //! Creates one value per entry of paTypes
      static std::vector<std::unique_ptr<CIEC_ANY>> createValues(std::span<const SLuaDataType> paTypes);
      //! Sets each of paValues, created by createValues from paTypes, back to its initial value
      static void setInitialValues(std::span<const SLuaDataType> paTypes,
                                   std::span<const std::unique_ptr<CIEC_ANY>> paValues);

    private:
      EMGMResponse parseEvents(lua_State *paState,
                               int paIndex,
                               const char *paPrefix,
                               std::vector<StringId> &paNames,
                               std::vector<std::vector<TPortId>> &paWith,
                               std::string &paError);
      EMGMResponse parseWith(lua_State *paState,
                             int paIndex,
                             const char *paPrefix,
                             std::size_t paNumDataPoints,
                             std::vector<std::vector<TPortId>> &paWith,
                             std::string &paError);
      EMGMResponse parseData(lua_State *paState,
                             int paIndex,
                             const char *paNumKey,
                             const char *paNamesKey,
                             const char *paTypesKey,
                             const char *paInitialValuesKey,
                             std::size_t paMaxNum,
                             std::vector<StringId> &paNames,
                             std::vector<SLuaDataType> &paTypes,
                             std::string &paError);
      EMGMResponse parseAdapters(lua_State *paState, int paIndex, std::string &paError);
      void buildSpecs();

      std::vector<StringId> mEINames;
      std::vector<StringId> mEONames;
      std::vector<StringId> mDINames;
      std::vector<StringId> mDONames;
      std::vector<StringId> mDIONames;
      std::vector<StringId> mPlugNames;
      std::vector<StringId> mSocketNames;
      std::vector<StringId> mInternalNames;
      std::vector<std::vector<TPortId>> mEIWith;
      std::vector<std::vector<TPortId>> mEOWith;
      std::vector<SLuaDataType> mDITypes;
      std::vector<SLuaDataType> mDOTypes;
      std::vector<SLuaDataType> mDIOTypes;
      std::vector<SLuaDataType> mInternalTypes;
      std::vector<SLuaAdapterDefinition> mAdapters;
      SFBInterfaceSpec mSpec{};
      SFBInterfaceSpec mMirroredSpec{};
  };

  //! An FB instance inside the network of a Lua composite type
  struct SLuaInternalFB {
      StringId mName;
      StringId mTypeName;
  };

  //! A parameter of an internal FB, the literal is parsed when an instance is created
  struct SLuaParameter {
      StringId mFBName;
      StringId mDIName;
      std::string mValue;
  };

  /*!\brief The network of a Lua composite type, read from the fbnSpec table of its module
   *
   * Connections name their endpoints by FB number: the position in internalFBs, -1 for the composite's own
   * interface, or 65536 + i for its own adapter i (plugs first, then sockets). They are converted to the names
   * a CCompositeFB connects by. A fanned out connection becomes a plain connection from the source of its primary.
   * The composite's own in-out variables are data sources only, as in a compiled composite, and do not drive its
   * outputs.
   */
  class CLuaFBNetwork {
    public:
      //! Upper bound for internal FBs, connections per kind and parameters
      static constexpr std::size_t scmMaxEntries = 65535;
      //! The FB number of the composite's own interface
      static constexpr intmax_t scmOwnInterface = -1;
      //! The FB number of the composite's own adapter 0
      static constexpr intmax_t scmOwnAdapterBase = 0x10000;

      CLuaFBNetwork() = default;
      CLuaFBNetwork(const CLuaFBNetwork &) = delete;
      CLuaFBNetwork &operator=(const CLuaFBNetwork &) = delete;

      /*!\brief Reads the fbnSpec table at paIndex
       *
       * @param paInterface the composite's interface, already read
       * @param paTypeNameId the composite's type name, an internal FB of the same type is rejected
       * @return Ready or BadParams
       */
      EMGMResponse parse(lua_State *paState,
                         int paIndex,
                         const CLuaInterface &paInterface,
                         StringId paTypeNameId,
                         std::string &paError);

      [[nodiscard]] const SCFB_FBNData &getFBNData() const {
        return mFBNData;
      }

      [[nodiscard]] std::span<const SLuaInternalFB> getInternalFBs() const {
        return mInternalFBs;
      }

      [[nodiscard]] std::span<const SLuaParameter> getParameters() const {
        return mParameters;
      }

    private:
      enum class EConnectionKind { Event, Data, Adapter };

      EMGMResponse parseInternalFBs(lua_State *paState, int paIndex, StringId paTypeNameId, std::string &paError);
      EMGMResponse parseParameters(lua_State *paState, int paIndex, std::string &paError);
      EMGMResponse parseConnections(lua_State *paState,
                                    int paIndex,
                                    const char *paKey,
                                    const char *paCountKey,
                                    const char *paFannedOutKey,
                                    const char *paFannedOutCountKey,
                                    EConnectionKind paKind,
                                    const CLuaInterface &paInterface,
                                    std::vector<SCFB_FBConnectionData> &paConnections,
                                    std::string &paError);
      //! Reads one endpoint of the connection table at paIndex
      EMGMResponse readEndpoint(lua_State *paState,
                                int paIndex,
                                bool paIsSource,
                                EConnectionKind paKind,
                                const CLuaInterface &paInterface,
                                StringId &paFBName,
                                StringId &paPortName,
                                std::string &paError) const;

      std::vector<SLuaInternalFB> mInternalFBs;
      std::vector<SLuaParameter> mParameters;
      std::vector<SCFB_FBConnectionData> mEventConnections;
      std::vector<SCFB_FBConnectionData> mDataConnections;
      std::vector<SCFB_FBConnectionData> mAdapterConnections;
      SCFB_FBNData mFBNData{};
  };

  /*!\brief Keeps the type entry of a Lua type instance alive and counts the instance
   *
   * Must be the first base of an instance class: the interface spec handed to the function block base lives in
   * the entry.
   */
  template<typename T>
  class CLuaTypeHolder {
    protected:
      explicit CLuaTypeHolder(std::shared_ptr<const T> paTypeEntry) : mTypeEntry(std::move(paTypeEntry)) {
        mTypeEntry->addInstance();
      }

      ~CLuaTypeHolder() {
        mTypeEntry->removeInstance();
      }

      CLuaTypeHolder(const CLuaTypeHolder &) = delete;
      CLuaTypeHolder &operator=(const CLuaTypeHolder &) = delete;

      const std::shared_ptr<const T> mTypeEntry;
  };

} // namespace forte::lua
