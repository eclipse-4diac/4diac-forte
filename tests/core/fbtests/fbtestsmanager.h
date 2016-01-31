/*******************************************************************************
 * Copyright (c) 2011 - 2014 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef FBTESTSMANAGER_H_
#define FBTESTSMANAGER_H_

#include <stringdict.h>
#include <typelib.h>
#include <fortelist.h>
#include <resource.h>

class CFBTester;

typedef CFBTester *(*TFBTesterCreateFunc)(CResource *);


#define DECLARE_FB_TESTER(testerclass) \
  private: \
    const static CFBTesterEntry csm_oFBTesterEntry_##fbclass; \
  public:  \
    static CFBTester *createFBTester(CResource *m_poTestResource){ \
      return new testerclass(m_poTestResource); \
    }; \
    virtual CStringDictionary::TStringId getFBTypeId(void) const; \
  private:

#define DEFINE_FB_TESTER(testerclass, fbTypeNameId)\
  const CFBTesterEntry testerclass::csm_oFBTesterEntry_##fbclass((fbTypeNameId), testerclass::createFBTester); \
  CStringDictionary::TStringId testerclass::getFBTypeId(void) const {return (fbTypeNameId); }


class CFBTesterEntry : public CTypeLib::CTypeEntry{
  public:
    CFBTesterEntry(CStringDictionary::TStringId pa_nTypeNameId, TFBTesterCreateFunc pa_pfuncCreateTester);

    CFBTester *createFBInstance(CResource *m_poTestResource){
      return m_pfuncCreateTester(m_poTestResource);
    }
  private:
    TFBTesterCreateFunc m_pfuncCreateTester;
};


class CFBTestsManager{
  DECLARE_SINGLETON(CFBTestsManager)

  public:
  /**\brief Run the FB test for all registered function block tests.
   *
   * \return returns the number of unsuccessful test cases
   */
    unsigned int runAllTests(CResource *paRessource);
    void runTestsForFB(CStringDictionary::TStringId pa_nFBTypeNameId);

    void addFBTester(CFBTesterEntry* pa_poFBTester);

  private:
    void executeFBTests(CFBTesterEntry* pa_poFBTestEntery, CResource *paRessource);
    void clearTestLog();

    typedef CSinglyLinkedList<CFBTesterEntry*> TFBTesterList;

    TFBTesterList m_lstroFBTesterList;

    unsigned int m_nNumSuccessfulTestCases;
    unsigned int m_nNumUnsuccessfulTestCases;
};

#endif /* FBTESTSMANGER_H_ */
