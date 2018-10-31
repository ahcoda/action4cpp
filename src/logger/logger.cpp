#include "logger.h"

INITIALIZE_EASYLOGGINGPP;
#define ELPP_LOGGING_FLAGS_FROM_ARG
void init_logger(int argc, char **argv)
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
