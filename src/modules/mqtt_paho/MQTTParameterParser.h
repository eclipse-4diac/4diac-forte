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


#ifndef MQTTPARAMETERPARSER_H_
#define MQTTPARAMETERPARSER_H_

//mqtt[tcp://localhost:1883, ClientID, Topic]

class MQTTParameterParser {
public:
	explicit MQTTParameterParser(char* paParameters);
	virtual ~MQTTParameterParser();

	void setSeparator(char paSeparator);
	int parseParameters();
	void moveToNextParameter();
	char* getCurrentParameter();

	char* operator[](int const& index);


private:
	static int const mAmountOfParameters = 3;
	int mCurrentParameter;
	char* mParameterSeparators[mAmountOfParameters];
	char* const mParameters;
	char mSeparator;
	char* parsePosition;

	void moveToPositionOfFirstNonWhiteSpaceCharacter();
	void saveStartPositionForParameterSubstring(int paParameterNumber);
	void moveToPositionOfNextParameterSeparatorOrEndOfString();
	void trimTrailingWhiteSpacesOfParameterSubstring();
	void moveToNextParameterStart();
};

#endif /* MQTTPARAMETERPARSER_H_ */
