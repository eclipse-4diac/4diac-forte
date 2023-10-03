/*******************************************************************************
 * Copyright (c) 2023 - Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "E_CTUD.h"
#include "basicfb.h"
#include "internalvartests_gen.cpp"



BOOST_AUTO_TEST_SUITE(FUNCBLOC)

BOOST_AUTO_TEST_CASE(FB_TO_STRING_TEST){
    FORTE_E_CTUD testFb(0, nullptr); // Dummy FB, do not use for anything else than testing toString
    constexpr char result[] = "(PV:=0, QU:=FALSE, QD:=FALSE, CV:=0)";
    char buffer[50];
    BOOST_TEST(testFb.toString(buffer, sizeof(buffer)) == strlen(result));
    BOOST_TEST(buffer == result);
}

BOOST_AUTO_TEST_CASE(FB_TO_STRING_BUFFER_SIZE_TEST_WITH_INRENAL_VAR){

    // Test for FB with internal vars

    class CInternalVarTestFB : public CBasicFB {
        const SFBInterfaceSpec gcEmptyInterface = {
            0, nullptr, nullptr, nullptr,
            0, nullptr, nullptr, nullptr,
            0, nullptr, nullptr,
            0, nullptr, nullptr,
            0, nullptr,
            0, nullptr};

    public:
        CInternalVarTestFB(const SInternalVarsInformation *paVarInternals) : CBasicFB(nullptr, &gcEmptyInterface, CStringDictionary::scmInvalidStringId, paVarInternals) {
        }

        CIEC_ANY *getVarInternal(size_t paVarIntNum) override {
            return CBasicFB::getVarInternal(paVarIntNum);
        }

        CStringDictionary::TStringId getFBTypeId() const override {
            return CStringDictionary::scmInvalidStringId;
        }

        void executeEvent(TEventID) override {
            // nothiing to do here
        }

        void readInputData(TEventID) override {
        }

        void writeOutputData(TEventID) override {
        }
};

    CStringDictionary::TStringId varInternalNames[] = {g_nStringIdQU, g_nStringIdQD, g_nStringIdCV};
    CStringDictionary::TStringId varInternalTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT};
    SInternalVarsInformation varData{3, varInternalNames, varInternalTypeIds};
    CInternalVarTestFB testFb(&varData);
    BOOST_ASSERT(testFb.initialize());
    size_t size = testFb.getToStringBufferSize();
    BOOST_CHECK_EQUAL(size , 39);
}

BOOST_AUTO_TEST_CASE(FB_TO_STRING_BUFFER_SIZE_TEST_WITHOUT_INRENAL_VAR){

    // Test for FB with inputs and outputs
    FORTE_E_CTUD testFb(0, nullptr); // Dummy FB, do not use for anything else than testing getToStringBufferSize
    size_t size = testFb.getToStringBufferSize();
    BOOST_CHECK_EQUAL(size , 51);
}

BOOST_AUTO_TEST_SUITE_END()