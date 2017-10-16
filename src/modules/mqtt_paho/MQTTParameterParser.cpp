/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/


#include "MQTTParameterParser.h"
#include <string.h>
#include <cctype>

MQTTParameterParser::MQTTParameterParser(char* paParameters) : mParameters(paParameters), mSeparator(0), parsePosition(0) {
	mCurrentParameter = 0;
}

MQTTParameterParser::~MQTTParameterParser() {
	// TODO Auto-generated destructor stub
}

void MQTTParameterParser::setSeparator(char paSeparator) {
	mSeparator = paSeparator;
}

int MQTTParameterParser::parseParameters() {
	parsePosition = mParameters;
	int i;
	for(i = 0; ((i < mAmountOfParameters) && ('\0' != *parsePosition)); ++i) {
		moveToPositionOfFirstNonWhiteSpaceCharacter();
		saveStartPositionForParameterSubstring(i);
		moveToPositionOfNextParameterSeparatorOrEndOfString();
		trimTrailingWhiteSpacesOfParameterSubstring();
		moveToNextParameterStart();
	}
	return i;
}

void MQTTParameterParser::moveToPositionOfFirstNonWhiteSpaceCharacter() {
	while (0 != isspace(*parsePosition)) {
		++parsePosition;
	}
}

void MQTTParameterParser::saveStartPositionForParameterSubstring(int paParameterNumber) {
	mParameterSeparators[paParameterNumber] = parsePosition;
}

void MQTTParameterParser::moveToPositionOfNextParameterSeparatorOrEndOfString() {
	while (mSeparator != *parsePosition && '\0' != *parsePosition) {
		++parsePosition;
	}
}

void MQTTParameterParser::trimTrailingWhiteSpacesOfParameterSubstring() {
	char* backTraceCharacter = parsePosition;
	*backTraceCharacter = ' ';
	while( 0 != isspace(*backTraceCharacter) ) {
		--backTraceCharacter;
	}
	backTraceCharacter++;
	*backTraceCharacter = '\0';
}

void MQTTParameterParser::moveToNextParameterStart() {
	++parsePosition;
}

void MQTTParameterParser::moveToNextParameter() {
	mCurrentParameter < mAmountOfParameters ? ++mCurrentParameter: mCurrentParameter;
}

char* MQTTParameterParser::getCurrentParameter() {
	return mParameterSeparators[mCurrentParameter];
}

char* MQTTParameterParser::operator [](const int& index) {
	return mParameterSeparators[index];
}
