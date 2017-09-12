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
#include "DEV_MGR.h"

class ForteBootFileLoader {
public:
	ForteBootFileLoader(DEV_MGR &paDevMgr);
	~ForteBootFileLoader();

private:
	char* bootFileName;
	FILE *bootfile;
	DEV_MGR &paDevMgr;

	void loadBootFile();
	void setBootFileName();
	bool checkBootFile();
	void openBootFile();
};

#endif /* SRC_STDFBLIB_ITA_FORTEBOOTFILELOADER_H_ */
