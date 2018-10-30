#include <iostream>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
using namespace CppUnit;

int main(int argc, char **argv)
{
  // Create the event manager and test controller
  TestResult controller;

  // Add a listener that colllects test result
  TestResultCollector result;
  controller.addListener(&result);

  // Add a listener that print dots as test run.
  BriefTestProgressListener progress;
  controller.addListener(&progress);

  // Add the top suite to the test runner
  TestRunner runner;
  runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry("test1").makeTest());
  runner.run(controller);

  return result.wasSuccessful() ? 0 : 1;
}
