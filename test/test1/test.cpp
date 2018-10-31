#include "test.h"
#include <cppunit/TestCase.h>
#include "cppunit/TestAssert.h"
#include "../../src/vender/easylogging++.h"

// CPPUNIT_TEST_SUITE_REGISTRATION(Test);

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Test, "test1");

INITIALIZE_EASYLOGGINGPP;

using namespace std;

void Test::testHelloWorld3()
{
  cout << "test2" << endl;

  LOG(INFO) << "My first info log using default logger";
  LOG(TRACE) << "***** trace log  *****";
  LOG(DEBUG) << "***** debug log  *****";
  LOG(ERROR) << "***** error log  *****";
  LOG(WARNING) << "***** warning log  *****";
  LOG(INFO) << "***** info log  *****";
}

void Test::testHelloWorld2()
{
  cout << "test2" << endl;
  int resulty = 3;
  CPPUNIT_ASSERT(resulty == 4);
}

void Test::testHelloWorld1()
{
  cout << "test1" << endl;

  int resultx = 2;
  CPPUNIT_ASSERT_MESSAGE("error", resultx == 3);
}

void Test::setUp(void)
{
  el::Configurations conf("../../default-logger.conf");
  el::Loggers::reconfigureAllLoggers(conf);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  /// 防止Fatal级别日志中断程序
  el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
  /// 选择划分级别的日志
  el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
  /// 设置级别门阀值，修改参数可以控制日志输出
  el::Loggers::setLoggingLevel(el::Level::Global);
}
void Test::tearDown(void)
{
}