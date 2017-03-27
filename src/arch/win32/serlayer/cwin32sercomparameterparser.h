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


#ifndef CWIN32SERCOMPARAMETERPARSER_H_
#define CWIN32SERCOMPARAMETERPARSER_H_

//ser[interface, baudrate, bytesize, stopbits, parity, termination_symbol]

class CWin32SerComParameterParser {
public:
	explicit CWin32SerComParameterParser(char* pa_pcParameters);
	virtual ~CWin32SerComParameterParser();

	void setSeparator(char pa_cSeparator);
	void parseParameters();
	void moveToNextParameter();
	char* getCurrentParameter();

	char* operator[](int const& pa_cnIndex);

	enum ESerComParameter
	{
		eInterface = 0,
		eBaudrate,
		eByteSize,
		eStopBits,
		eParity,
		eTerminationSymbol,
		eSerComParamterAmount
	};


private:
	static int const scm_nAmountOfParameters = eSerComParamterAmount; /**< Specifies the expected amount of parameters */
	int m_nCurrentParameter;
	char *m_pcParameterSeparators[scm_nAmountOfParameters];
	char m_cSeparator;
	char *const m_pcParameters;
	char *m_pcParsePosition;

	void moveToPositionOfFirstNonWhiteSpaceCharacter();
	void saveStartPositionForParameterSubstring(int paParameterNumber);
	void moveToPositionOfNextParameterSeparatorOrEndOfString();
	void trimTrailingWhiteSpacesOfParameterSubstring();
	void moveToNextParameterStart();
};

#endif /* CWIN32SERCOMPARAMETERPARSER_H_ */
