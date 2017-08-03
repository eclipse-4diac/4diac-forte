/*******************************************************************************
 * Copyright (c) 2016 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/


#include "cwin32sercomparameterparser.h"
#include <string.h>
#include <cctype>

CWin32SerComParameterParser::CWin32SerComParameterParser(char* pa_pcParameters) : m_pcParameters(pa_pcParameters), m_cSeparator(0), m_pcParsePosition(0)  {
	m_nCurrentParameter = 0;
	memset(m_pcParameterSeparators, 0, sizeof(m_pcParameterSeparators)); //TODO change this to  m_pcParameterSeparators{0} in the extended list when fully switching to C++11
}

CWin32SerComParameterParser::~CWin32SerComParameterParser() {
	// TODO Auto-generated destructor stub
}

void CWin32SerComParameterParser::setSeparator(char pa_cSeparator) {
	m_cSeparator = pa_cSeparator;
}

void CWin32SerComParameterParser::parseParameters() {
	m_pcParsePosition = m_pcParameters;
	for(int i = 0; i < scm_nAmountOfParameters; ++i) {
		moveToPositionOfFirstNonWhiteSpaceCharacter();
		saveStartPositionForParameterSubstring(i);
		moveToPositionOfNextParameterSeparatorOrEndOfString();
		trimTrailingWhiteSpacesOfParameterSubstring();
		moveToNextParameterStart();
	}
}

void CWin32SerComParameterParser::moveToPositionOfFirstNonWhiteSpaceCharacter() {
	while (0 != isspace(*m_pcParsePosition)) {
		++m_pcParsePosition;
	}
}

void CWin32SerComParameterParser::saveStartPositionForParameterSubstring(int paParameterNumber) {
	m_pcParameterSeparators[paParameterNumber] = m_pcParsePosition;
}

void CWin32SerComParameterParser::moveToPositionOfNextParameterSeparatorOrEndOfString() {
	while (m_cSeparator != *m_pcParsePosition && '\0' != *m_pcParsePosition) {
		++m_pcParsePosition;
	}
}

void CWin32SerComParameterParser::trimTrailingWhiteSpacesOfParameterSubstring() {
	char* backTraceCharacter = m_pcParsePosition;
	*backTraceCharacter = ' ';
	while( 0 != isspace(*backTraceCharacter) ) {
		--backTraceCharacter;
	}
	backTraceCharacter++;
	*backTraceCharacter = '\0';
}

void CWin32SerComParameterParser::moveToNextParameterStart() {
	++m_pcParsePosition;
}

void CWin32SerComParameterParser::moveToNextParameter() {
	m_nCurrentParameter < scm_nAmountOfParameters ? ++m_nCurrentParameter : m_nCurrentParameter;
}

char* CWin32SerComParameterParser::getCurrentParameter() {
	return m_pcParameterSeparators[m_nCurrentParameter];
}

char* CWin32SerComParameterParser::operator [](const int& index) {
	return m_pcParameterSeparators[index];
}
