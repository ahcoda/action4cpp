#ifndef _TEST1_H_
#define _TEST1_H_

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

class Test : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(Test);
  CPPUNIT_TEST(testHelloWorld1);
  CPPUNIT_TEST(testHelloWorld2);
  CPPUNIT_TEST(testHelloWorld3);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void);
  void tearDown(void);

protected:
  void testHelloWorld1(void);
  void testHelloWorld2(void);
  void testHelloWorld3(void);
};

#endif