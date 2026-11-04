/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: GEN_F_MUX
 *** Description: multiplexer
 *** Version:
 ***     1.0: 2013-08-30/Monika Wenger - TU Wien ACIN -
 ***     1.1: 2024-07-15/Franz Höpfinger - HR Agrartechnik GmbH - F_MUX_2 copy and added 1 IN
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 *************************************************************************/

#include "forte/iec61131/selection/GEN_F_MUX_fbt.h"

#include "forte/util/string_utils.h"
#include "forte/iec61131_functions/func_MUX.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61131::selection {
  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(GEN_F_MUX, "iec61131::selection::GEN_F_MUX"_STRID)

  GEN_F_MUX::GEN_F_MUX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      var_K(CIEC_ANY_INT_VARIANT()),
      var_OUT(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_K(nullptr),
      conn_OUT(*this, 0, var_OUT){};

  void GEN_F_MUX::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: {
        const auto index = var_K.unwrap().getUnsignedValue();
        const auto tryCandidate = [this, index]<typename Target>(const Target &paCandidate) {
          for (std::size_t i = 0; i < getGenDINums(); ++i) {
            const bool compatible = std::visit(
                [&]<typename Source>(const Source &paInput) {
                  if constexpr (mux_detail::input<Target> && mux_detail::input<Source>) {
                    return mpl::is_implicitly_castable_v<Source, Target>;
                  } else if constexpr (std::is_same_v<Target, Source>) {
                    return paCandidate && paInput && mux_detail::same_type(*paCandidate, *paInput);
                  } else {
                    return false;
                  }
                },
                static_cast<const CIEC_ANY_VARIANT::variant &>(mGenDIs[i]));
            if (!compatible) {
              return false;
            }
          }
          Target result(paCandidate);
          if constexpr (mux_detail::input<Target>) {
            result.reset();
          } else {
            result->reset();
          }
          if (index < getGenDINums()) {
            for (std::size_t i = 0; i < getGenDINums(); ++i) {
              std::visit(
                  [&]<typename Source>(const Source &paInput) {
                    if constexpr (mux_detail::input<Target> && mux_detail::input<Source>) {
                      if constexpr (mpl::is_implicitly_castable_v<Source, Target>) {
                        result = func_MUX(i == index ? 1_UINT : 0_UINT, result, Target(paInput));
                      }
                    } else if constexpr (std::is_same_v<Target, Source>) {
                      if (i == index) {
                        result = paInput;
                      }
                    }
                  },
                  static_cast<const CIEC_ANY_VARIANT::variant &>(mGenDIs[i]));
            }
          } else {
            DEVLOG_ERROR("MUX selector is out of range.\n");
          }
          var_OUT = result;
          return true;
        };
        bool resolved = false;
        for (std::size_t i = 0; i < getGenDINums() && !resolved; ++i) {
          resolved = std::visit(tryCandidate, static_cast<const CIEC_ANY_VARIANT::variant &>(mGenDIs[i]));
        }
        if (!resolved) {
          resolved = std::apply([&](const auto &...paCandidates) { return (tryCandidate(paCandidates) || ...); },
                                mux_detail::numeric_candidates{});
        }
        if (!resolved) {
          DEVLOG_ERROR("MUX inputs have incompatible types.\n");
          var_OUT = CIEC_ANY_VARIANT{};
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
      }
    }
  }

  void GEN_F_MUX::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_K, conn_K);
        for (TPortId i = 0; i < getGenDINums(); ++i) {
          readData(i + getGenDIOffset(), mGenDIs[i], mGenDIConns[i]);
        }
        break;
      }
      default: break;
    }
  }

  void GEN_F_MUX::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(getFBInterfaceSpec().getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  bool GEN_F_MUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *pcPos = strrchr(paConfigString, '_'); // find last _
    if (pcPos == nullptr) {
      return false;
    }

    pcPos++; // move after underscore
    // we have an underscore and it is the first underscore after F_MUX
    unsigned int numDIs = static_cast<unsigned int>(util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("DIs: %d;\n", numDIs);

    if (numDIs < 2) {
      DEVLOG_ERROR("GEN_F_MUX must have at least 2 IN ports.\n");
      return false;
    }

    // now the number of needed eventInputs and dataOutputs are available in the integer array
    // create the dataInputs
    mDINames.emplace_back("K"_STRID);
    for (unsigned int i = 0; i < numDIs; ++i) {
      mDINames.emplace_back(StringId::insert("IN" + std::to_string(i)));
    }

    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;
    paInterfaceSpec.mDINames = mDINames;
    paInterfaceSpec.mDONames = cDataOutputNames;

    return true;
  }

  CIEC_ANY *GEN_F_MUX::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_K;
    }
    return &mGenDIs[paIndex - getGenDIOffset()];
  }

  CIEC_ANY *GEN_F_MUX::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *GEN_F_MUX::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  void GEN_F_MUX::createGenInputData() {
    if (getGenDINums()) {
      mGenDIs = std::make_unique<CIEC_ANY_VARIANT[]>(getGenDINums());
    }
  }

  CDataConnection **GEN_F_MUX::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_K;
      default: return CGenFunctionBlock::getDIConUnchecked(paIndex);
    }
    return nullptr;
  }

  CDataConnection *GEN_F_MUX::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

} // namespace forte::iec61131::selection
