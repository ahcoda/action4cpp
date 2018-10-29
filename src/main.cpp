#include <stdio.h>
#include <iostream>
#include "vender/easylogging++.h"
using namespace std;
INITIALIZE_EASYLOGGINGPP;

#define ELPP_LOGGING_FLAGS_FROM_ARG
void initLogger(int argc, char **argv)
{
    START_EASYLOGGINGPP(argc, argv);

    el::Configurations conf("../default-logger.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    /// 防止Fatal级别日志中断程序
    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    /// 选择划分级别的日志
    el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
    /// 设置级别门阀值，修改参数可以控制日志输出
    el::Loggers::setLoggingLevel(el::Level::Global);
}
int main(int argc, char **argv)
{
    initLogger(argc, argv);

    LOG(INFO) << "My first info log using default logger";
    LOG(TRACE) << "***** trace log  *****";
    LOG(DEBUG) << "***** debug log  *****";
    LOG(ERROR) << "***** error log  *****";
    LOG(WARNING) << "***** warning log  *****";
    LOG(INFO) << "***** info log  *****";
    int s = 100;
    cout << "x = " << s << endl;

    /// Debug模式日志记录
    DLOG(TRACE);
    DLOG(DEBUG);
    DLOG(FATAL);
    DLOG(ERROR);
    DLOG(WARNING);
    DLOG(INFO);
    DVLOG(0);

    return 0;
}
