/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_STDFBLIB_ITA_FORTEBOOTFILELOADER_H_
#define SRC_STDFBLIB_ITA_FORTEBOOTFILELOADER_H_

#include <stdio.h>
#include <stdlib.h>
#include "IBootFileCallback.h"
#include "../../../src/core/datatypes/forte_string.h"

enum LoadBootResult {
  OK,
  MISSING_COLON,
  FILE_NOT_OPENED,
  EXTERNAL_ERROR,
};

class ForteBootFileLoader {
public:

  /**
   * Constructor which uses the the default values for the boot file location
   * @param paCallback Object to be called for each command
   */
  explicit ForteBootFileLoader(IBootFileCallback &paCallback);

  /**
   * Constructor which uses a specific bootfile name instead of the default one
   * @param paCallback Object to be called for each command
   * @param paBootFileName Specific boot file name to be opened
   */
	explicit ForteBootFileLoader(IBootFileCallback &paCallback, CIEC_STRING &paBootFileName);
	~ForteBootFileLoader();

	LoadBootResult loadBootFile();

	bool isOpen() const{
	  return (0 != bootfile);
	}

private:
	FILE *bootfile;
	IBootFileCallback &mCallback; //for now with one callback is enough for all cases

  bool openBootFile(CIEC_STRING* paBootFileName);
	bool readLine(CIEC_STRING &line);
	bool checkCommandEnding(const CIEC_STRING &line) const;
};

#endif /* SRC_STDFBLIB_ITA_FORTEBOOTFILELOADER_H_ */
